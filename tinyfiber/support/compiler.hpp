#pragma once

#include <cstdlib>

// Suppress compiler warnings about unused variable/argument
#define TINY_UNUSED(x) \
  do {                 \
    (void)(x);         \
  } while (false)

// Unreachable by control flow
#define TINY_UNREACHABLE() std::abort()
