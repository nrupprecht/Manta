#include "manta/lexer/LexerDFA.hpp"
#include "manta/generator/ParserGenerator.h"
#include "manta/parser/LALRParser.hpp"
#include "manta/visualization/Display.hpp"

#include <Lightning/Lightning.h>

using namespace manta;
using namespace lightning;

void compareParsers(const std::string &rulesFilepath) {
  ParserGenerator slr(ParserType::SLR);
  ParserGenerator lalr(ParserType::LALR);

  auto slr_parser = slr.CreateParserFromFile(rulesFilepath);
  auto lalr_parser = lalr.CreateParserFromFile(rulesFilepath);

  CompareParsers(*slr_parser, *lalr_parser);
}

void testParser(const std::string &rules_filepath, const std::string &code_filepath) {
  // Parser
  ParserGenerator generator(ParserType::LALR);
  LOG_SEV(Info) << "Parsing rules from \"" << rules_filepath << "\"";
  std::shared_ptr<LALRParser> parser;
  try {
    parser = generator.CreateParserFromFile(rules_filepath);
  }
  catch (const std::exception& ex) {
    LOG_SEV(Error) << "Exception parsing rules:" << lightning::NewLineIndent << ex.what();
    return;
  }

  auto parser_gen_trace = generator.GetParserGenerationTrace();
  if (!parser_gen_trace.empty()) {
    LOG_SEV(Info)
        << "\n--- Parser Generation Trace     --- \n" << parser_gen_trace
        << "\n--- End Parser Generation Trace ---\n";
  }
  else {
    LOG_SEV(Info) << "No parser generation trace.";
  }

  if (parser) {
    // Print out the transition table.
    std::cout << "\n";
    generator.WriteStates(std::cout);
    std::cout << "\n" << parser->PrintTable() << "\n\n";
    LOG_SEV(Info) << "Description parse successful.\n\n";
    std::shared_ptr<ParseNode> program;
    try {
      program = parser->ParserCodeFile(code_filepath);
    }
    catch (const std::exception& ex) {
      LOG_SEV(Error) << "Exception during parsing: " << ex.what();
    }
    if (program) {
      LOG_SEV(Info) << "Parse took " << parser->NumParseSteps() << " steps.";
      std::cout << Display::RenderParseTree(program);
    } else {
      LOG_SEV(Warning) << "FAILURE parsing file. Printing parse trace:\n\n";
      std::cout << "=============================================================\n\n";
      std::cout << parser->GetParseTrace() << "\n";
    }

  } else {
    LOG_SEV(Warning) << "Failure to create Parser.";
    std::cout << generator.GetParserGenerationTrace() << "\n";
  }

}

int main(int argc, char **argv) {
  lightning::Global::GetCore()->AddSink(std::make_shared<OstreamSink>());

  // compareParsers("../config/simpler_rules.txt");

  testParser("../../config/simple-rules.txt", "../../examples/test.txt");

  // testParser("../../config/op_prec.txt", "../../examples/code-ex.txt");

  // testParser("../../config/full_rules.txt", "../../examples/basic_parser_and_lexer.txt");

  // testParser("../../config/manta.txt", "../../examples/manta_code.txt");

  // testParser("../../config/test_newline_or.txt", "../../examples/test_newline_or.txt");

  // testParser("../../config/code_rules.txt", "../../examples/codefile.txt");

  // testParser("../../config/lalr_grammar.txt", "../../examples/lalr_example.txt");

  return 0;
}