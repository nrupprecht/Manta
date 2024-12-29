//
// Created by Nathaniel Rupprecht on 12/22/24.
//

#include <gtest/gtest.h>
// Includes
#include "manta/generator/typesystem/HigherTypeSystem.h"

using namespace manta::typesystem;

namespace UnitTest {

TEST(HigherTypeSystem, TemplateType) {
  GeneralizedTypeSystem type_system;
  auto vector_tc = type_system.GetObjectType("std::vector", 1);
  auto int_t     = type_system.GetBasicType("int");

  ASSERT_TRUE(int_t->IsType());
  ASSERT_FALSE(vector_tc->IsType());
  ASSERT_EQ(vector_tc->Write(), "object std::vector<$0>");

  auto vector_int_t = type_system.Substitute(vector_tc, {{0, int_t}});
  ASSERT_TRUE(vector_int_t->IsType());
  ASSERT_EQ(vector_int_t->Write(), "object std::vector<int>");

  auto vector_vector_tc = type_system.Substitute(vector_tc, {{0, vector_tc}});
  ASSERT_FALSE(vector_vector_tc->IsType());
  ASSERT_EQ(vector_vector_tc->Write(), "object std::vector<object std::vector<$0>>");

  auto vector_vector_int_t = type_system.Substitute(vector_vector_tc, {{0, int_t}});
  ASSERT_TRUE(vector_vector_int_t->IsType());
  ASSERT_EQ(vector_vector_int_t->Write(), "object std::vector<object std::vector<int>>");
}

TEST(HigherTypeSystem, TemplateTypes2) {
  GeneralizedTypeSystem type_system;
  auto pair_tc   = type_system.GetObjectType("std::pair", 2);
  auto vector_tc = type_system.GetObjectType("std::vector", 1);
  auto int_t     = type_system.GetBasicType("int");

  ASSERT_EQ(pair_tc->GetNumFreeVars(), 2);
  ASSERT_EQ(vector_tc->GetNumFreeVars(), 1);
  ASSERT_EQ(int_t->GetNumFreeVars(), 0);

  auto pair_vec_t_int = type_system.Substitute(pair_tc, {{0, vector_tc}, {1, int_t}});
  ASSERT_EQ(pair_vec_t_int->GetNumFreeVars(), 1);
  ASSERT_EQ(pair_vec_t_int->Write(), "object std::pair<object std::vector<$0>, int>");

  auto pair_vec_vec_tc = type_system.Substitute(pair_tc, {{0, vector_tc}, {1, vector_tc}});
  ASSERT_EQ(pair_vec_vec_tc->GetNumFreeVars(), 2);
  ASSERT_EQ(pair_vec_vec_tc->Write(), "object std::pair<object std::vector<$0>, object std::vector<$1>>");

  auto pair_vec_int_vec_int_t = type_system.Substitute(pair_vec_vec_tc, {{0, int_t}, {1, int_t}});
  ASSERT_EQ(pair_vec_int_vec_int_t->GetNumFreeVars(), 0);
  ASSERT_EQ(pair_vec_int_vec_int_t->Write(),
            "object std::pair<object std::vector<int>, object std::vector<int>>");
}

TEST(HigherTypeSystem, Function) {
  GeneralizedTypeSystem type_system;
  auto int_t     = type_system.GetBasicType("int");
  auto vector_tc = type_system.GetObjectType("std::vector", 1);
  auto pair_tc   = type_system.GetObjectType("std::pair", 2);

  auto func_int_int_int_t =
      type_system.GetFunctionType({ElaboratedType(int_t), ElaboratedType(int_t), ElaboratedType(int_t)});
  ASSERT_EQ(func_int_int_int_t->Write(), "func int -> int -> int");

  auto func_int_vec_int_tc = type_system.GetFunctionType(
      {ElaboratedType(int_t), ElaboratedType(vector_tc).TypeBind({0}), ElaboratedType(int_t)}, 1);
  ASSERT_EQ(func_int_vec_int_tc->Write(), "func [*1] int -> object std::vector<$0> -> int");

  auto func_pair_pair_tc = type_system.GetFunctionType(
      {ElaboratedType(pair_tc).TypeBind({0, 1}), ElaboratedType(pair_tc).TypeBind({0, 1})}, 2);
  ASSERT_EQ(func_pair_pair_tc->Write(), "func [*2] object std::pair<$0, $1> -> object std::pair<$0, $1>");

  auto func_pair_pair_2_tc = type_system.GetFunctionType({ElaboratedType(pair_tc).TypeBind({0, 1}),
                                                          ElaboratedType(pair_tc).TypeBind({1, 2}),
                                                          ElaboratedType(pair_tc).TypeBind({0, 3})},
                                                         3);
  ASSERT_EQ(func_pair_pair_2_tc->Write(),
            "func [*3] object std::pair<$0, $1> -> object std::pair<$1, $2> -> object std::pair<$0, $3>");
}

TEST(HigherTypeSystem, HigherFunction) {
  GeneralizedTypeSystem type_system;
  auto int_t   = type_system.GetBasicType("int");
  auto float_t = type_system.GetBasicType("float");

  auto func_int_int_t   = type_system.GetFunctionType({ElaboratedType(int_t), ElaboratedType(int_t)});
  auto func_int_float_t = type_system.GetFunctionType({ElaboratedType(int_t), ElaboratedType(float_t)});

  auto higher_func = type_system.GetFunctionType(
      {ElaboratedType(func_int_int_t), ElaboratedType(func_int_float_t), ElaboratedType(func_int_float_t)});
  ASSERT_EQ(higher_func->Write(), "func (func int -> int) -> (func int -> float) -> (func int -> float)");

  auto really_high_func = type_system.GetFunctionType({ElaboratedType(higher_func),
                                                       ElaboratedType(func_int_int_t),
                                                       ElaboratedType(func_int_float_t),
                                                       ElaboratedType(func_int_float_t)});
  ASSERT_EQ(really_high_func->Write(),
            "func (func (func int -> int) -> (func int -> float) -> (func int -> float)) -> (func int -> "
            "int) -> (func int -> float) -> (func int -> float)");
}

TEST(HigherTypeSystem, Enumeration) {
  GeneralizedTypeSystem type_system;
  auto int_t = type_system.GetBasicType("int");

  using namespace std::string_literals;

  auto enum_tc =
      type_system.GetEnumerationType("Colors", {{"RED"s, int_t}, {"GREEN"s, int_t}, {"BLUE"s, int_t}});
  ASSERT_EQ(enum_tc->GetPrototypeType(), PrototypeType::ENUM);
  ASSERT_EQ(enum_tc->Write(), "enum Colors");

  auto proto = dynamic_cast<const EnumeratedPrototype*>(enum_tc->GetPrototype());
  ASSERT_TRUE(proto);
  ASSERT_EQ(proto->GetEnumName(), "Colors");

  auto& options = proto->GetOptions();
  ASSERT_EQ(options.size(), 3);
  ASSERT_EQ(options.at("RED")->Write(), "int");
  ASSERT_EQ(options.at("GREEN")->Write(), "int");
  ASSERT_EQ(options.at("BLUE")->Write(), "int");
}

TEST(HigherTypeSystem, Object) {
  GeneralizedTypeSystem type_system;

  auto my_class_proto = type_system.RegisterObjectPrototype("MyClass");
  ASSERT_TRUE(my_class_proto);
  auto my_class = type_system.GetType(my_class_proto);
  ASSERT_TRUE(my_class);

  auto string_t = type_system.GetBasicType("std::string");

  my_class_proto->AddField("field_1", string_t);
  my_class_proto->AddField("next_node", my_class);
  EXPECT_EQ(my_class_proto->GetFields().size(), 2);

  // Add a constructor.
  EXPECT_NO_THROW(my_class_proto->AddConstructor(
      StructureConstructor {}.WithArguments({{ElaboratedType(string_t), "field"}, {ElaboratedType(my_class), "next"}})));

  // Since 'field_2' is not a field of the class, trying to add a constructor that
  // initializes it should fail.
  EXPECT_ANY_THROW(my_class_proto->AddConstructor(
      StructureConstructor {}.WithAdditionalInitializations({{"field_2", {"nullptr"}}})));

  // TODO: Check that calling parent class constructors fails if the "parent class" is not
  //       actually a parent class.
}

}  // namespace UnitTest