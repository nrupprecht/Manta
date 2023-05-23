#include "manta/lexer/LexerDFA.hpp"
#include "manta/generator/ParserCodegen.h"
#include "manta/parser/LALRParser.hpp"
#include "manta/visualization/Display.hpp"
#include "manta/lightning/Lightning.h"

using namespace manta;
using namespace lightning;
using namespace lightning::time;

namespace std {

void format_logstream(const exception& ex, lightning::RecordHandler& handler) {
  using namespace lightning::formatting;

  handler << NewLineIndent << AnsiColor8Bit(R"(""")", AnsiForegroundColor::Red)
          << StartAnsiColor8Bit(AnsiForegroundColor::Yellow); // Exception in yellow.
  const char* begin = ex.what(), * end = ex.what();
  while (*end) {
    for (; *end && *end != '\n'; ++end); // Find next newline.
    handler << NewLineIndent << string_view(begin, end - begin);
    while (*end && *end == '\n') ++end; // Pass any number of newlines.
    begin = end;
  }
  handler << ResetStreamColor() << NewLineIndent // Reset colors to default.
          << AnsiColor8Bit(R"(""")", AnsiForegroundColor::Red);
}

} // namespace std

int main(int argc, char** argv) {
  lightning::Global::GetCore()
      ->AddSink<UnsynchronizedFrontend, OstreamSink>()
      .SetAllFormats("[{Severity}][{DateTime}]: {Message}");

  Global::GetLogger()
      .AddAttribute(attribute::DateTimeAttribute{}, attribute::DateTimeFormatter{})
      .AddLoggerAttributeFormatter(attribute::SeverityFormatter{});

  ParserCodegen generator;

  LOG_SEV(Info) << "Done deducing types. Filling in type descriptions.";

  std::ofstream fout("../../include/manta/generatedparsers/TestParser.h");
  if (fout.fail()) {
    std::cout << "Error opening file to write parser code.";
    return 0;
  }

  // std::ifstream fin("../../config/full_rules_codegen.txt");
  std::ifstream fin("../../config/simple-rules.txt");
  if (fin.fail()) {
    std::cout << "Error opening file to read parser description.";
    return 0;
  }

  try {
    generator.GenerateParserCode(fout, fin, ParserType::LALR);
  }
  catch (const std::exception& ex) {
    LOG_SEV(Fatal) << "Exception generating code: " << lightning::formatting::NewLineIndent << ex;
  }

  LOG_SEV(Info) << "Program finished. Exiting.";

  return 0;
}