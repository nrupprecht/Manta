#include "lexer-Manta.hpp"

namespace Manta {

  Lexer::Lexer() {
    // Unused by default.
    for (int i=0; i<5; ++i) built_in_token[i] = -1;
  }
  
  Token Lexer::getNext() {
    char c;
    string acc;

    // Loop.
    while (!instream.eof()) {
      // Get the next charater.
      instream.get(c);
      // Pass spaces.
      if (c==' ' || c=='\t');
      // Look for newlines
      else if (c=='\n' || c=='\r') return Token(built_in_token[1], "\n");
      // Numbers
      else if (isdigit(c)) {
        // Keep getting digits for as long as possible.
        do {
          acc.push_back(c);
          instream.get(c);
        } while (!instream.eof() && (isdigit(c) || c=='.'));
        // Return a number token.
        return Token(built_in_token[2], acc);
      }
      // Words
      else if (isalpha(c) || c=='_') {
        // Keep getting characters for as long as possible.
        do {
          acc.push_back(c);
          instream.get(c);
        } while (!instream.eof() && (isalpha(c) || c=='_'));

        auto key = reserved_words.find(acc);
        if (key!=reserved_words.end()) return Token(key->second, acc);
        else return Token(built_in_token[3], acc);
      }
      // Operators.
      else {
        do {
          acc.push_back(c);
          instream.get(c);
        } while (!instream.eof() && !isdigit(c) && !isspace(c) && !isalpha(c));
        // Returns an operator token.
        auto op = reserved_operators.find(acc);
        if (op!=reserved_operators.end()) return Token(op->second, acc);
        else return Token(built_in_token[4], acc);
      }
    }

    return Token(built_in_token[0], "");
  }

  bool Lexer::openFile(const string& fileName) {
    // Close just in case
    instream.close();
    // Try to open the file
    instream.open(fileName);
    if (instream.fail()) return false;
    else return true;
  }

  int Lexer::getBuiltInType(int i) {
    if (-1<i && i<5) {
      if (built_in_token[i]==-1) {
        built_in_token[i] = next_lexeme_id++;
        return built_in_token[i];
      }
      else return built_in_token[i];
    }
    return -1;
  }

  int Lexer::addKeyword(const string word) {
    // Only add if it is not already there.
    auto it = reserved_words.find(word);
    if (it==reserved_words.end()) {
      reserved_words.insert(pair<string, int>(word, next_lexeme_id));
      inverse_map.insert(pair<int, string>(next_lexeme_id, word));
      ++next_lexeme_id;
      return next_lexeme_id-1;
    }
    else return it->second;
  }

  int Lexer::addOperator(const string word) {
    // Only add if it is not already there.
    auto it = reserved_operators.find(word);
    if (it==reserved_operators.end()) {
      reserved_operators.insert(pair<string, int>(word, next_lexeme_id));
      inverse_map.insert(pair<int, string>(next_lexeme_id, word));
      ++next_lexeme_id;
      return next_lexeme_id-1;
    }
    else return it->second;
  }

  int Lexer::getKeywordID(const string word) {
    auto it = reserved_words.find(word);
    if (it==reserved_words.end()) return 3;
    else return it->second;
  }

  int Lexer::getOperatorID(const string op) {
    auto it = reserved_operators.find(op);
    if (it==reserved_operators.end()) return 4;
    else return it->second;
  }

  int Lexer::getNumberOfIDs() const {
    return next_lexeme_id;
  }

  string Lexer::getTokenLiteral(int i) {
    if (i<0) return "";
    else if (i==built_in_token[0]) return "@eof";
    else if (i==built_in_token[1]) return "@newline";
    else if (i==built_in_token[2]) return "@number";
    else if (i==built_in_token[3]) return "@identifier";
    else if (i==built_in_token[4]) return "@operator";
    else {
      auto it = inverse_map.find(i);
      if (it!=inverse_map.end()) return it->second;
      else return "[ERROR]";
    }
  }

  bool Lexer::isReserved(const string& test) const {
    // Check maps.
    if (reserved_operators.find(test)!=reserved_operators.end()) return true;
    if (reserved_words.find(test)!=reserved_words.end()) return true;
    // Couldn't find it.
    return false;
  }

  bool Lexer::isEOF() const {
    return instream.eof();
  }

}