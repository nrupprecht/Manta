//
// Created by Nathaniel Rupprecht on 2/1/21.
//

#include "LexerGenerator.h"
#include "LexerDFA.hpp"

using namespace Manta;

std::shared_ptr<LexerDFA> LexerGenerator::create_lexer(const std::string& filename) {
    std::ifstream fin(filename);
    if (fin.fail()) {
        cout << "Failed to open file [" << filename << "].\n";
        return nullptr;
    }
    // Create parser.
    return create_lexer(fin);
}

std::shared_ptr<LexerDFA> LexerGenerator::create_lexer(std::istream& in) {
    // Lambda function for passing whitespaces.
    auto pass_white_space = [&] () {
        char c;
        in.get(c);
        while (!in.eof() && (c==' ' || c=='\t')) {
            in.get(c);
        }
        in.putback(c);
    };

    // Clear old data, add a new head node.
    lexer_dfa.clear();
    all_lexemes.clear();
    int head_id = lexer_dfa.add_node();

    // Parse parser description.
    char c;
    in.get(c);
    while (!in.eof()) {
        // Start of a lexeme definition.
        if (c=='@') {
            std::string lexeme;
            in.get(c);
            while (!in.eof() && c != ':') {
                lexeme.push_back(c);
                in.get(c);
            } // Ends with c==':'

            // Make sure this is not a repeat lexeme. If not, store it.
            if (std::find(all_lexemes.begin(), all_lexemes.end(), lexeme)!=all_lexemes.end()) {
                cout << "ERROR: This lexeme already exists.\n";
            }
            else {
                all_lexemes.push_back(lexeme);
            }

            pass_white_space();

            /*
            A | B - Or
            (...) or [...] - Grouping.

            a-z   - Character range.

            --- Escape characters ---
            \s    - Space
            \n    - Newline
            \t    - Tab
            \0    - eof
            \\
            \[ \]
            \( \)
            \{ \}
            \|
            \+
            \-
            \*
            \?
            -------------------------
            */

            // Get the lexeme definition.
            auto [start_id, end_id] = get_sequence(in, '\n');
            // Set recent_id node to be accepting.
            lexer_dfa.set_accepting(end_id, static_cast<int>(all_lexemes.size()) - 1);
            // Lambda transition from head node to start_id node.
            lexer_dfa.add_transition(head_id, start_id);
        }
            // Full line comments.
        else if (c=='#') {
            while (!in.eof() && c!='\n') {
                in.get(c);
            }
        }
            // Command.
        else if (c=='%') {
            pass_white_space();
            string command;
            in.get(c);
            // Get the command and check what it is.
            while (!in.eof() && isalpha(c)) {
                command.push_back(c);
                in.get(c);
            }
            // End parsing.
            if (command=="End") {
                break;
            }
        }
        // Get next character.
        in.get(c);
    }
    // Reduce the NFA to a DFA.
    lexer_dfa = lexer_dfa.nfa_to_dfa();

    return std::shared_ptr<LexerDFA>(new LexerDFA(lexer_dfa, all_lexemes, reserved_tokens, skip_lexemes));
}

int LexerGenerator::accepts(const string& word) const {
    return lexer_dfa.accepts(word);
}

int LexerGenerator::check_status() const {
    return lexer_dfa.check_status();
}

char LexerGenerator::peek() const {
    return lexer_dfa.peek();
}

string LexerGenerator::lexeme_name(const int index) const {
    if (index < 0 || all_lexemes.size() <= index) {
        //throw InvalidIndex();
        return "";
    }
    return all_lexemes[index];
}

int LexerGenerator::reserved_index(const string& word) const {
    auto it = std::find(reserved_tokens.begin(), reserved_tokens.end(), word);
    if (it == reserved_tokens.end()) {
        return -1;
    }
    else {
        return std::distance(reserved_tokens.begin(), it);
    }
}

int LexerGenerator::add_reserved(const string& keyword) {
    // Make sure the keyword will be accepted by the lexer.
    if (accepts(keyword) < 0) {
        return -1;
    }
    // Check if the keyword already exists.
    auto it = std::find(reserved_tokens.begin(), reserved_tokens.end(), keyword);
    if (it == reserved_tokens.end()) {
        reserved_tokens.push_back(keyword);
        return static_cast<int>(reserved_tokens.size() + all_lexemes.size() - 1);
    }
    else {
        return static_cast<int>(std::distance(reserved_tokens.begin(), it) + all_lexemes.size());
    }
}

