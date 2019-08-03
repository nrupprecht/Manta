#ifndef __LEXER_HPP__MANTA__
#define __LEXER_HPP__MANTA__

// http://smlweb.cpsc.ucalgary.ca/start.html
// https://web.cs.dal.ca/~sjackson/lalr1.html

#include "utility.hpp"

#include <stack>
using std::stack;

#include <deque>
using std::deque;

namespace Manta {

  struct Token {
    //! \brief Constructor.
    Token(int ty, string lit) : type(ty), literal(lit) {};

    Token(int ty, int st) : type(ty), state(st) {};

    Token(int s) : state(s) {};

    //! \brief The type of the terminal or production.
    int type = -1;
    //! \brief Literal value of a terminal.
    string literal = "";

    //! \brief The state - for parsing only.
    int state = -1;
  };

  // Tokens IDs:
  //   0 -> End of file @eof.
  //   1 -> Newline @newline.
  //   2 -> Number @number.
  //   3 -> String (generic) @identifier.
  //   4 -> Operator (generic) @operator.
  //   5 <=  -> Reserved operators or keywords.
  class Lexer {
  public:
    //! \brief Default constructor.
    Lexer();

    //! \brief Get the next token from the instream
    Token getNext();

    //! \brief Attach the instream to a file. Returns whether opening the file succeeded.
    bool openFile(const string&);

    //! \brief Return the total number of lex tokens.
    int size() const { return next_lexeme_id; }

    //! \brief Signals that a built in type needs to be looked for.
    int getBuiltInType(int);

    //! \brief Add a reserved word. Return its id.
    int addKeyword(const string);
    //! \brief Add a reserved operator. Return its id.
    int addOperator(const string);

    //! \brief Get the id corresponding to a keyword.
    int getKeywordID(const string);
    //! \brief Get the id corresponding to an operator.
    int getOperatorID(const string);

    //! \brief Returns the number of lexeme IDs.
    int getNumberOfIDs() const;

    //! \brief Get the string 
    string getTokenLiteral(int);

    //! \brief Returns whether the string is a reserved word or operator.
    bool isReserved(const string&) const;

    //! \brief Is the instream at EOF.
    bool isEOF() const;

  private:

    bool isOperatorPrefix(const string&);

    //! \brief Input stream.
    std::ifstream instream;

    //! \brief Reserved keywords.
    map<string, int> reserved_words;
    //! \brief Reserved operators.
    map<string, int> reserved_operators;

    //! \brief Maps token numbers to their names. Used for printing.
    map<int, string> inverse_map;

    //! \brief The int values for the built in tokens. A -1 value means they are not needed.
    int built_in_token[5];

    //! \brief The number that should be assigned to the next lexeme.
    int next_lexeme_id = 0;
  };

}

#endif // __LEXER_HPP__MANTA__