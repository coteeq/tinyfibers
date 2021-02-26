#include <tinyfibers/runtime/parking_lot.hpp>

#include <tinyfibers/runtime/scheduler.hpp>

namespace tinyfibers {

void ParkingLot::Park() {
  waiter_ = GetCurrentFiber();
  GetCurrentScheduler()->Suspend();
}

void ParkingLot::Wake() {
  if (waiter_) {
    GetCurrentScheduler()->Resume(waiter_);
  }
}

}  // namespace tinyfibers
