#include "manta/lexer/LexerDFA.h"
// Other files.
#include <Lightning/Lightning.h>
#include <filesystem>

namespace manta {

bool LexerDFA::SetFileToLex(const std::string& file_name) {
  if (!std::filesystem::exists(file_name)) {
    return false;
  }
  auto instream = utility::IStreamContainer::OpenFile(file_name);
  lexer_dfa_.SetContainer(instream);
  return true;
}

void LexerDFA::SetStringToLex(const std::string& sentence) {
  auto instream = utility::IStreamContainer::StreamString(sentence);
  lexer_dfa_.SetContainer(instream);
}

void LexerDFA::SetContainer(utility::IStreamContainer container) {
  lexer_dfa_.SetContainer(container);
}

bool LexerDFA::CheckAnyRemaining() const {
  return lexer_dfa_.AnyRemaining();
}

std::optional<LexerResult> LexerDFA::LexNext() {
  std::optional<LexerResult> result;
  do {
    result = lexer_dfa_.LexNext();
    // Check status
    if ((CheckStatus() != FAStatus::Valid && CheckStatus() != FAStatus::AcceptedEOF)
        || !result /* Failed to get anything */) {
      return {};
    }
    // If all accepted states of the token (note that there are generally just one
    // accepted state) are skip, then skip the token.
    bool is_skip = false;
    for (auto& [lexeme_id, _] : result->accepted_lexemes) {
      if (isSkip(lexeme_id)) {
        is_skip = true;
        break;
      }
    }
    if (is_skip) {
      continue;
    }
    return result;
  } while (CheckAnyRemaining());
  // If we got here, there was a problem.
  return {};
}

std::size_t LexerDFA::GetNumLexemes() const {
  return all_lexemes_.size();
}

std::vector<Token> LexerDFA::LexAll() {
  // Only considers the highest precedence lexeme if there are multiple valid lexemes.
  std::vector<Token> output;
  auto result = LexNext();
  while (result) {
    // If it is a skip lexeme, skip it.
    if (!isSkip(result->accepted_lexemes[0].first)) {
      output.emplace_back(result->accepted_lexemes[0].first, result->literal, result->source_position);
    }
    // Get the next lexeme.
    result = LexNext();
  }
  return output;
}

int LexerDFA::Size() const {
  return lexer_dfa_.size();
}

void LexerDFA::ResetStatus() {
  lexer_dfa_.ResetStatus();
}

FAStatus LexerDFA::CheckStatus() const {
  return lexer_dfa_.CheckStatus();
}

std::string LexerDFA::LexemeName(int index) const {
  if (index < 0 || static_cast<int>(all_lexemes_.size()) <= index) {
    return "";
  }
  return all_lexemes_[index];
}

std::optional<int> LexerDFA::LexemeID(const std::string& name) const {
  const auto it = std::ranges::find(all_lexemes_, name);
  if (it == all_lexemes_.end()) {
    return {};
  }
  return std::distance(all_lexemes_.begin(), it);
}

int LexerDFA::GetLine() const {
  return lexer_dfa_.GetLine();
}

int LexerDFA::GetColumn() const {
  return lexer_dfa_.GetCharacter();
}

SourcePosition LexerDFA::GetSourcePosition() const {
  return lexer_dfa_.GetSourcePosition();
}

void LexerDFA::SetRepeatEOF(bool flag) {
  lexer_dfa_.SetRepeatEOF(flag);
}

bool LexerDFA::IsGood() const {
  return lexer_dfa_.IsGood();
}

bool LexerDFA::isSkip(int lexeme_id) const {
  return skip_lexemes_.contains(lexeme_id);
}

} // namespace manta