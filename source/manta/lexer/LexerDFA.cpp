#include "manta/lexer/LexerDFA.hpp"

using namespace manta;

bool LexerDFA::SetFileToLex(const std::string &fileName) {
  std::ifstream fin(fileName);
  if (!fin.fail()) {
    auto instream = IStreamContainer::OpenFile(fileName);
    lexer_dfa_.SetStream(instream);
    return true;
  }
  return false;
}

void LexerDFA::SetStringToLex(const std::string &sentence) {
  auto instream = IStreamContainer::StreamString(sentence);
  lexer_dfa_.SetStream(instream);
}

bool LexerDFA::CheckAnyRemaining() const {
  return lexer_dfa_.AnyRemaining();
}

std::optional<LexResult> LexerDFA::LexNext() {
  std::optional<LexResult> result;
  do {
    result = lexer_dfa_.LexNext();
    // Check status
    if (CheckStatus() != FAStatus::Valid && CheckStatus() != FAStatus::AcceptedEOF || !result /* Failed to get anything */) {
      return {};
    }
    // If all accepted states of the token (note that there are generally just one accepted state) are skip, then skip the token.
    bool is_skip = false;
    for (auto& [lexeme_id, _]: result->accepted_lexemes) {
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
      output.emplace_back(result->accepted_lexemes[0].first, result->literal);
    }
    // Get the next lexeme.
    result = LexNext();
  }
  return output;
}

int LexerDFA::size() const {
  return lexer_dfa_.size();
}

void LexerDFA::ResetStatus() {
  lexer_dfa_.ResetStatus();
}

FAStatus LexerDFA::CheckStatus() const {
  return lexer_dfa_.CheckStatus();
}

std::string LexerDFA::LexemeName(int index) const {
  if (index < 0 || all_lexemes_.size() <= index) {
    //throw InvalidIndex();
    return "";
  }
  return all_lexemes_[index];
}

int LexerDFA::GetLine() const {
  return lexer_dfa_.GetLine();
}

int LexerDFA::GetColumn() const {
  return lexer_dfa_.GetCharacter();
}

void LexerDFA::SetRepeatEOF(bool flag) {
  lexer_dfa_.SetRepeatEOF(flag);
}

inline bool LexerDFA::isSkip(int lexeme_id) {
  return std::find(skip_lexemes_.begin(), skip_lexemes_.end(), lexeme_id) != skip_lexemes_.end();
}
