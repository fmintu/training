#include <gtest/gtest.h>

#include "../../../lib/shape/shape.h"

TEST(ShapeTest, AreaCorrect) {
  EXPECT_EQ(square(1), 1);
  EXPECT_EQ(square(2), 2);
}