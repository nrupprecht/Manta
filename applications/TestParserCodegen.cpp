#include "manta/lexer/LexerDFA.hpp"
#include "manta/generator/ParserCodegen.h"
#include "manta/parser/LALRParser.hpp"
#include "manta/visualization/Display.hpp"

using namespace manta;

int main(int argc, char **argv) {

  ParserCodegen generator;

  std::ofstream fout("../../include/manta/generatedparsers/TestParser.h");
  if (fout.fail()) {
    std::cout << "Error opening file to write parser code.";
    return 0;
  }

  std::ifstream fin("../../config/full_rules_codegen.txt");
  if (fin.fail()) {
    std::cout << "Error opening file to read parser description.";
    return 0;
  }

  generator.GenerateParserCode(fout, fin, ParserType::LALR);

  return 0;
}