#ifndef __LEXER_HPP__MANTA__
#define __LEXER_HPP__MANTA__

// http://smlweb.cpsc.ucalgary.ca/start.html
// https://web.cs.dal.ca/~sjackson/lalr1.html.......5/,

#include <stack>
#include <deque>

#include "utility.hpp"

namespace Manta {

    struct Token {
        //! \brief Constructor.
        Token(int ty, string lit) : type(ty), literal(lit) {};

        Token(int ty, int st) : type(ty), state(st) {};

        Token(int s) : state(s) {};

        Token() = default;

        //! \brief Pretty print the literal.
        std::string to_string() {
            std::string out;
            for (char c : literal) {
                if (c == '\n') out += "\\n";
                else if (c == '\r') out += "\\r";
                else out.push_back(c);
            }
            return out;
        }

        bool isNull() { return type < 0; }

        //! \brief The type of the terminal or production.
        int type = -1;
        //! \brief Literal value of a terminal.
        string literal = "";

        //! \brief The state - for parsing only.
        int state = -1;
    };

    // Tokens IDs:
    //  @eof - End of file.
    //  @newline -> Newline.
    //  @number -> Number.
    //  @identifier -> String (generic).
    //  @operator -> Operator (generic).
    //  @string -> String, "..."
    class Lexer {
    public:
        //! \brief Default constructor.
        Lexer();

        Lexer(const Lexer& lexer);

        //! \brief Get the next token from the instream
        Token getNext();

        //! \brief Attach the instream to a file. Returns whether opening the file succeeded.
        bool openFile(const string&);

        //! \brief Return the total number of lex tokens.
        int size() const { return next_lexeme_id; }

        //! \brief Signals that a built in type needs to be looked for.
        int getBuiltInType(int);

        //! \brief Add a reserved word. Return its id.
        int addKeyword(const string&);
        //! \brief Add a reserved operator. Return its id.
        int addOperator(const string&);

        //! \brief Get the id corresponding to a keyword.
        int getKeywordID(const string&);
        //! \brief Get the id corresponding to an operator.
        int getOperatorID(const string&);

        //! \brief Returns the number of lexeme IDs.
        int getNumberOfIDs() const;

        //! \brief Get the string
        string getTokenLiteral(int);

        //! \brief Returns whether the string is a reserved word or operator.
        bool isReserved(const string&) const;

        //! \brief Is the instream at EOF.
        bool isEOF() const;

        int getLine() const { return line; }

        int getCharacter() const { return character; }

    private:
        //! \brief Is this string the prefix to any reserved operator?
        bool isOperatorPrefix(const string&);

        //! \brief Get a character from the instream, keeping track of the line and character pointers.
        void get(char&);

        //! \brief Put back a character into instream, keeping track of the line and character pointers.
        void putback(char c);

        //! \brief Keep track of what line and character (in the line) of the file we are in.
        int line = 1, character = 1;

        //! \brief Input stream.
        std::ifstream instream;

        //! \brief Reserved keywords.
        std::map<string, int> reserved_words;

        //! \brief Reserved operators.
        std::map<string, int> reserved_operators;

        //! \brief Maps token numbers to their names. Used for printing.
        std::map<int, string> inverse_map;

        //! \brief The int values for the built in tokens. A -1 value means they are not needed.
        int built_in_token[6]{};

        //! \brief The number that should be assigned to the next lexeme.
        int next_lexeme_id = 0;
    };

}

#endif // __LEXER_HPP__MANTA__