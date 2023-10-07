//
// Created by Nathaniel Rupprecht on 4/1/23.
//

#include <Lightning/Lightning.h>

#include "manta/generatedparsers/TestParser.h"
#include "manta/utility/Timer.h"

auto main() -> int {
  lightning::Global().GetCore()->AddSink(
      lightning::NewSink<lightning::UnlockedSink, lightning::OstreamSink>());

  lightning::Logger logger;
  logger.GetCore()->AddSink(lightning::NewSink<lightning::UnlockedSink, lightning::OstreamSink>());

  Parser parser;
  parser.SetLogger(logger);

  // parser.SetInput(manta::utility::IStreamContainer::OpenFile("../../examples/codefile.txt"));
  parser.SetInput(manta::utility::IStreamContainer::OpenFile("../../examples/basic_parser_and_lexer.txt"));

  manta::utility::Timer timer {};
  auto node = parser.ParseInput();
  timer.Stop();

  LOG_SEV_TO(logger, Info) << "Parse took " << timer.Time() << " seconds.";

  PrintingVisitor visitor;
  node->Accept(visitor);

  return 0;
}