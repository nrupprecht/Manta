#include "lexer.hpp"

#include "compiler-compiler.hpp"

#ifdef CPARSE
#include "cparse.hpp"
#endif // CPARSE

using namespace Manta;

int main(int argc, char** argv) {

  // Test CC

  if (true) {
    CompilerCompiler cc;
    cc.readFile("src/lexing-rules.txt");
    cc.writeFile("src/", "cparse");
  }

  //string input = "if true then\n";

  std::ifstream fin("src/test-program.txt");
  string input;
  if (fin.fail()) {
    cout << "File failed to open.\n";
  }
  else {
    char c;
    fin.get(c);
    while (!fin.eof()) {
      input.push_back(c);
      fin.get(c);
    }
  }
  fin.close();

  Lexer lexer;
  lexer.addKeyword("for");
  lexer.addKeyword("while");
  lexer.addKeyword("if");
  lexer.addKeyword("else");
  // Datatypes
  lexer.addKeyword("int");
  lexer.addKeyword("float");
  lexer.addKeyword("double");
  lexer.addKeyword("list");
  lexer.addKeyword("set");
  lexer.addKeyword("array");
  // Operators
  lexer.addOperator("+");
  lexer.addOperator("+=");
  lexer.addOperator("++");
  lexer.addOperator("-");
  lexer.addOperator("-=");
  lexer.addOperator("--");
  lexer.addOperator("*");
  lexer.addOperator("*=");
  lexer.addOperator("/");
  lexer.addOperator("/=");
  lexer.addOperator("=");
  lexer.addOperator("==");
  lexer.addOperator("%");
  lexer.addOperator("/");
  lexer.addOperator("...");
  // Functions
  lexer.addKeyword("map");

  // Lex the program string.
  lexer.lex_string(input);
  auto tokens = lexer.getLexemes();

  if (true) {
    cout << "Input: " << input << endl;
    cout << "\n-------\n";
    cout << "Progr: " << lexer.getProgram() << endl;
    cout << "\n-------\n";
    for (auto lx : tokens) cout << "\"" << lx.type << "\" - " << lx.literal << endl;
  }

  #ifdef CPARSE
  cout << "Ready to test Parser.\n\n";
  // Test the parser
  CC_Parser parser;
  bool success = parser.parse(tokens);
  cout << "Compile success: " << success << endl << endl;
  #endif // CPARSE


  return 0;
}