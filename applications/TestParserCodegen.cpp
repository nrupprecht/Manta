#include "manta/generator/ParserCodegen.h"
#include <Lightning/Lightning.h>

using namespace manta;
using namespace lightning;
using namespace lightning::time;

int main(int argc, char** argv) {
  lightning::Global::GetCore()->AddSink(std::make_shared<OstreamSink>());

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