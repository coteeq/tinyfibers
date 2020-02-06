#pragma once

#include <cstdlib>

// Suppress compiler warnings about unused variable/argument
#define UNUSED(x) \
  do {            \
    (void)(x);    \
  } while (false)

// Unreachable by control flow
#define UNREACHABLE() std::abort()

// https://llvm.org/docs/BranchWeightMetadata.html#builtin-expect
#define LIKELY(cond) __builtin_expect(!!(cond), 1)
#define UNLIKELY(cond) __builtin_expect(!!(cond), 0)

// https://preshing.com/20120625/memory-ordering-at-compile-time/
#define COMPILER_BARRIER() asm volatile("" ::: "memory")
