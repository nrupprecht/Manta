#ifndef LEXER_HPP_MANTA_
#define LEXER_HPP_MANTA_

// http://smlweb.cpsc.ucalgary.ca/start.html
// https://web.cs.dal.ca/~sjackson/lalr1.html.......5/,

#include <stack>
#include <deque>

#include "utility.hpp"
#include "Exceptions.h"

namespace manta {

struct Token {
  //! \brief Constructor.
  Token(int ty, std::string lit)
      : type(ty), literal(std::move(lit)) {};
  Token(int ty, int st)
      : type(ty), state(st) {};
  Token(int s)
      : state(s) {};
  Token() = default;

  //! \brief Pretty print the literal.
  std::string ToString() {
    std::string out;
    for (char c : literal) {
      if (c == '\n') out += "\\n";
      else if (c == '\r') out += "\\r";
      else out.push_back(c);
    }
    return out;
  }

  NO_DISCARD bool IsNull() const {
    return type < 0;
  }

  //! \brief Literal value of a terminal.
  std::string literal;

  //! \brief The type of the terminal or production.
  int type = -1;

  //! \brief The state - for parsing only.
  int state = -1;
};

}

#endif // LEXER_HPP_MANTA_