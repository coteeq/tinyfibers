#pragma once

#include <tinyfibers/rt/run.hpp>
#include <tinyfibers/rt/watcher.hpp>
#include <tinyfibers/rt/parking_lot.hpp>

#include <vector>

namespace tinyfibers {

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

}  // namespace tinyfibers
