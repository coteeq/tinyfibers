#pragma once

#include <clew/support/compiler.hpp>

#include <exception>

#define VERIFY(cond, error)  \
  do {                       \
    if (UNLIKELY(!(cond))) { \
      std::terminate();      \
    }                        \
  } while (false);
