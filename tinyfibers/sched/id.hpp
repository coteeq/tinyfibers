#pragma once

#include <tinyfibers/rt/id.hpp>

namespace tinyfibers::self {

// Returns the id of the current fiber

FiberId GetId();

}  // namespace tinyfibers::self
