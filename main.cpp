

#include "include/LexerGenerator.h"
#include "include/LexerDFA.hpp"

#include "ParserGenerator.h"
#include "LALRParser.hpp"

#include "Display.hpp"

using namespace manta;

void testParser(const string &rulesFilepath, const string &codeFilepath) {
  // Parser
  ParserGenerator generator;
  std::cout << "Parsing rules from \"" << rulesFilepath << "\"\n";
  auto parser = generator.CreateParserFromFile(rulesFilepath);
  if (parser) {
    // Print out the transition table.
    std::cout << "\n";
    generator.WriteStates(std::cout);
    std::cout << "\n" << parser->PrintTable() << "\n\n";
    std::cout << "Description parse successful.\n\n";
    auto program = parser->ParserCodeFile(codeFilepath);
    if (program) {
      std::cout << program->printTree() << endl;
      std::cout << "\n\n" << LALRParser::PrintAsMathematica(program) << "\n\n";
    } else {
      std::cout << "Failure. Printing parse trace:\n";
      std::cout << parser->GetParseTrace() << "\n";
    }

    std::cout << Display::RenderParseTree(program);

  } else {
    std::cout << "Failure." << endl;
  }

}

int main(int argc, char **argv) {

  // Test CC
  //testParser("../config/simple-rules.txt", "../config/code-ex.txt");
  testParser("../config/op_prec.txt", "../config/code-ex.txt");

  return 0;
}