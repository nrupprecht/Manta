//
// Created by Nathaniel Rupprecht on 11/12/23.
//

#include <gtest/gtest.h>
// Includes
#include <gmock/internal/gmock-internal-utils.h>

#include "manta/generator/LexerGenerator.h"
#include "manta/generator/ParserGenerator.h"
#include "manta/lexer/LexerDFA.h"
#include "manta/parser/LALRParser.h"

using namespace manta;

namespace UnitTest {
TEST(Parser, Parser) {
  const std::string parser_description =
  R"(
    .Lexer"
        @integer: r`\d+`
        @spaces: r`\s+`
    .Skip spaces
    .End
    .Parser
        .Start start

        start -> expr
        expr -> "[" expr "]"
        | expr "+" expr
        | expr "-" expr
        | expr "*" expr
        | expr "/" expr
        | @integer
    .End
  )";
  const std::string example_code = "1+[2*3]";

  ParserGenerator generator(ParserType::LALR);
  std::shared_ptr<LALRParser> parser{};
  ASSERT_NO_THROW(parser = generator.CreateParserFromString(parser_description));
  std::shared_ptr<ParseNode> program{};
  ASSERT_NO_THROW(program = parser->ParseString(example_code));
  ASSERT_TRUE(program);

  // Check the full parse tree.
  EXPECT_EQ(program->designator, "start");
  ASSERT_EQ(program->children.size(), 1);
  auto&& expr = program->children[0];
  ASSERT_EQ(expr->children.size(), 3);
  auto expr1 = expr->children[0];
  auto expr2 = expr->children[2];
  EXPECT_EQ(expr1->designator, "expr");
  EXPECT_EQ(expr->children[1]->designator, "+");
  EXPECT_EQ(expr2->designator, "expr");
  ASSERT_EQ(expr1->children.size(), 1);
  ASSERT_EQ(expr2->children.size(), 3);
  EXPECT_EQ(expr1->children[0]->designator, "1");
  EXPECT_EQ(expr2->children[0]->designator, "[");
  EXPECT_EQ(expr2->children[1]->designator, "expr");
  EXPECT_EQ(expr2->children[2]->designator, "]");
  expr = expr2->children[1];
  ASSERT_EQ(expr->children.size(), 3);
  EXPECT_EQ(expr->children[0]->designator, "expr");
  ASSERT_EQ(expr->children[0]->children.size(), 1);
  EXPECT_EQ(expr->children[0]->children[0]->designator, "2");
  EXPECT_EQ(expr->children[1]->designator, "*");
  EXPECT_EQ(expr->children[2]->designator, "expr");
  ASSERT_EQ(expr->children[2]->children.size(), 1);
  EXPECT_EQ(expr->children[2]->children[0]->designator, "3");
}
} // namespace UnitTest
