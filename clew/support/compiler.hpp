#pragma once

#include <cstdlib>

// Suppress compiler warnings about unused variable/argument
#define CLEW_UNUSED(x) \
  do {                 \
    (void)(x);         \
  } while (false)

// Unreachable by control flow
#define CLEW_UNREACHABLE() std::abort()
