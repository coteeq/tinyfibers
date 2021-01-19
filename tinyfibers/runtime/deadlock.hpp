#pragma once

#include <functional>

namespace tinyfibers {

void SetDeadlockHandler(std::function<void()> handler);

}  // namespace tinyfibers
