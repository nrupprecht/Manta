
#include "LexerGenerator.h"
#include "LexerDFA.hpp"

#include "ParserGenerator.h"
#include "LALRParser.hpp"

#include "Display.hpp"

using namespace manta;

void compareParsers(const string &rulesFilepath) {
  ParserGenerator slr(ParserType::SLR);
  ParserGenerator lalr(ParserType::LALR);

  auto slr_parser = slr.CreateParserFromFile(rulesFilepath);
  auto lalr_parser = lalr.CreateParserFromFile(rulesFilepath);

  CompareParsers(*slr_parser, *lalr_parser);
}

void testParser(const string &rulesFilepath, const string &codeFilepath) {
  // Parser
  ParserGenerator generator;
  std::cout << "Parsing rules from \"" << rulesFilepath << "\"\n";
  std::shared_ptr<LALRParser> parser;
  try {
    parser = generator.CreateParserFromFile(rulesFilepath);
  }
  catch (const std::exception& ex) {
    std::cout << "Exception parsing rules: " << ex.what();
    return;
  }

  auto parser_gen_trace = generator.GetParserGenerationTrace();
  if (!parser_gen_trace.empty()) {
    std::cout
        << "\n--- Parser Generation Trace     --- \n" << parser_gen_trace
        << "\n--- End Parser Generation Trace ---\n";
  }
  else {
    std::cout << "\nNo parser generation trace.\n";
  }

  if (parser) {
    // Print out the transition table.
    std::cout << "\n";
    generator.WriteStates(std::cout);
    std::cout << "\n" << parser->PrintTable() << "\n\n";
    std::cout << "Description parse successful.\n\n";
    std::shared_ptr<ParseNode> program;
    try {
      program = parser->ParserCodeFile(codeFilepath);
    }
    catch (const std::exception& ex) {
      std::cout << "Exception during parsing: " << ex.what() << std::endl;
    }
    if (program) {
      std::cout << "Parse took " << parser->NumParseSteps() << " steps.\n";
      std::cout << Display::RenderParseTree(program);
    } else {
      std::cout << "FAILURE parsing file. Printing parse trace:\n\n";
      std::cout << "=============================================================\n\n";
      std::cout << parser->GetParseTrace() << "\n";
    }

  } else {
    std::cout << "Failure to create Parser." << "\n";
    std::cout << generator.GetParserGenerationTrace() << "\n";
  }

}

int main(int argc, char **argv) {
  // testParser("../config/simple-rules.txt", "../examples/code-ex.txt");
  // testParser("../config/config_rules.txt", "../examples/example_config.txt");
  // testParser("../config/op_prec.txt", "../examples/code-ex.txt");
  testParser("../config/full_rules.txt", "../examples/basic_parser_and_lexer.txt");

  return 0;
}