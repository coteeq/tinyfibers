#include "gtest/gtest.h"

#include <tinyfiber/support/string_builder.hpp>

using namespace tinyfiber;

TEST(StringBuilder, HelloWorld) {
  std::string greeting = StringBuilder() << "Hello" << ", " << "World" << "!";
  ASSERT_EQ(greeting, "Hello, World!");
}
