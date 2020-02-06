#pragma once

#include <clew/support/string_builder.hpp>

namespace clew {
namespace detail {
void Panic(const std::string& error);
}  // namespace detail
}  // namespace clew

// Print error message to stderr, then abort
// Usage: PANIC("Internal error: " << e.what());

#define CLEW_PANIC(error)                                  \
  do {                                                     \
    clew::detail::Panic(StringBuilder() << ": " << error); \
  } while (false)
