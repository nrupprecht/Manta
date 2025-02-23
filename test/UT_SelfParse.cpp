//
// Created by Nathaniel Rupprecht on 2/13/23.
//

#include <gtest/gtest.h>
// Includes
#include "manta/generator/ParserGenerator.h"
#include "manta/parser/LALRParser.h"

using namespace manta;

namespace UnitTest {

TEST(Lexer, LexRegex) {
  std::string definition = ".Lexer\n"
                           "    @regex: r` r\\` ( \\ \\` | [~\\`])* \\` `\n"
                           ".End\n"
                           ".Parser\n"
                           "    start -> E @eof\n"
                           "    E -> @regex\n"
                           ".End";

  ParserGenerator generator;
  auto parser = generator.CreateParserFromString(definition);

  auto lexer = parser->GetLexer();

  lexer->SetStringToLex("r`(\\@ | \\d)*`");

  std::string test_input = "r`(\\@ | \\d)*`";
  auto node = parser->ParseString(test_input);
  EXPECT_TRUE(node); // We expected the parse to succeed.
}

} // namespace UnitTest

