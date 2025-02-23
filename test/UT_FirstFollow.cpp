//
// Created by Nathaniel Rupprecht on 7/26/21.
//

#include <gtest/gtest.h>
// Includes
#include "manta/generator/ParserGenerator.h"
#include "manta/lexer/LexerDFA.h"

using namespace manta;

namespace UnitTest {

TEST(FirstFollow, FirstSet) {
  // S -> A a
  // S -> A c
  // A -> b
  std::string description = ".Lexer\n.End\n.Parser\n.Start S\nS -> A \"a\" | A \"c\" \nA -> \"b\"";

  ParserGenerator generator;
  generator.CreateParserFromString(description);
  auto first_A = generator.FirstSet("A");
  EXPECT_EQ(first_A.size(), 1);
  EXPECT_EQ(first_A, std::set<std::string> {"RES:b"});

  auto first_S = generator.FirstSet("S");
  EXPECT_EQ(first_S.size(), 1);
  EXPECT_EQ(first_S, std::set<std::string> {"RES:b"});
}

TEST(FirstFollow, FirstSet2) {
  // S -> A a
  // S -> A c
  // S -> d S
  // A -> b
  // A -> e
  std::string description =
      ".Lexer\n.End\n.Parser\n.Start S\nS -> A \"a\" | A \"c\" | \"d\" S\nA -> \"b\" | \"e\"";

  ParserGenerator generator;
  generator.CreateParserFromString(description);
  auto first_A = generator.FirstSet("A");
  std::set<std::string> check_set {"RES:b", "RES:e"};
  EXPECT_EQ(first_A, check_set);

  auto first_S = generator.FirstSet("S");
  check_set = {"RES:b", "RES:d", "RES:e"};
  EXPECT_EQ(first_S, check_set);
}

TEST(FirstFollow, FollowSet) {
  // S -> Aa
  // A -> Ac
  // A -> b
  std::string description = ".Lexer\n.End\n.Parser\n.Start S\nS -> A\"a\"\n | A\"c\"\nA -> \"b\" \n";

  ParserGenerator generator;
  auto parser = generator.CreateParserFromString(description);

  // Follow(A) = { a, c }
  auto follow_set = generator.FollowSet("A");

  std::set<std::string> check_set {"RES:a", "RES:c"};
  EXPECT_EQ(follow_set, check_set);
}

TEST(FirstFollow, SLR_1) {
  // start -> E "$"
  // E -> E "+" T
  //    | T
  // T -> T "*" "num"
  //    | "num"
  std::string description =
      ".Lexer\n.End\n.Parser\nstart -> E \"$\"\nE -> E \"+\" T | T\nT -> T \"*\" \"num\" | \"num\"";

  ParserGenerator generator;
  auto parser = generator.CreateParserFromString(description);

  // Follow(A) = { a, c }
  auto follow_set_S = generator.FollowSet("start");
  auto follow_set_E = generator.FollowSet("E");
  auto follow_set_T = generator.FollowSet("T");

  std::set<std::string> check_set {"eof"};
  EXPECT_EQ(follow_set_S, check_set);

  check_set = {"RES:$", "RES:+"};
  EXPECT_EQ(follow_set_E, check_set);

  check_set = {"RES:*", "RES:+", "RES:$"};
  EXPECT_EQ(follow_set_T, check_set);
}

}  // namespace UnitTest