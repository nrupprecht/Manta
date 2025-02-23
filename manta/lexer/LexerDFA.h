#pragma once

#include <exception>

#include "manta/lexer/DeterministicFiniteAutomata.h"
#include "manta/utility/IStreamContainer.h"

namespace manta {

class LexerDFA {
public:
  //! \brief Set up the lexer to parse a file. Takes the filename.
  bool SetFileToLex(const std::string& fileName);

  //! \brief Set up the lexer to parse a string. Takes the string.
  void SetStringToLex(const std::string& sentence);

  //! \brief Set the input stream container.
  void SetContainer(utility::IStreamContainer container);

  // =====================================================================================
  //  Helper functions.
  // =====================================================================================

  //! Returns whether there are any characters left to be processed.
  NO_DISCARD bool CheckAnyRemaining() const;

  //! \brief Get the next token from the stream.
  std::optional<LexerResult> LexNext();

  //! \brief Get the total number of lexemes the Lexer recognizes.
  NO_DISCARD std::size_t GetNumLexemes() const;

  //! \brief Parse all the input.
  std::vector<Token> LexAll();

  //! \brief Return the number of states in the underlying FiniteAutomaton.
  NO_DISCARD int Size() const;

  //! \brief Reset the status of the underlying FiniteAutomaton.
  void ResetStatus();

  //! \brief Check the status of the underlying FiniteAutomaton.
  NO_DISCARD FAStatus CheckStatus() const;

  //! \brief Return the name of a lexeme.
  NO_DISCARD std::string LexemeName(int index) const;

  //! \brief Get the lexeme ID of a lexeme name.
  NO_DISCARD std::optional<int> LexemeID(const std::string& name) const;

  //! \brief Get the line that the lexer is currently on.
  NO_DISCARD int GetLine() const;

  //! \brief Get the character column number in the line that the lexer is currently on.
  NO_DISCARD int GetColumn() const;

  //! \brief Get the source position of the lexer.
  NO_DISCARD SourcePosition GetSourcePosition() const;

  //! \brief Set the repeat eof flag.
  void SetRepeatEOF(bool flag);

  //! \brief Checks whether the lexer DFA is in a good state.
  bool IsGood() const;

private:
  // Lexer generator is a class so it can construct Lexers via the private constructor.
  friend class LexerGenerator;

  //! \brief Private constructor.
  LexerDFA(FiniteAutomaton dfa,
           std::vector<std::string> lexemes,
           std::map<std::string, int> reserved,
           std::set<int> skips)
      : all_lexemes_(std::move(lexemes))
      , reserved_tokens_(std::move(reserved))
      , skip_lexemes_(std::move(skips))
      , lexer_dfa_(std::move(dfa)) {};

  //! \brief Check if a token should be skipped.
  NO_DISCARD bool isSkip(int lexeme_id) const;

  //! \brief A vector of lexemes.
  std::vector<std::string> all_lexemes_ {};

  //! \brief Keep track of the reserved tokens.
  //!
  //! Maps from the reserved literal to its lexeme number.
  //!
  //! Each reserved token must be acceptable as some lexeme type.
  //! TODO(Nate): Check if this is still true, and if so, find a way to relax it.
  std::map<std::string, int> reserved_tokens_ {};

  //! \brief A list of lexemes to skip.
  std::set<int> skip_lexemes_ {};

  //! \brief The underlying deterministic finite automaton used to do the parsing.
  FiniteAutomaton lexer_dfa_ {};
};

//! \brief Lex everything the lexer points at, and return a string representation
//!     of the lex. Used in testing.
inline std::string LexAllToString(const std::shared_ptr<LexerDFA>& lexer) {
  std::stringstream stream;
  auto all_tokens = lexer->LexAll();
  for (const auto& token : all_tokens) {
    stream << "(" << lexer->LexemeName(token.type) << " | \"" << clean(token.literal) << "\") ";
  }
  return stream.str();
}

}  // namespace manta
