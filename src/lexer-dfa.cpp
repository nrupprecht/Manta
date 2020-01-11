#include "lexer-dfa.hpp"

namespace Manta {

  LexerDFA::~LexerDFA() {
    // If the file stream is open, close it.
    if (fin.is_open()) fin.close();
  }

  void LexerDFA::parse_file(const string& fileName) {
    // If the file stream is open, close it.
    if (fin.is_open()) fin.close();
    // Open the file and set the dfa's istream to be the ifstream.
    fin.open(fileName);
    if (!fin.fail()) lexer_dfa.set_stream(fin);
  }

  void LexerDFA::parse_string(const string& sentence) {
    // If the file stream is open, close it.
    if (fin.is_open()) fin.close();
    // Read the string into the stringstream and set the dfa's istream to be the stringstream.
    stream << sentence;
    lexer_dfa.set_stream(stream);
  }

  void LexerDFA::create_parser(string fileName) {
    std::ifstream fin(fileName);
    if (fin.fail()) {
      cout << "Failed to open file [" << fileName << "].\n";
      return;
    }
    // Create parser.
    create_parser(&fin);
  }

  void LexerDFA::create_parser(std::istream *in) {
    // Lambda function for passing whitespaces.
    auto pass_white_space = [&] () {
      char c;
      in->get(c);
      while (!in->eof() && (c==' ' || c=='\t')) in->get(c);
      in->putback(c);
    };

    // Clear old data, add a new head node.
    lexer_dfa.clear();
    all_lexemes.clear();
    int head_id = lexer_dfa.add_node();

    // Parse parser description.
    char c;
    in->get(c);
    while (!in->eof()) {
      // Start of a lexeme definition.
      if (c=='@') {
        string lexeme;
        in->get(c);
        while (!in->eof() && c!=':') {
          lexeme.push_back(c);
          in->get(c);
        } // Ends with c==':'

        // Make sure this is not a repeat lexeme. If not, store it.
        if (std::find(all_lexemes.begin(), all_lexemes.end(), lexeme)!=all_lexemes.end())
          cout << "ERROR: This lexeme already exists.\n";
        else all_lexemes.push_back(lexeme);

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
        lexer_dfa.set_accepting(end_id, all_lexemes.size()-1);
        // Lambda transition from head node to start_id node.
        lexer_dfa.add_transition(head_id, start_id);
      }
      // Full line comments.
      else if (c=='#') {
        while (!in->eof() && c!='\n') in->get(c);
      }
      // Command.
      else if (c=='%') {
        pass_white_space();
        string command;
        in->get(c);
        // Get the command and check what it is.
        while (!in->eof() && isalpha(c)) {
          command.push_back(c);
          in->get(c);
        }
        // End parsing.
        if (command=="End") break; 
      }
      // Get next character.
      in->get(c);
    }
    // Reduce the NFA to a DFA.
    lexer_dfa = lexer_dfa.nfa_to_dfa();
  }

  bool LexerDFA::any_remaining() const {
    return lexer_dfa.any_remaining();
  }

  Token LexerDFA::get_token() {
    Token tok(-1, "");
    do {
      tok = lexer_dfa.get_token(); 
      // Check status
      if (check_status()!=0) return Token(-1, "");
      // Process token.
      int index = reserved_index(tok.literal);
      if (0<=index) {
        tok.type = index + all_lexemes.size();
        return tok;
      }
      else if (!is_skip(tok.type)) return tok;
      // If we get here, then the token is not a reserved word, and is a skipped lexeme type.
    } while (any_remaining());
    // If we got here, there was a problem.
    return Token(-1, "");
  }

  int LexerDFA::size() const {
    return lexer_dfa.size();
  }

  int LexerDFA::accepts(const string& word) const {
    return lexer_dfa.accepts(word);
  }

  int LexerDFA::accepts_empty() const {
    return lexer_dfa.accepts_empty();
  }

  int LexerDFA::check_status() const { 
    return lexer_dfa.check_status(); 
  }

  char LexerDFA::peek() const { 
    return lexer_dfa.peek(); 
  }

  string LexerDFA::lexeme_name(const int index) const {
    if (index<0 || all_lexemes.size()<=index) return "";
    return all_lexemes[index];
  }

  int LexerDFA::reserved_index(const string& word) const {
    auto it = std::find(reserved_tokens.begin(), reserved_tokens.end(), word);
    if (it==reserved_tokens.end()) return -1;
    else return std::distance(reserved_tokens.begin(), it);
  }

  int LexerDFA::add_reserved(const string& keyword) {
    // Make sure the keyword will be accepted by the lexer.
    if (!accepts(keyword)) return -1;
    // Check if the keyword already exists.
    auto it = std::find(reserved_tokens.begin(), reserved_tokens.end(), keyword);
    if (it==reserved_tokens.end()) {
      reserved_tokens.push_back(keyword);
      return reserved_tokens.size() - 1 + all_lexemes.size();
    }
    else return std::distance(reserved_tokens.begin(), it) + all_lexemes.size();
  }

