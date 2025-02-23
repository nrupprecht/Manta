//
// Created by Nathaniel Rupprecht on 12/28/24.
//

#include <gtest/gtest.h>
// Includes
#include "manta/generator/codegen/CodeGen.h"
#include "manta/generator/typesystem/HigherTypeSystem.h"

using namespace manta::typesystem;
using namespace manta::codegen;

namespace UnitTest {

TEST(CppCodegen, WriteName) {
  GeneralizedTypeSystem type_system;
  CppCodegen codegen;
  auto vector_tc = type_system.GetObjectType("std::vector", 1);
  auto int_t     = type_system.GetBasicType("int");

  {
    std::ostringstream out;
    std::vector<std::string> names {"T"};
    ASSERT_NO_THROW(codegen.WriteName(out, vector_tc, names));
    ASSERT_EQ(out.str(), "std::vector<T>");
  }

  auto vector_int_t = type_system.Substitute(vector_tc, {{0, int_t}});
  {
    std::ostringstream out;
    ASSERT_NO_THROW(codegen.WriteName(out, vector_int_t));
    ASSERT_EQ(out.str(), "std::vector<int>");
  }

  auto vector_vector_tc = type_system.Substitute(vector_tc, {{0, vector_tc}});
  {
    std::ostringstream out;
    std::vector<std::string> names {"T"};
    ASSERT_NO_THROW(codegen.WriteName(out, vector_vector_tc, names));
    ASSERT_EQ(out.str(), "std::vector<std::vector<T>>");
  }

  auto vector_vector_int_t = type_system.Substitute(vector_vector_tc, {{0, int_t}});
  {
    std::ostringstream out;
    ASSERT_NO_THROW(codegen.WriteName(out, vector_vector_int_t));
    ASSERT_EQ(out.str(), "std::vector<std::vector<int>>");
  }

  auto pair_tc = type_system.GetObjectType("std::pair", 2);

  auto pair_vec_t_int = type_system.Substitute(pair_tc, {{0, vector_tc}, {1, int_t}});
  {
    std::ostringstream out;
    std::vector<std::string> names {"T"};
    ASSERT_NO_THROW(codegen.WriteName(out, pair_vec_t_int, names));
    ASSERT_EQ(out.str(), "std::pair<std::vector<T>, int>");
  }

  auto pair_vec_vec_tc = type_system.Substitute(pair_tc, {{0, vector_tc}, {1, vector_tc}});
  {
    std::ostringstream out;
    std::vector<std::string> names {"S", "T"};
    ASSERT_NO_THROW(codegen.WriteName(out, pair_vec_vec_tc, names));
    ASSERT_EQ(out.str(), "std::pair<std::vector<S>, std::vector<T>>");
  }

  auto pair_vec_int_vec_int_t = type_system.Substitute(pair_vec_vec_tc, {{0, int_t}, {1, int_t}});
  {
    std::ostringstream out;
    ASSERT_NO_THROW(codegen.WriteName(out, pair_vec_int_vec_int_t));
    ASSERT_EQ(out.str(), "std::pair<std::vector<int>, std::vector<int>>");
  }
}

TEST(HigherTypeSystem, Enumeration) {
  GeneralizedTypeSystem type_system;
  CppCodegen codegen;

  auto int_t = type_system.GetBasicType("int");

  using namespace std::string_literals;

  auto enum_t =
      type_system.GetEnumerationType("Colors", {{"RED"s, int_t}, {"GREEN"s, int_t}, {"BLUE"s, int_t}});

  std::ostringstream out;
  codegen.WriteDefinition(out, enum_t);
  ASSERT_EQ(out.str(), "enum class Colors : int {\n  RED,\n  GREEN,\n  BLUE,\n};\n");
}

TEST(CppCodegen, Objects) {
  GeneralizedTypeSystem type_system;
  CppCodegen codegen;

  auto my_class_proto = type_system.RegisterObjectPrototype("MyClass");
  auto my_class       = type_system.GetType(my_class_proto);
  auto vector_tc      = type_system.GetObjectType("vector", "std", 1);

  auto string_t = type_system.GetObjectType("string", "std");

  my_class_proto->AddField("field_1", string_t);
  my_class_proto->AddField("next_node", my_class);

  {
    std::ostringstream out;
    codegen.WriteDeclaration(out, my_class);
    ASSERT_EQ(out.str(), "class MyClass;");
  }

  {
    std::ostringstream out;
    codegen.WriteDefinition(out, my_class);
    ASSERT_EQ(out.str(), "class MyClass {\n  std::string field_1;\n  MyClass next_node;\n};\n");
  }

  {
    std::ostringstream out;
    ASSERT_NO_THROW(codegen.WriteDeclaration(out, vector_tc));
    ASSERT_EQ(out.str(), "template <typename T0_t>\nclass vector;");
  }

  auto vector_int_t = type_system.Substitute(vector_tc, {{0, string_t}});
  {
    std::ostringstream out;
    ASSERT_ANY_THROW(codegen.WriteDeclaration(out, vector_int_t));
  }
}

}  // namespace UnitTest