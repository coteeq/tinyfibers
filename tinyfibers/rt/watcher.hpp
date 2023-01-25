#pragma once

#include <memory>

namespace tinyfibers::rt {

struct IFiberWatcher {
  virtual ~IFiberWatcher() = default;
  virtual void OnCompleted() = 0;
};

}  // namespace tinyfibers::rt
