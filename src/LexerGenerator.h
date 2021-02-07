//
// Created by Nathaniel Rupprecht on 2/1/21.
//

#ifndef MANTACLION_LEXERGENERATOR_H
#define MANTACLION_LEXERGENERATOR_H

#include "dfa.hpp"
#include "istream_container.hpp"
#include <exception>

namespace Manta {
    // Forward declaration
    class LexerDFA;

    class LexerGenerator {
    public:
        LexerGenerator();

        //! \brief Read the specification of a parser from a file to create that parser.
        std::shared_ptr<LexerDFA> create_lexer(const std::string &filename);

        //! \brief Read the specification of a parser from an istream.
        std::shared_ptr<LexerDFA> create_lexer(std::istream &in);

        //! \brief Create a lexer from the current NDFA.
        std::shared_ptr<LexerDFA> create_lexer();

        //! \brief Check if a string would be accepted by the lexer.
        int accepts(const string&) const;

        //! \brief Check the status of the underlying FiniteAutomaton.
        int check_status() const;

        //! \brief Peek at the next character in the stream.
        char peek() const;

        //! \brief Return the name of a lexeme.
        std::string lexeme_name(int) const;

        //! \brief Get the index of a string if it is a reserved word, and -1 if it isn't.
        int reserved_index(const std::string&) const;

        //! \brief Add a reserved token. Returns the id for the reserved keyword.
        int add_reserved(const std::string&, int precedence = 2);

        //! \brief Add a lexeme by regular expression.
        int add_lexeme(const std::string& lexeme, const std::string& regex, int precedence = 1);

        //! \brief Exception class for encountering an unexpected new line.
        struct UnexpectedNewLine;
        //! \brief Exception class for when trying to access a lexeme with an invalid index.
        struct InvalidIndex;

    private:

        istream_container in;

        //! \brief Recursively get a sequence of characters, constructing a NFA,
        //! until a terminator character is reached (or eof).
        inline std::pair<int, int> get_sequence(char terminator, bool useTerminator=true);


        void make_star(const int idi, const int idf, int& recent_id);

        void make_plus(const int idi, const int idf, int& recent_id);

        void make_ques(const int idi, const int idf, int& recent_id);

        // Add (if needed) a modifier to a sequence of nodes.
        void check_modifier(const int idi, const int idf, int& recent_id);

        // Lambda function that adds another node in the line.
        void add_node(const char ci, const char cf, int& recent_id);

        //! \brief Function for adding a character node. Checks whether a */+/? comes after the char.
        void add_char(const char c, int& recent_id);

        void specialCharacters(char c, int& recent_id);

        //! \brief Check if a token should be skipped.
        inline bool is_skip(int);

        //! \brief The head ID of the NDFA.
        int head_id;

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

    struct LexerGenerator::UnexpectedNewLine : public std::exception {
        const char* what() const noexcept override {
            return "Unexpected new line character.";
        }
    };

    struct LexerGenerator::InvalidIndex : public std::exception {
        const char* what() const noexcept override {
            return "Invalid index.";
        }
    };

}
#endif //MANTACLION_LEXERGENERATOR_H
