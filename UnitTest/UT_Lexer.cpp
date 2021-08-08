//
// Created by Nathaniel Rupprecht on 7/25/21.
//

#include <gtest/gtest.h>
// Includes
#include "LexerGenerator.h"
#include "LexerDFA.hpp"

using namespace manta;

namespace UnitTest {

TEST(Lexer, Basic) {
  LexerGenerator test(false);
  test.AddLexeme("Pattern1", "[A-Z] [a-z]+");
  test.AddLexeme("Pattern2", "[A-Z] [a-z]+ [A-Z]");
  test.AddLexeme("Pattern3", "[a-z]+ [A-Z]");
  // Skip spaces
  test.AddLexeme("Spaces", "\\s+");
  test.AddSkip("Spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("Hello WorlD friendS");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 3);
  EXPECT_EQ(lexer->CheckStatus(), FAStatus::EndedNonAccepting);
  EXPECT_EQ(lexer->LexemeName(lexemes[0].type), "Pattern1");
  EXPECT_EQ(lexer->LexemeName(lexemes[1].type), "Pattern2");
  EXPECT_EQ(lexer->LexemeName(lexemes[2].type), "Pattern3");
}

TEST(Lexer, OR_1) {
  LexerGenerator test(false);
  test.AddLexeme("Word", "([a-z] | [A-Z])+");
  test.AddLexeme("Spaces", "\\s+");
  test.AddSkip("Spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("HeldiwK OpodO");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 2);
}

TEST(Lexer, OR_2) {
  LexerGenerator test(false);
  test.AddLexeme("Word", "([A-Z] [a-z]* | )+");
  test.AddLexeme("Spaces", "\\s+");
  test.AddSkip("Spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("HeldiwK OpodO");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 2);
}

TEST(Lexer, Precedence_I) {

  LexerGenerator test(false);
  test.AddLexeme("LowerString", "[a-z]+", 2);
  test.AddLexeme("UpperString", "[A-Z]+", 2);
  test.AddLexeme("MixedString", "\\@+", 1);
  test.AddLexeme("Whitespaces", "\\s+");
  // Skip whitespaces.
  test.AddSkip("Whitespaces");
  auto lexer = test.CreateLexer();

  lexer->SetStringToLex("hello WORLD myFriend");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 3);
  EXPECT_EQ(lexer->LexemeName(lexemes[0].type), "LowerString");
  EXPECT_EQ(lexer->LexemeName(lexemes[1].type), "UpperString");
  EXPECT_EQ(lexer->LexemeName(lexemes[2].type), "MixedString");

  // We expect to end in state FAStatus::EndedNonAccepting (ended after encountering EOF, but did not accept EOF).
  EXPECT_EQ(lexer->CheckStatus(), FAStatus::EndedNonAccepting);
}

TEST(Lexer, Cases) {
  LexerGenerator test(false);
  test.AddLexeme("SnakeCase", "([a-z]+ _ )+ [a-z]+ | [a-z]+", 2);
  test.AddLexeme("CapSnakeCase", "([A-Z][a-z]* _ )+ [A-Z] [a-z]* | [A-Z] [a-z]*", 2);
  test.AddLexeme("OtherWord", "( \\@ | _ )+");
  test.AddLexeme("Spaces", "\\s+");
  test.AddSkip("Spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("snake_case Cap_Snake_Case Not_snake_case word_on_the_move ");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 4);
  EXPECT_EQ(lexer->LexemeName(lexemes[0].type), "SnakeCase");
  EXPECT_EQ(lexer->LexemeName(lexemes[1].type), "CapSnakeCase");
  EXPECT_EQ(lexer->LexemeName(lexemes[2].type), "OtherWord");
  EXPECT_EQ(lexer->LexemeName(lexemes[3].type), "SnakeCase");

  // We expect to end in state FAStatus::EndedNonAccepting (ended after encountering EOF, but did not accept EOF).
  // We expect this even though the string ends with a skip token (space).
  EXPECT_EQ(lexer->CheckStatus(), FAStatus::EndedNonAccepting);
}

TEST(Lexer, LargeTest) {

  LexerGenerator test(false);
  test.AddLexeme("String", "\\@+");
  test.AddLexeme("Whitespaces", "\\s+");
  test.AddLexeme("Punctuation", ". | , | ! | ?");
  test.AddLexeme("Integer", "\\d+");
  test.AddLexeme("Double", "\\d+ . \\d*");
  test.AddLexeme("Float", "\\d+ . \\d*f");
  test.AddLexeme("Hexidecimal", "0x([A-F] | \\d)+");
  test.AddLexeme("EOF", "\0");
  test.AddReserved("for");
  test.AddReserved("String");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("String for, hello world! 3.145, 9, 3.2f, 0x01FF");
  auto lexemes = lexer->LexAll();

  // We expect 21 lexemes.
  EXPECT_EQ(lexemes.size(), 21);
  // Record the expected lexeme types.
  //  Note: We do this instead of using the integer representations of the states, since we do
  //  not care if the implementation / bookkeeping details of state representation change.
  std::vector<std::string> expected_names = {
      "RES:String",
      "Whitespaces",
      "RES:for",
      "Punctuation",
      "Whitespaces",
      "String",
      "Whitespaces",
      "String",
      "Punctuation",
      "Whitespaces",
      "Double",
      "Punctuation",
      "Whitespaces",
      "Integer",
      "Punctuation",
      "Whitespaces",
      "Float",
      "Punctuation",
      "Whitespaces",
      "Hexidecimal",
      "EOF"
  };
  for (std::size_t i = 0; i < 21; ++i) {
    EXPECT_EQ(lexer->LexemeName(lexemes[i].type), expected_names[i]);
  }
  // We expect to end in state 3 (ended after encountering EOF).
  EXPECT_EQ(lexer->CheckStatus(), FAStatus::AcceptedEOF);
}

}