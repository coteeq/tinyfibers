#pragma once

#include <tinyfiber/support/string_builder.hpp>

namespace tiny::fiber {
namespace detail {
void Panic(const std::string& error);
}  // namespace detail
}  // namespace tiny::fiber

// Print error message to stderr, then abort
// Usage: PANIC("Internal error: " << e.what());

#define TINY_PANIC(error)                                       \
  do {                                                          \
    tiny::fiber::detail::Panic(StringBuilder() << ": " << error); \
  } while (false)
