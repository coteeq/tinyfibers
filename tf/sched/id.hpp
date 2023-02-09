#pragma once

#include <tf/rt/id.hpp>

namespace tf {

// Returns the id of the current fiber

using rt::FiberId;

FiberId GetId();

}  // namespace tf
