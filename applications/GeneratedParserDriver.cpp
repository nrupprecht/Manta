//
// Created by Nathaniel Rupprecht on 4/1/23.
//

#include <Lightning/Lightning.h>

#include "manta/generatedparsers/CminusParser.h"
#include "manta/utility/Timer.h"

using namespace lightning;

auto main() -> int {
  Global::GetCore()->AddSink(NewSink<StdoutSink>());

  Logger logger;
  logger.GetCore()->AddSink(NewSink<StdoutSink>());

#ifdef MANTA_PARSER_GENERATED

  Parser parser;
  parser.SetLogger(logger);

  // parser.SetInput(manta::utility::IStreamContainer::OpenFile("../../examples/codefile.txt"));
  parser.SetInput(manta::utility::IStreamContainer::OpenFile("../../examples/C-/C- code.cmm"));

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

#endif  // MANTA_PARSER_GENERATED

  return 0;
}