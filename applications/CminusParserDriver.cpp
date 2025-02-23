//
// Created by Nathaniel Rupprecht on 2/25/24.
//

#include "manta/generatedparsers/CminusParser.h"

#include <Lightning/Lightning.h>

#include "manta/utility/Timer.h"

using namespace lightning;

auto main() -> int {
  /*
   * Application to run the Manta generated "C- language" parser.
   *
   * I found the C- language at http://marvin.cs.uidaho.edu/Teaching/CS445/ and thought it would be a really
   * good test case since it is not at all a trivial language, but it also lacks the complexity of e.g. a full
   * C++ parser, which is not even a context free grammar, and does not require indentation based parsing like
   * Python.
   *
   */

  Global::GetCore()->AddSink(NewSink<StdoutSink>());

  Logger logger;
  logger.GetCore()->AddSink(NewSink<StdoutSink>());

#ifdef MANTA_PARSER_GENERATED

  std::string input_file = "examples/C-/C- code.cmm";

  MANTA_ASSERT(std::filesystem::exists(input_file), "input file " << input_file << " does not exist");
  LOG_SEV(Info) << "Parsing input from '" << input_file << "'.";

  Parser parser;
  parser.SetLogger(logger);
  parser.SetInput(manta::utility::IStreamContainer::OpenFile(input_file));

  manta::utility::Timer timer {};
  std::shared_ptr<ASTNodeBase> node {};
  try {
    node = parser.ParseInput();
  } catch (const std::exception& ex) {
    LOG_SEV_TO(logger, Fatal) << "Exception caught: " << ex.what();
    return 1;
  }
  timer.Stop();

  LOG_SEV_TO(logger, Info) << "Parse took " << timer.Time() << " seconds.";

  // PrintingVisitor visitor;
  // node->Accept(visitor);

#endif  // MANTA_PARSER_GENERATED

  return 0;
}