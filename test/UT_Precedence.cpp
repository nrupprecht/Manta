//
// Created by Nathaniel Rupprecht on 8/7/21.
//


#include <gtest/gtest.h>

// Includes
#include <cmath>

#include "manta/generator/ParserGenerator.h"
#include "manta/parser/LALRParser.h"

using namespace manta;

namespace UnitTest {

// Function to evaluate a numeric parse tree.
double evaluate(const std::shared_ptr<ParseNode>& node) {
  if (!node) {
    return std::numeric_limits<double>::quiet_NaN();
  }
  if (node->children.empty()) {
    return std::stod(node->designator);
  }

  if (node->designator == "+") {
    return evaluate(node->children[0]) + evaluate(node->children[1]);
  }
  if (node->designator == "-") {
    return evaluate(node->children[0]) - evaluate(node->children[1]);
  }
  if (node->designator == "*") {
    return evaluate(node->children[0]) * evaluate(node->children[1]);
  }
  if (node->designator == "/") {
    return evaluate(node->children[0]) / evaluate(node->children[1]);
  }
  if (node->designator == "^") {
    return std::pow(evaluate(node->children[0]), evaluate(node->children[1]));
  }

  throw std::exception();
}

TEST(Precedence, Operators) {
  std::string definition =
      ".Lexer\n"
      "    @num: r`\\d+`\n"
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

  using namespace lightning;
  Logger logger;
  logger.GetCore()->AddSink(NewSink<StdoutSink>());
  generator.SetLogger(logger);
  auto parser = generator.CreateParserFromString(definition);

  std::cout << parser->PrintTable() << std::endl;

  try {
    {
      std::string test_input = "1+2";
      auto node              = parser->ParseString(test_input);
      EXPECT_TRUE(node);  // We expected the parse to succeed.
      EXPECT_EQ(evaluate(node), 3.0);
    }

    {
      std::string test_input = "1+2/3";
      auto node              = parser->ParseString(test_input);
      EXPECT_TRUE(node);  // We expected the parse to succeed.
      EXPECT_EQ(evaluate(node), 1 + 2. / 3.);
    }

    {
      std::string test_input = "1+2-3/4";
      auto node              = parser->ParseString(test_input);
      EXPECT_TRUE(node);  // We expected the parse to succeed.
      EXPECT_EQ(evaluate(node), 1 + 2 - 3. / 4.);
    }

    {
      std::string test_input = "9*(3+7)";
      auto node              = parser->ParseString(test_input);
      EXPECT_TRUE(node);  // We expected the parse to succeed.
      EXPECT_EQ(evaluate(node), 9 * (3 + 7));
    }

    {
      std::string test_input = "3/4*5";
      auto node              = parser->ParseString(test_input);
      EXPECT_TRUE(node);  // We expected the parse to succeed.
      EXPECT_EQ(evaluate(node), 3. / 4. * 5);
    }

    {
      std::string test_input = "1+2-3/4*9*(3+7)";
      auto node              = parser->ParseString(test_input);
      EXPECT_TRUE(node);  // We expected the parse to succeed.
      EXPECT_EQ(evaluate(node), -64.5);
    }

    {
      auto node = parser->ParseString("5+98*3^4-(2*5^2)+8");
      EXPECT_TRUE(node);  // We expected the parse to succeed.
      EXPECT_EQ(evaluate(node), 7901);
    }
  } catch (const std::exception& ex) {
    std::cout << "Exception was " << ex.what() << std::endl;
    throw;
  }
}

}  // namespace UnitTest