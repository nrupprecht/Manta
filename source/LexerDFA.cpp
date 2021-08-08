#include "../include/LexerDFA.hpp"

using namespace manta;

bool LexerDFA::SetFileToLex(const string &fileName) {
  std::ifstream fin(fileName);
  if (!fin.fail()) {
    auto instream = IStreamContainer::OpenFile(fileName);
    lexer_dfa_.SetStream(instream);
    return true;
  }
  return false;
}

void LexerDFA::SetStringToLex(const string &sentence) {
  auto instream = IStreamContainer::StreamString(sentence);
  lexer_dfa_.SetStream(instream);
}

bool LexerDFA::CheckAnyRemaining() const {
  return lexer_dfa_.any_remaining();
}

Token LexerDFA::GetToken() {
  Token tok;
  do {
    tok = lexer_dfa_.GetToken();
    // Check status
    if (CheckStatus() != FAStatus::Valid && CheckStatus() != FAStatus::AcceptedEOF) {
      return Token();
    }
    // Process token.
    if (isSkip(tok.type)) {
      continue;
    }
    return tok;
  } while (CheckAnyRemaining());
  // If we got here, there was a problem.
  return Token();
}

std::size_t LexerDFA::GetNumLexemes() const {
  return all_lexemes_.size();
}

std::vector<Token> LexerDFA::LexAll() {
  std::vector<Token> output;
  Token tok = GetToken();
  while (!tok.IsNull()) {
    output.push_back(tok);
    tok = GetToken();
  }
  return output;
}

int LexerDFA::size() const {
  return lexer_dfa_.size();
}

int LexerDFA::Accepts(const string &word) const {
  return lexer_dfa_.Accepts(word);
}

int LexerDFA::AcceptsEmpty() const {
  return lexer_dfa_.accepts_empty();
}

FAStatus LexerDFA::CheckStatus() const {
  return lexer_dfa_.check_status();
}

string LexerDFA::LexemeName(const int index) const {
  if (index < 0 || all_lexemes_.size() <= index) {
    //throw InvalidIndex();
    return "";
  }
  return all_lexemes_[index];
}

int LexerDFA::ReservedIndex(const string &word) const {
  auto it = std::find_if(reserved_tokens_.begin(),
                         reserved_tokens_.end(),
                         [&word](const auto &pr) { return pr.first == word; });
  if (it == reserved_tokens_.end()) {
    return -1;
  } else {
    return it->second;
  }
}

int LexerDFA::GetLine() const {
  return lexer_dfa_.GetLine();
}

int LexerDFA::GetCharacter() const {
  return lexer_dfa_.GetCharacter();
}

void LexerDFA::SetRepeatEOF(bool flag) {
  lexer_dfa_.SetRepeatEOF(flag);
}

inline bool LexerDFA::isSkip(int lexeme_id) {
  return std::find(skip_lexemes_.begin(), skip_lexemes_.end(), lexeme_id) != skip_lexemes_.end();
}
