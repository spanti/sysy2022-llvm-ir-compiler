#include <gtest/gtest.h>
#include "errors.h"
#include "commom.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(parse_int32_literal("0x10"), 16);
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
