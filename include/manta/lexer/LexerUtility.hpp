#pragma once

// http://smlweb.cpsc.ucalgary.ca/start.html
// https://web.cs.dal.ca/~sjackson/lalr1.html.......5/,

#include <deque>
#include <stack>

#include "manta/utility/Exceptions.h"
#include "manta/utility/utility.hpp"

namespace manta {

struct SourcePosition {
  SourcePosition() = default;

  SourcePosition(int l, int c) : line(l), column(c) {}

  std::string ToString() const { return lightning::formatting::Format("{}:{}", line, column); }

  void NewLine() {
    ++line;
    column = 1;
  }

  void Advance() { ++column; }

  //! \brief One-based line number.
  unsigned line = 0;
  //! \brief One-based column number.
  unsigned column = 0;
};

//! \brief Convert a SourcePosition to a string. This function will allow it to stream into lightning logging
//!        for free.
inline std::string to_string(const SourcePosition& pos) {
  return pos.ToString();
}

struct Token {
  Token() = default;

  Token(int type, std::string literal, const SourcePosition& source_position = {})
      : literal(std::move(literal))
      , type(type)
      , source_position(source_position) {}

  Token(int type, int state) : type(type), state(state) {}

  explicit Token(int state) : state(state) {}

  //! \brief Pretty print the literal.
  std::string ToString() {
    std::string out;
    for (char c : literal) {
      if (c == '\n')
        out += "\\n";
      else if (c == '\r')
        out += "\\r";
      else
        out.push_back(c);
    }
    return out;
  }

  NO_DISCARD bool IsNull() const { return type < 0; }

  //! \brief Literal value of a terminal.
  std::string literal;

  //! \brief The type of the terminal or production.
  int type = -1;

  //! \brief The state - for parsing only.
  int state = -1;

  //! \brief The source position of the token.
  SourcePosition source_position;
};

//! \brief Represents the result of lexing.
//!
//! Naively (and in the simple case), we expect
//! valid input to lex into exactly one lexeme. However, there can be practical reasons
//! why you might want to define two different lexemes that might accept the same string.
//! You might want separate lexemes for @variable and @function_name, even if they both
//! accept "mine". The lexer can inspect all the accepted lexemes and see which results in
//! a valid move, allowing both the lexer and parser to have more flexibility.
struct LexerResult {
  //! \brief The lexeme(s) that were accepted, listed as a lexeme id and lexeme name.
  //!
  //! Results are listed in order of precedence.
  std::vector<std::pair<int, std::string>> accepted_lexemes {};

  //! \brief The string literal that was lexed.
  std::string literal {};

  //! \brief The source position of the start of the token.
  SourcePosition source_position {};
};

}  // namespace manta
