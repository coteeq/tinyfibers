#pragma once

#include <functional>

namespace tinyfibers {

// Runs 'init' routine in fiber scheduler in the current thread
void RunScheduler(std::function<void()> init);

}  // namespace tinyfibers