inline std::pair<int, int> LexerGenerator::get_sequence(std::istream& in, char terminator) {
    int start_id = lexer_dfa.add_node();
    int end_id = lexer_dfa.add_node();
    int recent_id = start_id;
    std::pair<int, int> ends(start_id, end_id);

    // Lambda function for making a sequence of nodes into a *.
    auto make_star = [&] (const int idi, const int idf) {
        lexer_dfa.add_transition(idi, idf);
        lexer_dfa.add_transition(idf, idi);
    };
    auto make_plus = [&] (const int idi, const int idf) {
        lexer_dfa.add_transition(idf, idi);
    };
    auto make_ques = [&] (const int idi, const int idf) {
        lexer_dfa.add_transition(idi, idf);
    };
    // Add (if needed) a modifier to a sequence of nodes.
    auto check_modifier = [&] (const int idi, const int idf) {
        char d = in.peek();
        bool get_next = true;
        if (d == '+') make_plus(idi, idf);
        else if (d == '*') make_star(idi, idf);
        else if (d == '?') make_ques(idi, idf);
        else get_next = false;
        // If necessary, get the next character.
        if (get_next) {
            in.get(d);
        }
    };
    // Lambda function that adds another node in the line.
    auto add_node = [&] (const char ci, const char cf) {
        int id = lexer_dfa.add_node();
        lexer_dfa.add_transition(recent_id, id, ci, cf);
        check_modifier(recent_id, id);
        // Adjust pointer.
        recent_id = id;
    };
    // Lambda function for adding a character node. Checks whether a *+? comes after the char.
    auto add_char = [&] (const char c) {
        int id = lexer_dfa.add_node();
        // Add the normal character transition.
        lexer_dfa.add_transition(recent_id, id, c);
        check_modifier(recent_id, id);
        // Adjust pointer.
        recent_id = id;
    };

    char c;
    in.get(c);
    while (!in.eof()) {
        // Terminator character.
        if (c == terminator) {
            // Link recent_id node with end_id node with a epsilon transition.
            lexer_dfa.add_transition(recent_id, end_id);
            return ends;
        }
        // Pass spaces and tabs
        else if (c == ' ' || c == '\t');
        // Comment (full line)
        else if (c == '#') {
            while (!in.eof() && c != '\n') {
                in.get(c);
            }
        }
        // Start of grouping.
        else if (c == '(' || c == '[') {
            auto [start, end] = c == '(' ? get_sequence(in, ')') : get_sequence(in, ']');
            lexer_dfa.add_transition(recent_id, start); // Link up with a epsilon transition.
            // Peek if a modifiers modifies the grouping.
            in.get(c);
            if (!in.eof()) {
                if      (c=='*') make_star(start, end);
                else if (c=='?') make_ques(start, end);
                else if (c=='+') make_plus(start, end);
                else in.putback(c);
            }
            recent_id = end;
        }

            // Escape character
        else if (c == '\\') {
            in.get(c); // Get next letter.
            if (in.eof()) {
                add_node('\\', '\\');
                continue;
            }
            // Any letter. Need to add two transitions.
            if (c == '@') {
                int id = lexer_dfa.add_node();
                lexer_dfa.add_transition(recent_id, id, 'a', 'z');
                lexer_dfa.add_transition(recent_id, id, 'A', 'Z');
                check_modifier(recent_id, id);
                recent_id = id;
            }
                // Any lower case letter.
            else if (c == 'a') add_node('a', 'z');
                // Any upper case letter.
            else if (c == 'A') add_node('A', 'Z');
                // Any number
            else if (c == 'd') add_node('0', '9');
                // Other escape characters.
            else {
                char d = 0;
                if     (c=='\\') d = '\\';
                else if (c=='n') d = '\n';
                else if (c=='r') d = '\r';
                else if (c=='t') d = '\t';
                else if (c=='s') d = ' ';
                else if (c=='(') d = '(';
                else if (c=='[') d = '[';
                else if (c=='{') d = '{';
                else if (c==')') d = ')';
                else if (c==']') d = ']';
                else if (c=='}') d = '}';
                else if (c=='|') d = '|';
                else if (c=='+') d = '+';
                else if (c=='-') d = '-';
                else if (c=='*') d = '*';
                else if (c=='?') d = '?';
                else if (c=='0') d = 0;
                // Make the node.
                add_char(d);
            }
        }
            // OR
        else if (c == '|') {
            // Link recent_id node with end_id node via a epsilon transition, set up to get the next bin in the OR statement
            lexer_dfa.add_transition(recent_id, end_id);
            recent_id = start_id;
        }
        // Newline - we should only encounter this if terminator == '\n'
        else if (c == '\n') {
            throw UnexpectedNewLine();
        }
        // General character. Could be the start of a char range.
        else {
            char d = in.peek();
            if (d == '-') { // Char range
                d = in.peek();
                if (d != terminator) {
                    add_node(c, d);
                }
                else { // Not actually a char range. Just a character and a '-' character.
                    add_char(c);
                    add_char('-');
                }
            }
            else {
                add_char(c);
            }
        }

        // Get next character
        in.get(c);
    }
    // In case of eof.
    lexer_dfa.add_transition(recent_id, end_id); // For completeness.
    return ends;
}

inline bool LexerGenerator::is_skip(int lexeme_id) {
    return std::find(skip_lexemes.begin(), skip_lexemes.end(), lexeme_id)!=skip_lexemes.end();
}