#include <gtest/gtest.h>

int add(int a, int b) { return a + b; }

TEST(AdditionTest, HandlesPositiveNumbers) { EXPECT_EQ(add(2, 3), 5); }
TEST(AdditionTest, HandlesNegativeNumbers) { EXPECT_EQ(add(-2, -3), -5); }

TEST(AdditionTest, HandlesLargeNumbers) {
  EXPECT_EQ(add(1000000, 2000000), 3000000);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}