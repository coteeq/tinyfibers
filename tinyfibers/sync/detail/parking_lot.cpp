#include <tinyfibers/sync/detail/parking_lot.hpp>

#include <tinyfibers/rt/scheduler.hpp>

namespace tinyfibers {

namespace detail {

void ParkingLot::Park() {
  waitee_ = GetCurrentFiber();
  GetCurrentScheduler()->Suspend();
}

void ParkingLot::Wake() {
  if (waitee_ != nullptr) {
    GetCurrentScheduler()->Resume(waitee_);
  }
}

}  // namespace detail

}  // namespace tinyfibers
