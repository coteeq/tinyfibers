#pragma once

#include <functional>

namespace tinyfibers::rt {

using FiberRoutine = std::function<void()>;

}  // namespace tinyfibers::rt
