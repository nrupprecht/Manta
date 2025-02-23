//
// Created by Nathaniel Rupprecht on 08/25/24.
//

#include <gtest/gtest.h>
// Includes
#include "manta/generator/ParserCodegen.h"
#include "manta/generator/typesystem/CppCodegen.h"

using namespace manta;

namespace UnitTest {

TEST(ParserCodegen, FreeFunction_Basic) {
  TypeSystem type_system;
  auto integer_type = type_system.Custom("int");
  ElaboratedType elaborated_integer {integer_type};

  auto function_type = FunctionType {}
                           .WithArguments({elaborated_integer, elaborated_integer, elaborated_integer})
                           .WithReturnType(elaborated_integer);

  auto function_value = FunctionValue {}
                            .WithName("item_1")
                            .WithArgumentNames({"X", "Y", "Z"})
                            .WithType(function_type)
                            .WithBody("return X + Y + Z;");

  CppCodeGen codegen;
  std::stringstream stream;
  codegen.WriteDefinition(stream, &function_value);

  EXPECT_EQ(stream.str(), "int item_1(int X, int Y, int Z) { return X + Y + Z; }");
}

TEST(ParserCodegen, FreeFunction_Void) {
  TypeSystem type_system;
  auto integer_type = type_system.Custom("int");
  ElaboratedType elaborated_integer {integer_type};

  // A void function, since the return type is not set.
  auto function_type =
      FunctionType {}.WithArguments({elaborated_integer, elaborated_integer, elaborated_integer});

  auto function_value = FunctionValue {}
                            .WithName("void_function")
                            .WithArgumentNames({"X", "Y", "Z"})
                            .WithType(function_type)
                            .WithBody("std::cout << X + Y + Z << std::endl;");

  CppCodeGen codegen;
  std::stringstream stream;
  codegen.WriteDefinition(stream, &function_value);

  EXPECT_EQ(stream.str(),
            "void void_function(int X, int Y, int Z) { std::cout << X + Y + Z << std::endl; }");
}

TEST(ParserCodegen, FreeFunction_Indentation) {
  // Test that if there are multiple lines in the function body, they are indented correctly.
  TypeSystem type_system;
  auto integer_type = type_system.Custom("int");
  ElaboratedType elaborated_integer {integer_type};

  auto function_type = FunctionType {}
                           .WithArguments({elaborated_integer, elaborated_integer, elaborated_integer})
                           .WithReturnType(elaborated_integer);

  auto function_value = FunctionValue {}
                            .WithName("my_function")
                            .WithArgumentNames({"X", "Y", "Z"})
                            .WithType(function_type)
                            .WithBody("int x = X + Y;\nauto Z = Y + Z;\nreturn x * y;");

  CppCodeGen codegen;
  std::stringstream stream;
  codegen.WriteDefinition(stream, &function_value);

  EXPECT_EQ(
      stream.str(),
      "int my_function(int X, int Y, int Z) {\n  int x = X + Y;\n  auto Z = Y + Z;\n  return x * y;\n}");
}

}  // namespace UnitTest