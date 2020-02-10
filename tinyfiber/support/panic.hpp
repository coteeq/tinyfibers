#pragma once

#include <tinyfiber/support/string_builder.hpp>

namespace tinyfiber {
namespace detail {
void Panic(const std::string& error);
}  // namespace detail
}  // namespace tinyfiber

// Print error message to stderr, then abort
// Usage: PANIC("Internal error: " << e.what());

#define TINY_PANIC(error)                                       \
  do {                                                          \
    tinyfiber::detail::Panic(StringBuilder() << ": " << error); \
  } while (false)
