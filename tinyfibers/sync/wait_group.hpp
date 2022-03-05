#pragma once

#include <tinyfibers/core/api.hpp>
#include <tinyfibers/core/watcher.hpp>

#include <vector>

namespace tinyfibers {

class WaitGroup : public IFiberWatcher {
 public:
  WaitGroup& Spawn(FiberRoutine routine);
  void Wait();

  ~WaitGroup();

 private:
  void OnCompleted() override;

 private:
  size_t active_{0};
  ParkingLot parking_lot_;
};

}  // namespace tinyfibers
