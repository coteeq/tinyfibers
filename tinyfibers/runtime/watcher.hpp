#pragma once

namespace tinyfibers {

struct IWatcher {
  virtual ~IWatcher() = default;
  virtual void OnCompleted() = 0;
};

}  // namespace tinyfibers
