//
// Created by Nathaniel Rupprecht on 8/7/21.
//


#include <gtest/gtest.h>

// Includes
#include "ParserGenerator.h"
#include "LALRParser.hpp"

using namespace manta;

namespace UnitTest {

// Function to evaluate a numeric parse tree.
double evaluate(const std::shared_ptr<ParseNode> &node) {
  if (node->children.empty()) {
    return std::stod(node->designator);
  }

  if (node->designator == "+") {
    return evaluate(node->children[0]) + evaluate(node->children[1]);
  }
  else if (node->designator == "-") {
    return evaluate(node->children[0]) - evaluate(node->children[1]);
  }
  else if (node->designator == "*") {
    return evaluate(node->children[0]) * evaluate(node->children[1]);
  }
  else if (node->designator == "/") {
    return evaluate(node->children[0]) / evaluate(node->children[1]);
  }
  else if (node->designator == "^") {
    return std::pow(evaluate(node->children[0]), evaluate(node->children[1]));
  }

  throw std::exception();
}

TEST(Precedence, Operators) {
  std::string definition = ".Lexer\n"
                           "    @num: \\d+\n"
                           ".End\n"
                           ".Parser\n"
                           "    start -> E @eof                        : replace($0)\n"
                           "    E -> @num                              : replace($0)\n"
                           "       | E \"+\" E -> prec(1) assoc(Left)  : node(\"+\") add($0) add($2)\n"
                           "       | E \"-\" E -> prec(1) assoc(Left)  : node(\"-\") add($0) add($2)\n"
                           "       | E \"*\" E -> prec(2) assoc(Left)  : node(\"*\") add($0) add($2)\n"
                           "       | E \"/\" E -> prec(2) assoc(Left)  : node(\"/\") add($0) add($2)\n"
                           "       | E \"^\" E -> prec(3) assoc(Right) : node(\"^\") add($0) add($2)\n"
                           "       | \"(\" E \")\"                     : replace($1)\n"
                           "\n"
                           ".End";

  ParserGenerator generator;
  auto parser = generator.CreateParserFromString(definition);

  std::string test_input = "1+2-3/4*9*(3+7)";
  auto node = parser->ParseString(test_input);
  EXPECT_TRUE(node); // We expected the parse to succeed.
  EXPECT_EQ(evaluate(node), -64.5);

  node = parser->ParseString("5+98*3^4-(2*5^2)+8");
  EXPECT_TRUE(node); // We expected the parse to succeed.
  EXPECT_EQ(evaluate(node), 7901);

}

}