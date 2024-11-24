#include <filesystem>

#include <Lightning/Lightning.h>

#include "manta/generator/ParserGenerator.h"
#include "manta/lexer/LexerDFA.hpp"
#include "manta/parser/LALRParser.hpp"
#include "manta/visualization/Display.hpp"
#include "manta/utility/Timer.h"

using namespace manta;
using namespace lightning;

//! \brief Function that does some comparisons between parsers, looking at how their parse tables differ.
void compareParsers(const std::string& rules_filepath) {
  ParserGenerator slr(ParserType::SLR);
  ParserGenerator lalr(ParserType::LALR);

  const auto slr_parser = slr.CreateParserFromFile(rules_filepath);
  const auto lalr_parser = lalr.CreateParserFromFile(rules_filepath);

  CompareParsers(*slr_parser, *lalr_parser);
}

void testParser(const std::string& rules_filepath, const std::string& code_filepath) {
  Logger logger;

  std::filesystem::create_directory("logs");

  std::string file_name = "logs/ParserTest.log";
  logger.GetCore()->AddSink(NewSink<StdoutSink>()).AddSink(NewSink<FileSink>(file_name));

  // Generate a LALR parser from the grammar description.
  ParserGenerator generator(ParserType::LALR);
  LOG_SEV(Info) << "Parsing rules from \"" << rules_filepath << "\"";
  std::shared_ptr<LALRParser> parser;
  try {
    parser = generator.CreateParserFromFile(rules_filepath);
  }
  catch (const std::exception& ex) {
    LOG_SEV(Fatal) << "Exception parsing rules:" << NewLineIndent << ex.what();
    return;
  }


  if (parser) {
    // Set the logger in the parser.
    parser->SetLogger(logger);

    std::ofstream fout("logs/ParserTable.txt");

    // Print out the transition table.
    std::cout << "\n";
    generator.WriteStates(std::cout);
    std::cout << "\n" << parser->PrintTable() << "\n\n";

    generator.WriteStates(fout);
    fout << "\n" << parser->PrintTable() << "\n\n";
    fout.close();

    LOG_SEV(Info) << "Description parse successful.\n\n";
    std::shared_ptr<ParseNode> program;
    try {
      utility::Timer timer{};
      program = parser->ParseCodeFile(code_filepath);
      timer.Stop();
      LOG_SEV(Info) << "Parse took " << timer.Time() << " seconds.";

    } catch (const std::exception& ex) {
      LOG_SEV(Error) << "Exception during parsing: " << ex.what();
    }
    if (program) {
      LOG_SEV(Info) << "Parse took " << parser->NumParseSteps() << " steps.";
      // Print out a very pretty ascii tree.
      std::cout << Display::RenderParseTree(program);
    }
    else {
      LOG_SEV(Warning) << "FAILURE parsing file.";
      std::cout << "=============================================================\n\n";
    }
  }
  else {
    LOG_SEV(Warning) << "Failure to create Parser.";
  }
}

int main(int argc, char** argv) {
  Global::GetCore()->AddSink(NewSink<StdoutSink>());

  // testParser("../../examples/C-/C- grammar.txt", "../../examples/C-/C- code.cmm");
  testParser("../../config/code_rules.txt", "../../examples/codefile.txt");

  return 0;
}