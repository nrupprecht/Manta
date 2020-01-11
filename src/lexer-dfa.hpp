#ifndef __LEXER_DFA_HPP__MANTA__
#define __LEXER_DFA_HPP__MANTA__

#include "dfa.hpp"

namespace Manta {

  class LexerDFA {
  public:
    //! \brief Close the filestream, if necessary.
    ~LexerDFA();

    //! \brief Set up the lexer to parse a file. Takes the filename.
    void parse_file(const string&);
    //! \brief Set up the lexer to parse a string. Takes the string.
    void parse_string(const string&);

    //! \brief Read the specification of a parser from a file to create that parser.
    void create_parser(string);

    void create_parser(std::istream*);

    //! Returns whether there are any characters left to be processed.
    bool any_remaining() const;

    //! \brief Get the next token from the stream.
    Token get_token();

    //! \brief Return the number of states in the underlying FiniteAutomaton.
    int size() const;

    //! \brief Check if a string would be accepted by the lexer.
    int accepts(const string&) const;

    //! \brief Returns the accepting state of node 0 in the FiniteAutomaton.
    int accepts_empty() const;

    //! \brief Check the status of the underlying FiniteAutomaton.
    int check_status() const;

    //! \brief Peek at the next character in the stream.
    char peek() const;

    //! \brief Return the name of a lexeme.
    string lexeme_name(const int) const;

    //! \brief Get the index of a string if it is a reserved word, and -1 if it isn't.
    int reserved_index(const string&) const;

    //! \brief Add a reserved token. Returns the id for the reserved keyword.
    int add_reserved(const string&);

  private:
    //! \brief A stringstream that can be used to lex strings.
    std::stringstream stream;
    //! \brief A filestream that can be used to lex files.
    std::ifstream fin;

    //! \brief Recursively get a sequence of characters, constructing a NFA, until a terminator character is reached (or eof).
    inline pair<int, int> get_sequence(std::istream*, char);

    //! \brief Check if a token should be skipped.
    inline bool is_skip(int);

    //! \brief A vector of lexemes.
    vector<string> all_lexemes;

    //! \brief Keep track of the reserved tokens.
    //!
    //! Each reserved token must be acceptable as some lexeme type.
    vector<string> reserved_tokens;

    //! \brief A list of lexemes to skip.
    vector<int> skip_lexemes;

    //! \brief The underlying deterministic finite automaton used to do the parsing.
    FiniteAutomaton lexer_dfa;
  };

}
#endif // __LEXER_DFA_HPP__MANTA__