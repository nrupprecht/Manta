//
// Created by Nathaniel Rupprecht on 7/30/23.
//

#include <gtest/gtest.h>
// Includes
#include "manta/utility/Formatting.h"
#include "manta/utility/StreamFunctions.h"

using namespace manta;
using namespace manta::utility;

namespace UnitTest {

TEST(Utility, GetUntil_1) {
  std::stringstream stream;
  stream << "HelloXWorldX";
  auto container = IStreamContainer::FromStream(stream);
  EXPECT_EQ(GetUntil(container, 'X'), "Hello");
  EXPECT_EQ(GetUntil(container, 'X'), "World");
}

TEST(Utility, GetUntil_2) {
  std::stringstream stream;
  stream << "HelloXWorldX";
  auto container = IStreamContainer::FromStream(stream);
  EXPECT_EQ(GetUntil(container, '\0'), "HelloXWorldX");
}

TEST(Utility, EscapedChars) {
  std::stringstream stream;
  stream << "\\A\\BCB";
  auto container = IStreamContainer::FromStream(stream);
  EXPECT_EQ(GetUntil(container, 'B'), "\\ABC");
}

TEST(Utility, Exceptions) {
  std::stringstream stream;
  stream << "Hello";
  auto container = IStreamContainer::FromStream(stream);
  EXPECT_ANY_THROW(GetUntil(container, 'X'));
}

TEST(Utility, DetectIndentation) {
  std::string input = " A\n  X\n";
  EXPECT_EQ(formatting::DetectIndentation(input), 1);

  input = "  A\n   X\n";
  EXPECT_EQ(formatting::DetectIndentation(input), 2);

  input = "\n  A\n   X\n";
  EXPECT_EQ(formatting::DetectIndentation(input), 2);

  // TEST: Can't detect indentation if there are no non-whitespaces.
  input = "    \n   \n";
  EXPECT_EQ(formatting::DetectIndentation(input), 0);

  // TEST: Don't detect indentation from lines that are just whitespace.
  input = "    X\n    Y\n  ";
  EXPECT_EQ(formatting::DetectIndentation(input), 4);

  input =
      "\n        LOG_SEV(Info) << \"Reading definition for non-terminal \" << "
      "manta::formatting::CLY(object.nonterminal_name)\n                          << \", there are \" << "
      "object.productions.size() << \" productions.\";\n\n        auto pid = "
      "registerProduction(object.nonterminal_name);\n        makeNextItem(pid);  // Create the item for this "
      "production.\n        for (auto& production : object.productions) {\n          "
      "production->Accept(*this);\n        }\n        storeCurrentItem(); // Done creating the item, store "
      "it.\n    ";
  EXPECT_EQ(formatting::DetectIndentation(input), 8);
}

TEST(Utility, HandleIndentation) {
  std::stringstream stream;
  std::string input {};

  input = "Hello\n\nWorld!\n";
  formatting::HandleIndentation(stream, input, 2, true);
  EXPECT_EQ(stream.str(), "  Hello\n\n  World!\n");
  stream.str("");  // Clear

  input = "  Hello\n\t World!\n\nGood.";
  formatting::HandleIndentation(stream, input, 0, true, false);
  EXPECT_EQ(stream.str(), "Hello\nWorld!\n\nGood.\n");
  stream.str("");  // Clear

  input = "  if (true) {\n    return 0;\n  }\n";
  formatting::HandleIndentation(stream, input, 0, true);
  EXPECT_EQ(stream.str(), "if (true) {\n  return 0;\n}\n");
  stream.str("");  // Clear

  input = "Hi";
  formatting::HandleIndentation(stream, input, 4, true);
  EXPECT_EQ(stream.str(), "    Hi\n");
  stream.str("");  // Clear

  // TEST: This based on an input that caused an error at one point.
  input =
      "\n            // Indicates a non-terminal.\n            registerProduction(object.identifier);\n      "
      "  ";
  formatting::HandleIndentation(stream, input, 0, true);
  EXPECT_EQ(stream.str(), "\n// Indicates a non-terminal.\nregisterProduction(object.identifier);\n");
  stream.str("");  // Clear

  // TEST: Remove leading whitespaces.
  input =
      "\n  \n            // Indicates a non-terminal.\n            registerProduction(object.identifier);\n      "
      "  ";
  formatting::HandleIndentation(stream, input, 0, true, true, true, false);
  EXPECT_EQ(stream.str(), "// Indicates a non-terminal.\nregisterProduction(object.identifier);\n");
  stream.str("");  // Clear

  // TEST: Remove trailing whitespaces.
  input =
      "\n            // Indicates a non-terminal.\n            registerProduction(object.identifier);\n      "
      "  ";
  formatting::HandleIndentation(stream, input, 0, true, true, false, true);
  EXPECT_EQ(stream.str(), "\n// Indicates a non-terminal.\nregisterProduction(object.identifier);");
  stream.str("");  // Clear

  // TEST: Remove leading and trailing whitespaces.
  input =
      "\n  \n            // Indicates a non-terminal.\n            registerProduction(object.identifier);\n      "
      "  ";
  formatting::HandleIndentation(stream, input, 0, true, true, true, true);
  EXPECT_EQ(stream.str(), "// Indicates a non-terminal.\nregisterProduction(object.identifier);");
  stream.str("");  // Clear
}

}  // namespace UnitTest