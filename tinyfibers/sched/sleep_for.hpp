#pragma once

#include <chrono>

namespace tinyfibers {

// Blocks the execution of the current fiber for _at_least_
// the specified 'delay'

void SleepFor(std::chrono::milliseconds delay);

}  // namespace tinyfibers
