#include "LexerDFA.hpp"

namespace Manta {

    void LexerDFA::set_file_to_parse(const string& fileName) {
        std::ifstream fin(fileName);
        if (!fin.fail()) {
            auto instream = istream_container::open_file(fileName);
            lexer_dfa.set_stream(instream);
        }
    }

    void LexerDFA::set_string_to_lex(const string& sentence) {
        auto instream = istream_container::stream_string(sentence);
        lexer_dfa.set_stream(instream);
    }

    bool LexerDFA::any_remaining() const {
        return lexer_dfa.any_remaining();
    }

    Token LexerDFA::get_token() {
        Token tok;
        do {
            tok = lexer_dfa.get_token();
            // Check status
            if (check_status() != 0 && check_status() != 3) {
                return Token();
            }
            // Process token.
            int index = reserved_index(tok.literal);
            if (0 <= index) {
                tok.type = index + static_cast<int>(all_lexemes.size());
                return tok;
            }
            else if (!is_skip(tok.type)) {
                return tok;
            }
            // If we get here, then the token is not a reserved word, and is a skipped lexeme type.
        } while (any_remaining());
        // If we got here, there was a problem.
        return Token();
    }

    std::vector<Token> LexerDFA::lex_all() {
        std::vector<Token> output;
        Token tok = get_token();
        while (!tok.isNull()) {
            output.push_back(tok);
            tok = get_token();
        }
        return output;
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
        if (index < 0 || all_lexemes.size() <= index) {
            //throw InvalidIndex();
            return "";
        }
        return all_lexemes[index];
    }

    int LexerDFA::reserved_index(const string& word) const {
        auto it = std::find(reserved_tokens.begin(), reserved_tokens.end(), word);
        if (it == reserved_tokens.end()) {
            return -1;
        }
        else {
            return std::distance(reserved_tokens.begin(), it);
        }
    }

    inline bool LexerDFA::is_skip(int lexeme_id) {
        return std::find(skip_lexemes.begin(), skip_lexemes.end(), lexeme_id)!=skip_lexemes.end();
    }

}