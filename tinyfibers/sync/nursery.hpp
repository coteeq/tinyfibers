#pragma once

#include <tinyfibers/rt/run.hpp>
#include <tinyfibers/rt/watcher.hpp>
#include <tinyfibers/sync/parking_lot.hpp>

#include <vector>

namespace tinyfibers {

class Nursery : public IFiberWatcher {
 public:
  Nursery& Spawn(std::function<void()> routine);
  void Wait();

  ~Nursery();

 private:
  void OnCompleted() override;

 private:
  size_t active_{0};
  detail::ParkingLot parking_lot_;
};

}  // namespace tinyfibers
