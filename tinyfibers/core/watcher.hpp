#pragma once

#include <memory>

namespace tinyfibers {

struct IFiberWatcher {
  virtual ~IFiberWatcher() = default;
  virtual void OnCompleted() = 0;
};

using IFiberWatcherPtr = std::shared_ptr<IFiberWatcher>;

}  // namespace tinyfibers
