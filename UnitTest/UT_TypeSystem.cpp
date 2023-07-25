//
// Created by Nathaniel Rupprecht on 5/13/23.
//

#include <gtest/gtest.h>
// Includes
#include "manta/generator/typesystem/CppCodegen.h"

using namespace manta;

namespace UnitTest {

TEST(CppCodegen, Structure) {
  TypeSystem type_system;

  auto my_class = type_system.Structure("MyClass");
  ASSERT_TRUE(my_class);

  my_class->AddField("field_1", type_system.String());
  my_class->AddField("next_node", my_class);
  EXPECT_EQ(my_class->fields.size(), 2);

  // Add a constructor.
  EXPECT_NO_THROW(my_class->AddConstructor(StructureConstructor {}.WithArguments(
      {{type_system.String(), "field"}, {my_class, "next"}})));

  // Since 'field_2' is not a field of the class, trying to add a constructor that
  // initializes it should fail.
  EXPECT_ANY_THROW(my_class->AddConstructor(
      StructureConstructor {}.WithAdditionalInitializations({{"field_2", {"nullptr"}}})));

  // TODO: Check that calling parent class constructors fails if the "parent class" is not
  //  actually a parent class.
}

}  // namespace UnitTest