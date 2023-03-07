//
// Created by Nathaniel Rupprecht on 2/1/21.
//

#pragma once

#include "manta/lexer/DeterministicFiniteAutomata.hpp"
#include "manta/utility/IStreamContainer.hpp"
#include "manta/utility/Exceptions.h"

namespace manta {

// Forward declaration
class LexerDFA;

class LexerGenerator {
 public:
  //! \brief Create a lexer generator. If eof_token is true, a lexeme named @eof with
  //! the pattern "\0" is added to the lexer.
  explicit LexerGenerator(bool eof_token = true);

  //! \brief Read the specification of a parser from a file to create that parser.
  std::shared_ptr<LexerDFA> CreateLexer(const std::string &filename, bool clear_old = true);

  //! \brief Read the specification of a parser from an istream.
  std::shared_ptr<LexerDFA> CreateLexer(std::istream &in, bool clear_old = true);

  //! \brief Create a lexer from the current NDFA.
  std::shared_ptr<LexerDFA> CreateLexer();

  //! \brief Check if a string would be accepted by the lexer.
  NO_DISCARD int Accepts(const std::string &word) const;

  //! \brief Return the name of a lexeme based on the id.
  NO_DISCARD const std::string &LexemeName(int index) const;

  //! \brief Returns the id of a lexeme based on the name.
  NO_DISCARD int LexemeID(const std::string &name) const;

  //! \brief Get the index of a string if it is a reserved word, and -1 if it isn't.
  NO_DISCARD int ReservedIndex(const std::string &keyword) const;

  //! \brief Add a reserved token. Returns the id for the reserved keyword.
  int AddReserved(const std::string &keyword, int precedence = 2);

  //! \brief Add an existing lexeme type to the skip tokens by its keyword.
  void AddSkip(const std::string &keyword);

  //! \brief Add a lexeme by regular expression.
  int AddLexeme(const std::string &lexeme, const std::string &regex, int precedence = 1);

  //! \brief Get the total number of lexemes.
  NO_DISCARD std::size_t GetNumLexemes() const;

  //! \brief Exception class for encountering an unexpected new line.
  EXCEPTION(UnexpectedNewLine, "Unexpected new line character.");

  //! \brief Exception class for when trying to access a lexeme with an invalid index.
  EXCEPTION(InvalidIndex, "Invalid index.");

 private:
  //! \brief Peek at the next character in the stream.
  NO_DISCARD char peek() const;

  NO_DISCARD std::string getLexeme() const;

  //! \brief Turn a literal into a regex pattern that will accept exactly this literal.
  static std::string escapeLiteral(const std::string &literal);

  //! \brief Recursively get a sequence of characters, constructing a NFA,
  //! until a terminator character is reached (or eof).
  std::pair<int, int> getSequence(char terminator, bool useTerminator = true);

  //! \brief Create an NFA for a character class. This can be either
  //!     1) A standard character class, like [a-Z]
  //!     2) A complement character class, like [^a-z]
  //!     3) A string complement, like [~HI]
  std::pair<int, int> getCharacterClass();

  //! \brief Create an NFA that accepts a character if it is not part of a specific string.
  std::pair<int, int> stringComplement();

  //! \brief Make a character class or complement character class.
  std::pair<int, int> characterClass(bool make_complement = false);

  //! \brief Add the start (any number of instances) modifier to an NFA.
  void makeStar(int idi, int idf, int &recent_id);

  //! \brief Add plus (at least one instance) modifier to an NFA.
  void makePlus(int idi, int idf, int &recent_id);

  //! \brief Add the optional (zero or one instances) modifier to an NFA.
  void makeQues(int idi, int idf, int &recent_id);

  //! \brief Add (if needed) a modifier to a sequence of nodes.
  void checkModifier(int idi, int idf, int &recent_id);

  //! \brief Lambda function that adds another node in the line.
  void addNode(char ci, char cf, int &recent_id);

  //! \brief Function for adding a character node. Checks whether a */+/? comes after the char.
  void addChar(char c, int &recent_id);

  //! \brief Handle an escaped character or character group, like \n for 'newline,' or \@ for 'any alphabetic character.'
  void specialCharacters(char c, int &recent_id);

  //! \brief Check if a token should be skipped.
  inline bool isSkip(int);

  //! \brief What the escaped character represented by '\' follow by c corresponds to.
  char escapedCharacter(char c);

  // ================================================
  //  Private members.
  // ================================================

  //! \brief An istream container that holds the data to be parsed.
  IStreamContainer in_;

  //! \brief The head ID of the NDFA.
  int head_id_;

  //! \brief A vector of all lexeme names.
  std::vector<std::string> all_lexemes_;

  //! \brief Keep track of the reserved tokens.
  //!
  //! Each reserved token must be acceptable as some lexeme type.
  std::vector<std::pair<std::string, int>> reserved_tokens_;

  //! \brief A list of lexemes to skip.
  std::vector<int> skip_lexemes_;

  //! \brief The underlying deterministic finite automaton used to do the parsing.
  FiniteAutomaton lexer_dfa_;
};

} // namespace manta
