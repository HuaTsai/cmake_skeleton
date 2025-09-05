#include <gtest/gtest.h>

#include <myproject/math_utils.hpp>

TEST(MathUtilsTest, AddFunction) {
  EXPECT_EQ(add(2, 3), 5);
  EXPECT_EQ(add(-1, 1), 0);
  EXPECT_EQ(add(-1, -1), -2);
}

TEST(MathUtilsTest, SquareFunction) {
  EXPECT_EQ(square(3), 9);
  EXPECT_EQ(square(-4), 16);
  EXPECT_EQ(square(0), 0);
}