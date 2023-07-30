//
// Created by Nathaniel Rupprecht on 7/30/23.
//

#include <gtest/gtest.h>
// Includes
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

TEST(Utility, Exceptions) {
  std::stringstream stream;
  stream << "Hello";

  auto container = IStreamContainer::FromStream(stream);
  EXPECT_ANY_THROW(GetUntil(container, 'X'));
}

}  // namespace UnitTest