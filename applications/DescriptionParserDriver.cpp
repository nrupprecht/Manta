#include <fstream>

#include "manta/generator/parser/DescriptionParser.h"

#include <Lightning/Lightning.h>

#include "manta/generator/ParserGenerator.h"
#include "manta/generator/ParserCodegen.h"

using namespace lightning;

void InitializeLogging();

int main(int argc, char** argv) {
  // Set up the global core to an OstreamSink.
  Global::GetCore()->AddSink(NewSink<StdoutSink>());

  std::ifstream fin("../../config/full_rules_codegen.txt");
  std::ofstream fout("../../include/manta/generatedparsers/TestParser-2.h");

  manta::ParserCodegen parser_codegen;
  parser_codegen.SetDescriptionParser(std::make_shared<manta::generator::parser::ParsingDescriptionParser>());

  try {
    parser_codegen.GenerateParserCode(fout, fin, manta::ParserType::LALR);
  }
  catch (const std::exception& ex) {
    LOG_SEV(Fatal) << "Exception generating code: " << NewLineIndent << ex;
    return 0;
  }

  return 0;
}

void InitializeLogging() {
  // Set up the global core to an OstreamSink.
  Global::GetCore()->AddSink(NewSink<StdoutSink>());

  auto formatter = MakeMsgFormatter("[{}] [{}:{}] [{}] {}",
                                        formatting::DateTimeAttributeFormatter{},
                                        formatting::FileNameAttributeFormatter{},
                                        formatting::FileLineAttributeFormatter{},
                                        formatting::SeverityAttributeFormatter{},
                                        formatting::MSG);
  Global::GetCore()->SetAllFormatters(std::move(formatter));
}