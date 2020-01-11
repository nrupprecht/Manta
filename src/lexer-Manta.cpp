#include "lexer-Manta.hpp"

namespace Manta {

  Lexer::Lexer() {
    // Unused by default.
    for (int i=0; i<6; ++i) built_in_token[i] = -1;
    // Always use EOF.
    built_in_token[next_lexeme_id++] = 0;
  }
  
  Token Lexer::getNext() {
    char c;
    string acc;

    // Loop.
    while (!instream.eof()) {
      // Get the next charater.
      get(c);
      // Pass spaces.
      if (c==' ' || c=='\t');
      // Pass comments.
      else if (c=='#') {
        get(c);
        while (!instream.eof() && c!='\n') get(c);
      }
      // Look for newlines
      else if (c=='\n' || c=='\r') return Token(built_in_token[1], "\n");
      // Numbers
      else if (isdigit(c)) {
        // Keep getting digits for as long as possible.
        do {
          acc.push_back(c);
          get(c);
        } while (!instream.eof() && (isdigit(c) || c=='.'));
        // Return the last token.
        if (!instream.eof()) putback(c);
        // Return a number token.
        return Token(built_in_token[2], acc);
      }
      // Words
      else if (isalpha(c) || c=='_') {
        // Keep getting characters for as long as possible.
        do {
          acc.push_back(c);
          get(c);
        } while (!instream.eof() && (isalpha(c) || c=='_'));
        putback(c);

        auto key = reserved_words.find(acc);
        if (key!=reserved_words.end()) return Token(key->second, acc);
        else return Token(built_in_token[3], acc);
      }
      // Strings
      else if (c=='"') {
        get(c);
        while (!instream.eof() && c!='"') {
          acc.push_back(c);
          get(c);
        }
        // Return a @string token.
        return Token(built_in_token[5], acc);
      }
      // Operators.
      else if (!instream.eof()) {
        do {
          acc.push_back(c);
          get(c);
        } while (!instream.eof() && !isdigit(c) && !isspace(c) && !isalpha(c) && isOperatorPrefix(acc+c));
        // Return the last token.
        if (!instream.eof()) putback(c);
        // Returns an operator token.
        auto op = reserved_operators.find(acc);
        if (op!=reserved_operators.end()) return Token(op->second, acc);
        else return Token(built_in_token[4], acc);
      }
      // EOF
      else return Token(built_in_token[0], "@eof");

    }

    // Return EOF
    return Token(built_in_token[0], "@eof");
  }

  bool Lexer::openFile(const string& fileName) {
    // Close just in case
    instream.close();
    // Try to open the file
    instream.open(fileName);
    if (instream.fail()) return false;
    // Reset line and character.
    line = 0;
    character = 0;
    // Return success.
    return true;
  }

  int Lexer::getBuiltInType(int i) {
    const int built_in_types = 6;
    if (-1<i && i<built_in_types) {
      if (built_in_token[i]==-1) {
        built_in_token[i] = next_lexeme_id++;
        return built_in_token[i];
      }
      else return built_in_token[i];
    }
    else {
      cout << "ERROR: Asking for built in type " << i << " to be assigned an id.\n";
      cout << "This function thinks there are only " << built_in_types << " built in types.\n";
      return -1;
    }
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
    else if (i==built_in_token[5]) return "@string";
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

  bool Lexer::isOperatorPrefix(const string& opstring) {
    for (const auto & res : reserved_operators) {
      const string &op = res.first;
      if (op.size()<opstring.size()) continue;
      bool match = true;
      for (int i=0; i<opstring.size(); ++i)
        if (op[i]!=opstring[i]) match = false;
      if (match) return true;
    }
    return false;
  }

  void Lexer::get(char& c) {
    if (!instream.eof()) {
      instream.get(c);
      ++character;
      if (c=='\n') {
        ++line;
        character = 1;
      }
    }
    else cout << "EOF" << endl;
  }

  void Lexer::putback(char c) {
    instream.putback(c);
    --character;
  }

}