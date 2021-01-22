#pragma once

#include <wheels/test/test_framework.hpp>

// Run test routine in fiber scheduler

#define TINY_FIBER_TEST(name)       \
  void FiberTestRoutine##name();    \
  SIMPLE_TEST(name) {               \
    tinyfibers::RunScheduler([]() { \
      FiberTestRoutine##name();     \
    });                             \
  }                                 \
  void FiberTestRoutine##name()
