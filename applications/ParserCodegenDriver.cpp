#include "manta/generator/ParserCodegen.h"
#include <Lightning/Lightning.h>
#include <filesystem>

using namespace manta;
using namespace lightning;
using namespace lightning::time;

void InitializeLogging();

int main(int argc, char** argv) {
  // Set up the global logger.
  InitializeLogging();

  ParserCodegen generator;

  std::filesystem::path output_path = "../../include/manta/generatedparsers/TestParser.h";
  std::filesystem::path input_path = "../../examples/C-/C- grammar.txt";

  // Open a stream to the output path.
  if (!std::filesystem::exists(output_path)) {
    LOG_SEV(Fatal) << "File " << output_path << " does not exist. Exiting.";
    return 0;
  }
  std::ofstream fout(output_path);
  if (fout.fail()) {
    LOG_SEV(Fatal) << "Error opening file " << output_path << " to write parser code.";
    return 0;
  }
  LOG_SEV(Info) << "Opened file " << output_path << " to write parser code.";

  // Open a stream to the input path.
  std::ifstream fin(input_path);
  if (fin.fail()) {
    LOG_SEV(Fatal) << "Error opening file to read parser description.";
    return 0;
  }
  LOG_SEV(Info) << "Opened file " << input_path << " to read parser description.";

  try {
    // Generate parser code and write it to the output file.
    generator.GenerateParserCode(fout, fin, ParserType::LALR);

    // Write parser data, e.g. the parse table and how the states are represented.
    auto parser_data = generator.GetParserData();
    std::filesystem::create_directories("logs/TestParserCodegen");
    std::ofstream of("logs/TestParserCodegen/ParserData.txt");
    WriteToStream(of, *parser_data);
    of.close();
  }
  catch (const std::exception& ex) {
    LOG_SEV(Fatal) << "Exception generating code: " << ex.what();
  }
  LOG_SEV(Major) << "Program finished. Exiting.";

  return 0;
}

void InitializeLogging() {
  // Set up the global core to an OstreamSink.
  lightning::Global::GetCore()->AddSink(NewSink<lightning::StdoutSink>());

  formatting::FormatterBySeverity formatter;
  auto default_fmt = MakeMsgFormatter("[{}] [{}] {}",
                                      formatting::DateTimeAttributeFormatter{},
                                      formatting::SeverityAttributeFormatter{},
                                      formatting::MSG);
  auto low_level_fmt = MakeMsgFormatter("[{}] [{}:{}] [{}] {}",
                                        formatting::DateTimeAttributeFormatter{},
                                        formatting::FileNameAttributeFormatter{},
                                        formatting::FileLineAttributeFormatter{},
                                        formatting::SeverityAttributeFormatter{},
                                        formatting::MSG);
  formatter
      .SetDefaultFormatter(std::move(default_fmt))
      .SetFormatterForSeverity(LoggingSeverity < Severity::Info, *low_level_fmt);
  lightning::Global::GetCore()->SetAllFormatters(formatter);
}
