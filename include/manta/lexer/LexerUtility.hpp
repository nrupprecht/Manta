#pragma once

// http://smlweb.cpsc.ucalgary.ca/start.html
// https://web.cs.dal.ca/~sjackson/lalr1.html.......5/,

#include <stack>
#include <deque>

#include "manta/utility/utility.hpp"
#include "manta/utility/Exceptions.h"

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
    for (char c: literal) {
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

//! \brief Represents the result of lexing. Naively (and inn the simple case), we expect valid input to lex into
//! exactly one lexeme. However, there can be practical reasons why you might want to define two different lexemes
//! that might accept the same string. You might want separate lexemes for @variable and @function_name, even if
//! they both accept "mine". The lexer can inspect all the accepted lexemes and see which results in a valid move,
//! allowing both the lexer and parser to have more flexibility.
//!
struct LexResult {
  LexResult() = default;

  LexResult(std::vector<std::pair<int, std::string>> accepted, std::string lit)
      : accepted_lexemes(std::move(accepted)), literal(std::move(lit)) {}

  //! \brief The lexeme(s) that were accepted, listed as a lexeme id and lexeme name.
  //!
  //!     Results are listed in order of precedence.
  std::vector<std::pair<int, std::string>> accepted_lexemes;

  //! \brief The string literal that was lexed.
  std::string literal;
};

}
