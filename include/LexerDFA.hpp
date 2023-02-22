#ifndef LEXER_DFA_HPP_MANTA_
#define LEXER_DFA_HPP_MANTA_

#include "DeterministicFiniteAutomata.hpp"
#include "IStreamContainer.hpp"
#include <exception>

namespace manta {


class LexerDFA {
 public:
  //! \brief Set up the lexer to parse a file. Takes the filename.
  bool SetFileToLex(const string &fileName);

  //! \brief Set up the lexer to parse a string. Takes the string.
  void SetStringToLex(const string &sentence);

  // --- Helper functions.

  //! Returns whether there are any characters left to be processed.
  NO_DISCARD bool CheckAnyRemaining() const;

  //! \brief Get the next token from the stream.
  std::optional<LexResult> LexNext();

  //! \brief Get the total number of lexemes the Lexer recognizes.
  NO_DISCARD std::size_t GetNumLexemes() const;

  //! \brief Parse all the input.
  std::vector<Token> LexAll();

  //! \brief Return the number of states in the underlying FiniteAutomaton.
  NO_DISCARD int size() const;

  //! \brief Reset the status of the underlying FiniteAutomaton.
  void ResetStatus();

  //! \brief Check the status of the underlying FiniteAutomaton.
  NO_DISCARD FAStatus CheckStatus() const;

  //! \brief Return the name of a lexeme.
  NO_DISCARD std::string LexemeName(int index) const;

  //! \brief Get the line that the lexer is currently on.
  NO_DISCARD int GetLine() const;

  //! \brief Get the character column number in the line that the lexer is currently on.
  NO_DISCARD int GetColumn() const;

  //! \brief Set the repeat eof flag.
  void SetRepeatEOF(bool flag);

 private:
  // Lexer generator is a class so it can construct Lexers via the private constructor.
  friend class LexerGenerator;

  //! \brief Private constructor.
  LexerDFA(FiniteAutomaton &dfa,
           std::vector<std::string> lexemes,
           std::vector<std::pair<std::string, int>> reserved,
           std::vector<int> skips) :
      lexer_dfa_(dfa),
      all_lexemes_(std::move(lexemes)),
      skip_lexemes_(std::move(skips)),
      reserved_tokens_(std::move(reserved)) {};

  //! \brief Check if a token should be skipped.
  inline bool isSkip(int lexeme_id);

  //! \brief A vector of lexemes.
  std::vector<std::string> all_lexemes_;

  //! \brief Keep track of the reserved tokens.
  //!
  //! Maps from the reserved literal to its lexeme number.
  //! Each reserved token must be acceptable as some lexeme type.
  std::vector<std::pair<std::string, int>> reserved_tokens_;

  //! \brief A list of lexemes to skip.
  std::vector<int> skip_lexemes_;

  //! \brief The underlying deterministic finite automaton used to do the parsing.
  FiniteAutomaton lexer_dfa_;
};

//! \brief Lex everything the lexer points at, and return a string representation
//!     of the lex. Used in testing.
inline std::string lexAllToString(const std::shared_ptr<LexerDFA> &lexer) {
  std::stringstream stream;
  auto all_tokens = lexer->LexAll();
  for (const auto &token: all_tokens) {
    stream << "(" << lexer->LexemeName(token.type) << " | \"" << clean(token.literal) << "\") ";
  }
  return stream.str();
}

} // manta
#endif // LEXER_DFA_HPP_MANTA_