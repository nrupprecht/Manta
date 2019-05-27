#include "lexer.hpp"

namespace Manta {

  void Lexer::lex_string(const string& input) {
    clean(input);
    lex();
  }

  void Lexer::clean(const string& input) {
    // Clear the program string
    program.clear();

    // True if we have encountered a (non-newline) whitespace.
    bool whitespace = false;
    // True if we have encountered a new-line.
    bool newline = false;

    bool slcomment = false;
    int mlcomment = 0;

    for (int i=0; i<input.size(); ++i) {
      // Get the next character.
      char c = input[i];

      // Look for comments starting or ending
      if (c=='/' && i+1 < input.size()) {
        char p = input[i+1];
        if (p=='/') {
          ++i;
          slcomment = true;
        }
        else if (p=='*' && !slcomment) {
          ++i;
          ++mlcomment;
        }
      }
      else if (c=='*' && i+1<input.size() && input[i+1]=='/') {
        ++i;
        --mlcomment;
        continue;
      }
      // Pass while commented
      if (slcomment && c!='\n') continue;
      if (slcomment && c=='\n') {
        program.push_back('\n');
        slcomment = false;
        continue;
      }
      else if (0<mlcomment) continue;

      // Whitespace type characters.
      if (c==' ' || c=='\t') {
        // Turn all (non-newline) whitespaces into a single space.
        if (whitespace);
        else program.push_back(' '); 
        // Update flags
        whitespace = true;       
        newline = false;
      }
      // Newline type characters.
      else if (c=='\n' || c=='\r') {
        // Keep a newline
        if (newline);
        else program.push_back('\n');
        // Update flags
        whitespace = false;
        newline = true;
      }
      // Otherwise store the character.
      else {
        program.push_back(c);
        whitespace = false;
        newline = false;
      }
    }
    // Add terminator
    program.push_back(' ');
  }

  void Lexer::lex() {

    string acc = "";
    int size = program.size();

    for (int i=0; i<size; ++i) {
      char c = program[i];

      // Whitespace terminates token
      if (c==' ');
      // Newline
      else if (c=='\n') {
        lexemes.push_back(Lexeme(LexType::NewLine, "\\n"));
      }
      // Variable
      else if (isalpha(c)) {
        // Get a variable
        acc.push_back(c);
        ++i;
        while (i<size) {
          // Get next character
          c = program[i];
          // Check character type
          if (isalpha(c) || isdigit(c) || c=='_') acc.push_back(c);
          else {
            --i;
            break;
          }
          // Increment
          ++i;
        }
        // Check if identifier is a recognized token.
        if (contains(keywords, acc)) lexemes.push_back(Lexeme(LexType::Keyword, acc));
        else lexemes.push_back(Lexeme(LexType::Identifier, acc));
        // Clear string
        acc.clear();
      }
      // Number
      else if (isdigit(c)) {
        // Get a number.
        acc.push_back(c);
        ++i;
        int decimals = 0;
        while (i<size) {
          //Get next character
          c = program[i];
          if (isdigit(c)) acc.push_back(c);
          else if (c=='.') {
            if (decimals==0)
              acc.push_back(c);
            else throw false;
            ++decimals;
          }
          else {
            --i;
            break;
          }
          // Increment
          ++i;
        }
        lexemes.push_back(Lexeme(LexType::Number, acc));
        acc.clear();
      }
      // Open paren
      else if (c=='(') lexemes.push_back(Lexeme(LexType::OpenParen, "("));
      // Close paren
      else if (c==')') lexemes.push_back(Lexeme(LexType::CloseParen, ")"));
      // Open bracket
      else if (c=='{') lexemes.push_back(Lexeme(LexType::OpenBrack, "{"));
      // Close bracket
      else if (c=='}') lexemes.push_back(Lexeme(LexType::CloseBrack, "}"));
      // Open square bracket
      else if (c=='[') lexemes.push_back(Lexeme(LexType::OpenSBrack, "["));
      // Close square bracket
      else if (c==']') lexemes.push_back(Lexeme(LexType::CloseSBrack, "]"));
      // Langle
      else if (c=='<') lexemes.push_back(Lexeme(LexType::Langle, "<"));
      // Rangle
      else if (c=='>') lexemes.push_back(Lexeme(LexType::Rangle, ">"));
      // Colon
      else if (c==':') lexemes.push_back(Lexeme(LexType::Colon, ":"));
      // Semicolon
      else if (c==';') lexemes.push_back(Lexeme(LexType::Semicolon, ";"));
      // Comma
      else if (c==',') lexemes.push_back(Lexeme(LexType::Comma, ","));
      // Operators
      else {
        string sym;
        sym.push_back(c);
        ++i;
        while(i<program.size()) {
          // Append a symbol.
          sym.push_back(program[i]);
          // If this is not a valid operator, break.
          if (!prepends(operators, sym)) break;
        }
        sym.pop_back();
        // Check if this is a valid operator
        if (contains(operators, sym)) lexemes.push_back(Lexeme(LexType::Operator, sym));
        else {
          cout << "Unrecognized token, [" << sym << "]" << endl;
        }
      }
    }
    // End of loop.
  }

  void Lexer::addOperator(string op) {
    operators.insert(op);
  }

  void Lexer::addKeyword(string key) {
    keywords.insert(key);
  }

  const string& Lexer::getProgram() {
    return program;
  }

  const vector<Lexeme>& Lexer::getLexemes() {
    return lexemes;    
  }

}