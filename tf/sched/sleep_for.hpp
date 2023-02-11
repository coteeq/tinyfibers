#pragma once

#include <chrono>

namespace tf {

// Suspends this fiber for _at_least_ 'delay' milliseconds

void SleepFor(std::chrono::milliseconds delay);

}  // namespace tf
