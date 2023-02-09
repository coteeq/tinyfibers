#pragma once

#include <memory>

namespace tf::rt {

struct IFiberWatcher {
  virtual ~IFiberWatcher() = default;
  virtual void OnCompleted() = 0;
};

}  // namespace tf::rt
