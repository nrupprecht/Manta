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

  // std::filesystem::path input_path = "../../examples/C-/C- grammar.txt";
  // std::filesystem::path output_path = "../../include/manta/generatedparsers/CminusParser.h";

  std::filesystem::path input_path = "../../config/full_rules_codegen.txt";
  std::filesystem::path output_path = "../../include/manta/generatedparsers/ParserRepresentation.h";

  // Open a stream to the output path.
  // if (!std::filesystem::exists(output_path)) {
  //   LOG_SEV(Fatal) << "File " << output_path << " does not exist. Exiting.";
  //   return 0;
  // }
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
    LOG_SEV(Fatal) << "Exception generating code:" << ex;
    return 1;
  }
  LOG_SEV(Major) << "Program finished. Output is written to '" << output_path << "'. Exiting.";

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