  inline pair<int, int> LexerDFA::get_sequence(std::istream *in, char terminator) {
    int start_id = lexer_dfa.add_node();
    int end_id = lexer_dfa.add_node();
    int recent_id = start_id;
    pair<int, int> ends(start_id, end_id);
    // Lambda function that adds another node in the line.
    auto add_node = [&] (const char ci, const char cf) {
      int id = lexer_dfa.add_node();
      lexer_dfa.add_transition(recent_id, id, ci, cf);
      recent_id = id;
    };
    // Lambda function for adding a character node. Checks whether a *+? comes after the char.
    auto add_char = [&] (const char c) {
      char d = in->peek();
      int id = lexer_dfa.add_node();
      if (d=='+') {
        // Add backwards epsilon mode
        lexer_dfa.add_transition(id, recent_id);
      }
      else if (d=='*') {
        // Add bidirectional epsilon moves.
        lexer_dfa.add_transition(recent_id, id);
        lexer_dfa.add_transition(id, recent_id);
      }
      else if (d=='?') {
        // Add forwards epsilon move.
        lexer_dfa.add_transition(recent_id, id);
      }
      // Add the normal character transition.
      lexer_dfa.add_transition(recent_id, id, c);
      recent_id = id;
    };

    char c;
    in->get(c);
    while (!in->eof()) {
      // Terminator character.
      if (c==terminator) {
        // Link recent_id node with end_id node with a epsilon transition.
        lexer_dfa.add_transition(recent_id, end_id);
        return ends;
      }
      // Pass spaces and tabs
      else if (c==' ' || c== '\t');
      // Comment (full line)
      else if (c=='#') {
        while (!in->eof() && c!='\n') in->get(c);
      }

      // Start of grouping.
      else if (c=='(' || c=='[') {
        auto [start, end] = c=='(' ? get_sequence(in, ')') : get_sequence(in, ']');
        lexer_dfa.add_transition(recent_id, start); // Link up with a epsilon transition.
        // Peek if a modifiers modifies the grouping.
        in->get(c);
        if (!in->eof()) {
          if (c=='*') { // Kleene star
            // Add bidirectional epsilon transitions from the start node to the end node (so any number of times is an option).
            lexer_dfa.add_transition(start, end);
            lexer_dfa.add_transition(end, start);
            recent_id = end;
          }
          else if (c=='?') { // Zero or one
            // Add a epsilon transition from the start node to the end node (so 0 times is an option).
            lexer_dfa.add_transition(start, end);
          }
          else if (c=='+') { // One or more
            // Add a epsilon transition from the end node to the start node (so more than 1 time is an option).
            lexer_dfa.add_transition(end, start);
          }
          else in->putback(c);
        }
        recent_id = end;
      }

      // Escape character
      else if (c=='\\') {
        in->get(c); // Get next letter.
        if (in->eof()) {
          add_node('\\', '\\');
          continue;
        }
        // Any letter. Need to add two transitions.
        if (c=='@') {
          int id = lexer_dfa.add_node();
          lexer_dfa.add_transition(recent_id, id, 'a', 'z');
          lexer_dfa.add_transition(recent_id, id, 'A', 'Z');
          recent_id = id;
        }
        // Any lower case letter.
        else if (c=='a') add_node('a', 'z');
        // Any upper case letter.
        else if (c=='A') add_node('A', 'Z');
        // Any number
        else if (c=='d') add_node('0', '9');
        // Other escape characters.
        else {
          char d=0;
          if (c=='\\') d = '\\';
          else if (c=='n') d = '\n';
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
          //add_node(d, d);
          add_char(d);
        }
      }
      // OR
      else if (c=='|') {
        // Link recent_id node with end_id node via a epsilon transition, set up to get the next bin in the OR statement
        lexer_dfa.add_transition(recent_id, end_id);
        recent_id = start_id;
      }

      // Newline - we should only encounter this if terminator == '\n'
      else if (c=='\n') {
        cout << "ERROR: Encountered unexpected newline.\n";
        exit(0);
      }
      // General character. Could be the start of a char range.
      else {
        char d = in->peek();
        if (d=='-') { // Char range
          d = in->peek();
          if (d!=terminator) add_node(c, d);
          else { // Not actually a char range. Just a character and a '-' character.
            add_node(c, c);
            add_node('-', '-');
          }
        }
        else add_char(c); //add_node(c, c);
      }

      // Get next character
      in->get(c);
    }
    // In case of eof. 
    lexer_dfa.add_transition(recent_id, end_id); // For completeness.
    return ends;
  }

  inline bool LexerDFA::is_skip(int lexeme_id) {
    return std::find(skip_lexemes.begin(), skip_lexemes.end(), lexeme_id)!=skip_lexemes.end();
  }


}