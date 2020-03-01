#pragma once

#include <cstdlib>

// Unreachable by control flow
#define TINY_UNREACHABLE() std::abort()
