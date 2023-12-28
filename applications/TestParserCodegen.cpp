#include "manta/generator/ParserCodegen.h"
#include <Lightning/Lightning.h>

using namespace manta;
using namespace lightning;
using namespace lightning::time;

int main(int argc, char** argv) {
  // Set up the global core to an OstreamSink.
  lightning::Global::GetCore()->AddSink(NewSink<lightning::UnlockedSink, lightning::OstreamSink>());

  auto formatter = std::make_unique<formatting::FormatterBySeverity>();
  auto default_fmt = MakeMsgFormatter("[{}] [{}] {}",
                                      formatting::DateTimeAttributeFormatter{},
                                      formatting::SeverityAttributeFormatter{},
                                      formatting::MSG);
  auto low_level_fmt = MakeMsgFormatter("[{}] [{}:{}] [{}] {}",
                                        formatting::DateTimeAttributeFormatter{},
                                        formatting::FileNameAttributeFormatter{false},
                                        formatting::FileLineAttributeFormatter{},
                                        formatting::SeverityAttributeFormatter{},
                                        formatting::MSG);
  formatter->SetDefaultFormatter(std::move(default_fmt));
  for (auto severity: {Severity::Trace, Severity::Debug}) {
    formatter->SetFormatterForSeverity(severity, low_level_fmt->Copy());
  }
  auto as_base = formatter->Copy();
  lightning::Global::GetCore()->SetAllFormatters(as_base);


  ParserCodegen generator;

  LOG_SEV(Info) << "Done deducing types. Filling in type descriptions.";

  std::ofstream fout("../../include/manta/generatedparsers/TestParser.h");
  if (fout.fail()) {
    std::cout << "Error opening file to write parser code.";
    return 0;
  }

  // std::ifstream fin("../../config/code_rules.txt");
  std::ifstream fin("../../config/full_rules_codegen.txt");
  if (fin.fail()) {
    LOG_SEV(Fatal) << "Error opening file to read parser description.";
    return 0;
  }

  try {
    generator.GenerateParserCode(fout, fin, ParserType::LALR);
  }
  catch (const std::exception& ex) {
    LOG_SEV(Fatal) << "Exception generating code: " << ex.what();
  }

  LOG_SEV(Info) << "Program finished. Exiting.";

  return 0;
}