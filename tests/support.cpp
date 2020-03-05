#include "gtest/gtest.h"

#include <tinyfiber/support/string_builder.hpp>

using namespace tinyfiber;

TEST(StringBuilder, HelloWorld) {
  std::string greeting = StringBuilder() << "Hello" << ", " << "World" << "!";
  ASSERT_EQ(greeting, "Hello, World!");
}

TEST(StringBuilder, NotOnlyStrings) {
  char chr = 'X';
  int answer = 42;
  bool locked = true;

  std::string result = StringBuilder() << "char = " << chr
      << ", answer = " << answer << ", locked = " << locked;

  // TODO: booleans
  ASSERT_EQ(result, "char = X, answer = 42, locked = 1");
}