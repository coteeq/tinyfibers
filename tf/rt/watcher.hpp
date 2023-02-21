#pragma once

#include <memory>

namespace tf::rt {

struct IFiberWatcher {
  virtual ~IFiberWatcher() = default;
  virtual void OnCompleted() noexcept = 0;
};

}  // namespace tf::rt
