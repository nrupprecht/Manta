// //
// // Created by Nathaniel Rupprecht on 7/26/21.
// //
//
// #include <gtest/gtest.h>
// // Includes
// #include "manta/generator/ParserGenerator.h"
// #include "manta/lexer/LexerDFA.hpp"
// #include "manta/parser/LALRParser.hpp"
//
// using namespace manta;
//
// namespace UnitTest {
//
// TEST(AdvancedParser, ModifiedGrouping) {
//   // S -> A*
//   // A -> "a"
//   std::string description = ".Lexer\n.End\n.Parser\n.Start S\nS -> (A B)*\nA -> \"a\"\nB -> \"b\"\n .End";
//
//   ParserGenerator generator;
//   auto parser = generator.CreateParserFromString(description);
//
//   {
//     std::string example_code = "abababab";
//     std::shared_ptr<ParseNode> program {};
//     ASSERT_NO_THROW(program = parser->ParseString(example_code));
//     ASSERT_TRUE(program);
//     ASSERT_EQ(program->children.size(), 1);
//   }
// }
//
// TEST(AdvancedParser, ModifiedTerminal) {
//   // S -> A*
//   // A -> "a"
//   std::string description = ".Lexer\n.End\n.Parser\n.Start S\nS -> A*\nA -> \"a\" .End";
//
//   ParserGenerator generator;
//   auto parser = generator.CreateParserFromString(description);
//
//   {
//     std::string example_code = "aaaa";
//     std::shared_ptr<ParseNode> program {};
//     ASSERT_NO_THROW(program = parser->ParseString(example_code));
//     ASSERT_TRUE(program);
//     ASSERT_EQ(program->children.size(), 1);
//   }
// }
//
// }  // namespace UnitTest