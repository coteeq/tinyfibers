#pragma once

#include <functional>

namespace tinyfibers::rt {

void SetDeadlockHandler(std::function<void()> handler);

}  // namespace tinyfibers::rt
