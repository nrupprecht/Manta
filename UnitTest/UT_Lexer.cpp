//
// Created by Nathaniel Rupprecht on 7/25/21.
//

#include <gtest/gtest.h>
// Includes
#include "LexerGenerator.h"
#include "LexerDFA.hpp"

using namespace manta;

namespace UnitTest {

TEST(Lexer, LexEOF) {
  LexerGenerator test(false);
  test.AddLexeme("EOF", "");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 1);
}

TEST(Lexer, String) {
  LexerGenerator test(false);
  test.AddLexeme("string", "Hello");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("Hello");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 1);
}

TEST(Lexer, CharClassSimple) {
  LexerGenerator test(false);
  test.AddLexeme("lower", "[a-z]+");
  test.AddLexeme("upper", "[A-Z]+");
  // Skip spaces
  test.AddLexeme("Spaces", "\\s+");
  test.AddSkip("Spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("Hello worLDs");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 5);

  std::vector<std::string> expected_names{"upper", "lower", "lower", "upper", "lower"};

  for (std::size_t i = 0; i < std::min(lexemes.size(), expected_names.size()); ++i) {
    EXPECT_EQ(lexer->LexemeName(lexemes[i].type), expected_names[i])
              << "Disagreement for i = " << i << ": '" << lexemes[i].literal << "'";
  }
}

TEST(Lexer, CharClassComplex) {
  LexerGenerator test(false);
  test.AddLexeme("A", "[a-kL-Z]+");
  test.AddLexeme("B", "[l-zA-K]+");
  // Skip spaces
  test.AddLexeme("Spaces", "\\s+");
  test.AddSkip("Spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("talents tAlEnts TaLeNTS");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 7);

  std::vector<std::string> expected_names{"B", "A", "B", "A", "B", "B", "A"};

  for (std::size_t i = 0; i < std::min(lexemes.size(), expected_names.size()); ++i) {
    EXPECT_EQ(lexer->LexemeName(lexemes[i].type), expected_names[i])
              << "Disagreement for i = " << i << ": '" << lexemes[i].literal << "'";
  }
}

TEST(Lexer, CharClassRange) {

}

TEST(Lexer, StringComplement) {
  LexerGenerator test(false);
  test.AddLexeme("comment", "/* [~*/]* */");
  test.AddLexeme("word", "\\@+");
  // Skip spaces
  test.AddLexeme("Spaces", "\\s+");
  test.AddSkip("Spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("word /* more words inside */and continue");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 4);

  std::vector<std::string> expected_names{"word", "comment", "word", "word"};

  for (std::size_t i = 0; i < std::min(lexemes.size(), expected_names.size()); ++i) {
    EXPECT_EQ(lexer->LexemeName(lexemes[i].type), expected_names[i])
              << "Disagreement for i = " << i << ": '" << lexemes[i].literal << "'";
  }
}

TEST(Lexer, StringComplement_Example_String) {
  LexerGenerator test(false);
  test.AddLexeme("string", R"( " ( \\" | [~"] )* " )");
  // Skip spaces
  test.AddLexeme("spaces", "\\s+");
  test.AddSkip("spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex(R"("hi there" "he said \"hi there\", you know" "goodbye")");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 3);
  if (lexemes.size() == 3) {
    EXPECT_EQ(lexemes[0].literal, R"("hi there")");
    EXPECT_EQ(lexemes[1].literal, R"("he said \"hi there\", you know")");
    EXPECT_EQ(lexemes[2].literal, R"("goodbye")");
  }

  EXPECT_EQ(lexer->CheckStatus(), FAStatus::EndedNonAccepting);
}

TEST(Lexer, StringComplement_Example_Comment) {
  LexerGenerator test(false);
  test.AddLexeme("word", "\\@+");
  test.AddLexeme("comment", "# [~\\n]* \\n");
  // Skip spaces
  test.AddLexeme("spaces", "\\s+");
  test.AddSkip("spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("hi # and goodbye \n");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 3);
  if (lexemes.size() == 3) {
    EXPECT_EQ(lexemes[0].literal, R"("hi there")");
    EXPECT_EQ(lexemes[1].literal, R"("he said \"hi there\", you know")");
    EXPECT_EQ(lexemes[2].literal, R"("goodbye")");
  }

  EXPECT_EQ(lexer->CheckStatus(), FAStatus::EndedNonAccepting);
}

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

TEST(Lexer, Reserved) {
  LexerGenerator test(false);
  test.AddReserved("hello");
  test.AddReserved("world");
  test.AddLexeme("String", "\\@+");
  test.AddLexeme("Whitespaces", "\\s+");
  test.AddLexeme("EOF", "\0");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("hello world");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 4);
  std::vector<std::string> expected_names = {
      "RES:hello",
      "Whitespaces",
      "RES:world",
      "EOF"
  };

  for (std::size_t i = 0; i < std::min(lexemes.size(), expected_names.size()); ++i) {
    EXPECT_EQ(lexer->LexemeName(lexemes[i].type), expected_names[i])
              << "Disagreement for i = " << i << ": '" << lexemes[i].literal << "'";
  }
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

  // Add reserved words.
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
  for (std::size_t i = 0; i < std::min(expected_names.size(), lexemes.size()); ++i) {
    EXPECT_EQ(lexer->LexemeName(lexemes[i].type), expected_names[i]) << "Disagreement for i = " << i;
  }
  // We expect to end in state 3 (ended after encountering EOF).
  EXPECT_EQ(lexer->CheckStatus(), FAStatus::AcceptedEOF);
}

TEST(Lexer, Words) {
  LexerGenerator test(false);
  test.AddLexeme("Hello", "Hello");
  test.AddLexeme("Goodbye", "Goodbye");
  test.AddLexeme("Xthing", "(Any|Every)thing");
  test.AddLexeme("Whitespaces", "\\s+");
  test.AddLexeme("EOF", "");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("Hello Anything Everything Goodbye");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 8);

  std::vector<std::string> expected_names = {
      "Hello",
      "Whitespaces",
      "Xthing",
      "Whitespaces",
      "Xthing",
      "Whitespaces",
      "Goodbye",
      "EOF"
  };

  for (std::size_t i = 0; i < expected_names.size(); ++i) {
    EXPECT_EQ(lexer->LexemeName(lexemes.at(i).type), expected_names[i]) << "Disagreement for i = " << i;
  }
  // We expect to end in state 3 (ended after encountering EOF).
  EXPECT_EQ(lexer->CheckStatus(), FAStatus::AcceptedEOF);
}

TEST(Lexer, SkipLexemes) {
  LexerGenerator test(false);
  test.AddLexeme("String", "\\@+");
  test.AddLexeme("Whitespaces", "\\s+");
  test.AddLexeme("Punctuation", ". | , | ! | ?");
  test.AddLexeme("EOF", "");

  // Skip whitespaces.
  test.AddSkip("Whitespaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("String for, hello world!");
  auto lexemes = lexer->LexAll();

  // We expect 21 lexemes.
  EXPECT_EQ(lexemes.size(), 7);
  std::vector<std::string> expected_names = {
      "String",
      "String",
      "Punctuation",
      "String",
      "String",
      "Punctuation",
      "EOF",
  };
  for (std::size_t i = 0; i < 7; ++i) {
    EXPECT_EQ(lexer->LexemeName(lexemes[i].type), expected_names[i]) << "Disagreement for i = " << i;
  }
  // We expect to end in state 3 (ended after encountering EOF).
  EXPECT_EQ(lexer->CheckStatus(), FAStatus::AcceptedEOF);
}

TEST(Lexer, WithUnderscores) {
  LexerGenerator test(false);
  test.AddLexeme("identifier", "(\\@ | _)+");
  // Need these too.
  test.AddLexeme("spaces", "\\s+");
  test.AddLexeme("EOF", "");

  // Skip whitespaces.
  test.AddSkip("spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex("hello_world how _are_ you");

  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 5);
  std::vector<std::string> expected_names = {
      "identifier", "identifier", "identifier", "identifier", "EOF"
  };

  for (std::size_t i = 0; i < std::min(lexemes.size(), expected_names.size()); ++i) {
    EXPECT_EQ(lexer->LexemeName(lexemes[i].type), expected_names[i])
              << "Disagreement for i = " << i << ": '" << lexemes[i].literal << "'";
  }
}

TEST(Lexer, LexemeWithQuotes) {
  LexerGenerator test(false);
  test.AddLexeme("regex", R"(" ( \@ | \d | \| | \\ | \s | @ | + | * | _ )* ")");
  // test.AddLexeme("regex", "\" ( \\@ | \\\\ | @ | + )* \"");
  // test.AddLexeme("regex", "\" ( \\@ | @ | + | \\\\ )* \"");
  test.AddLexeme("spaces", "\\s+", 5);
  test.AddLexeme("eof", "");

  // Skip whitespaces.
  test.AddSkip("spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex(R"("@\@+")");
  // lexer->SetStringToLex("\"A\\@+A\"");
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 2);
  std::vector<std::string> expected_names = {
      "regex",
      "eof",
  };

  for (std::size_t i = 0; i < lexemes.size(); ++i) {
    EXPECT_EQ(lexer->LexemeName(lexemes[i].type), expected_names[i])
              << "Disagreement for i = " << i << ": '" << lexemes[i].literal << "'";
  }

  // We expect to end in state 3 (ended after encountering EOF).
  EXPECT_EQ(lexer->CheckStatus(), FAStatus::AcceptedEOF);
}

TEST(Lexer, LexTheLexer) {
  LexerGenerator test(false);
  test.AddLexeme("special_symbol", ".\\@+", 3);
  test.AddLexeme("semicolon", ":", 2);
  test.AddLexeme("lexeme_name", "@(\\@ | _)+", 1);
  test.AddLexeme("identifier", "(\\@ | _)+", 1);
  test.AddLexeme("regex", "r` ( \\\\ ` | [~`])* `");
  test.AddLexeme("spaces", "\\s+", 5);
  test.AddLexeme("newlines", "\\n+");
  test.AddLexeme("eof", "");

  // Skip whitespaces.
  test.AddSkip("spaces");

  auto lexer = test.CreateLexer();
  lexer->SetStringToLex(
      ".Lexer                               \n"
      "   @special_symbol: r`\".\\@+\"`       \n"
      "   @lexeme_name: r`\"@\\@+\"`           \n"
      "   @semicolon: r`\":\"`                 \n"
      "   @identifier: r`\"( \\@ | _ )+\"`     \n"
      "   @regex: r`\"(\\@ | \\d | \\| | \\\\ | \\s | @ | + | * | _ | \\( | \\) | . | : )*\"`  \n"
      "   @spaces: r`\"\\s+\"`                 \n"
      "   @newlines: r`\"\\n+\"`               \n"
      "   @eof: r`\"\\0\"`                     \n"
      ".End                                 \n"
  );
  auto lexemes = lexer->LexAll();

  EXPECT_EQ(lexemes.size(), 37);
  std::vector<std::string> expected_names = {
      "special_symbol",
      "newlines",
      "lexeme_name",
      "semicolon",
      "regex",
      "newlines",
      "lexeme_name",
      "semicolon",
      "regex",
      "newlines",
      "lexeme_name",
      "semicolon",
      "regex",
      "newlines",
      "lexeme_name",
      "semicolon",
      "regex",
      "newlines",
      "lexeme_name",
      "semicolon",
      "regex",
      "newlines",
      "lexeme_name",
      "semicolon",
      "regex",
      "newlines",
      "lexeme_name",
      "semicolon",
      "regex",
      "newlines",
      "lexeme_name",
      "semicolon",
      "regex",
      "newlines",
      "special_symbol",
      "newlines",
      "eof",
  };
  for (std::size_t i = 0; i < std::min(lexemes.size(), expected_names.size()); ++i) {
    EXPECT_EQ(lexer->LexemeName(lexemes[i].type), expected_names[i])
              << "Disagreement for i = " << i << ": '" << lexemes[i].literal << "'";
  }

  // We expect to end in state 3 (ended after encountering EOF).
  EXPECT_EQ(lexer->CheckStatus(), FAStatus::AcceptedEOF)
            << "Ended on line: " << lexer->GetLine() << ", character: " << lexer->GetColumn();
}

} // namespace UnitTest
