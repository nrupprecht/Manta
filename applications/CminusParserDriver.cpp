//
// Created by Nathaniel Rupprecht on 2/25/24.
//

#include <Lightning/Lightning.h>

#include "manta/generatedparsers/CminusParser.h"
#include "manta/utility/Timer.h"

using namespace lightning;

auto main() -> int {
  /*
   * Application to run the Manta generated "C- language" parser.
   *
   * I found the C- language at http://marvin.cs.uidaho.edu/Teaching/CS445/ and thought it would be a really good test
   * case since it is not at all a trivial language, but it also lacks the complexity of e.g. a full C++ parser, which
   * is not even a context free grammar, and does not require indentation based parsing like Python.
   *
   */

  Global::GetCore()->AddSink(NewSink<StdoutSink>());

  Logger logger;
  logger.GetCore()->AddSink(NewSink<StdoutSink>());

  Parser parser;
  parser.SetLogger(logger);
  parser.SetInput(manta::utility::IStreamContainer::OpenFile("../../examples/C-/C- code.cmm"));

  manta::utility::Timer timer{};
  std::shared_ptr<ASTNodeBase> node{};
  try {
    node = parser.ParseInput();
  }
  catch (const std::exception& ex) {
    LOG_SEV_TO(logger, Fatal) << "Exception caught: " << ex.what();
    return 1;
  }
  timer.Stop();

  LOG_SEV_TO(logger, Info) << "Parse took " << timer.Time() << " seconds.";

  PrintingVisitor visitor;
  node->Accept(visitor);

  return 0;
}