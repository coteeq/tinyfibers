#pragma once

#include <tf/rt/run.hpp>
#include <tf/rt/watcher.hpp>
#include <tf/rt/parking_lot.hpp>

#include <vector>

namespace tf {

class WaitGroup : public rt::IFiberWatcher {
 public:
  WaitGroup& Spawn(std::function<void()> routine);
  void Wait();

  ~WaitGroup();

 private:
  void OnCompleted() override;

 private:
  size_t active_{0};
  rt::ParkingLot parking_lot_;
};

}  // namespace tf
