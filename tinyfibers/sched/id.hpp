#pragma once

#include <tinyfibers/rt/id.hpp>

namespace tinyfibers {

// Returns the id of the current fiber

using rt::FiberId;

FiberId GetId();

}  // namespace tinyfibers
