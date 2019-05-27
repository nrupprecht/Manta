#ifndef __LEXER_2_HPP__MANTA__
#define __LEXER_2_HPP__MANTA__

#include "utility.hpp"

namespace Manta {

  enum class LexType {  Keyword, Identifier, Number, NewLine, OpenParen, 
                        CloseParen, OpenBrack, CloseBrack, OpenSBrack, 
                        CloseSBrack, Langle, Rangle, Operator, Colon, 
                        Semicolon, Comma };

  struct Lexeme {
    Lexeme(LexType t, string l) : type(t), literal(l) {};

    LexType type;
    string literal;
  };

  class Lexer {
  public:
    //! Take an input program and lex it.
    void lex_string(const string&);

    //! \brief Clean an input string.
    void clean(const string&);

    //! \brief Lex the program string
    void lex();

    void addOperator(string);
    void addKeyword(string);

    const string& getProgram();

    const vector<Lexeme>& getLexemes();

  private:
    //! \brief All the operators.
    set<string> operators;
    //! \brief All the keyworks
    set<string> keywords;

    //! \brief A string used to hold the (cleaned) program.
    string program;

    //! \brief List of lexemes.
    vector<Lexeme> lexemes;
  };

  // LexType printing function.
  inline std::ostream& operator<<(std::ostream& out, LexType type) {
    switch (type) {
      case LexType::Keyword: {
        out << "Keyword";
        break;
      }
      case LexType::Identifier: {
        out << "Identifier";
        break;
      }
      case LexType::Number: {
        out << "Number";
        break;
      }
      case LexType::NewLine: {
        out << "NewLine";
        break;
      }
      case LexType::OpenParen: {
        out << "OpenParen";
        break;
      }
      case LexType::CloseParen: {
        out << "CloseParen";
        break;
      }
      case LexType::OpenBrack: {
        out << "OpenBrack";
        break;
      }
      case LexType::CloseBrack: {
        out << "CloseBrack";
        break;
      }
      case LexType::OpenSBrack: {
        out << "OpenSBrack";
        break;
      }
      case LexType::CloseSBrack: {
        out << "CloseSBrack";
        break;
      }
      case LexType::Rangle: {
        out << "Rangle";
        break;
      }
      case LexType::Langle: {
        out << "Langle";
        break;
      }
      case LexType::Operator: {
        out << "Operator";
        break;
      }
      case LexType::Colon: {
        out << "Colon";
        break;
      }
      case LexType::Semicolon: {
        out << "Semicolon";
        break;
      }
      case LexType::Comma: {
        out << "Comma";
        break;
      }
      default: {
        out << "Unknown";
        break;
      }
    }
    return out;
  }

}
#endif // __LEXER_2_HPP__MANTA__