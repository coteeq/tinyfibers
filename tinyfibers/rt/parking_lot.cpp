#include <tinyfibers/rt/parking_lot.hpp>

#include <tinyfibers/rt/scheduler.hpp>

namespace tinyfibers::rt {

void ParkingLot::Park() {
  waitee_ = CurrentFiber();
  CurrentScheduler()->Suspend();
}

void ParkingLot::Wake() {
  if (waitee_ != nullptr) {
    CurrentScheduler()->Resume(waitee_);
  }
}

}  // namespace tinyfibers::rt
