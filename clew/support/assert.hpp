#pragma once

#include <clew/support/panic.hpp>

#include <iostream>
#include <exception>

#define CLEW_VERIFY(cond, error)                                   \
  do {                                                             \
    if (!(cond)) {                                                 \
      CLEW_PANIC("Assertion '" << #cond << "' failed: " << error); \
    }                                                              \
  } while (false);
