#ifndef __LEXER_DFA_HPP__MANTA__
#define __LEXER_DFA_HPP__MANTA__

#include "dfa.hpp"
#include "istream_container.hpp"
#include <exception>

namespace Manta {

    class LexerDFA {
    public:
        //! \brief Set up the lexer to parse a file. Takes the filename.
        void set_file_to_parse(const string&);

        //! \brief Set up the lexer to parse a string. Takes the string.
        void set_string_to_parse(const string&);

        // --- Helper functions.

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
        std::string lexeme_name(int) const;

        //! \brief Get the index of a string if it is a reserved word, and -1 if it isn't.
        int reserved_index(const std::string&) const;

        // Lexer generator is a class so
        friend class LexerGenerator;

    private:

        //! \brief Private constructor.
        LexerDFA(FiniteAutomaton& dfa,
                 std::vector<std::string> lexemes,
                 std::vector<std::string> reserved,
                 std::vector<int> skips)
            : lexer_dfa(dfa), all_lexemes(std::move(lexemes)), skip_lexemes(std::move(skips)), reserved_tokens(std::move(reserved)) {};


        //! \brief Check if a token should be skipped.
        inline bool is_skip(int);

        //! \brief A vector of lexemes.
        std::vector<std::string> all_lexemes;

        //! \brief Keep track of the reserved tokens.
        //!
        //! Each reserved token must be acceptable as some lexeme type.
        std::vector<std::string> reserved_tokens;

        //! \brief A list of lexemes to skip.
        std::vector<int> skip_lexemes;

        //! \brief The underlying deterministic finite automaton used to do the parsing.
        FiniteAutomaton lexer_dfa;
    };

}
#endif // __LEXER_DFA_HPP__MANTA__