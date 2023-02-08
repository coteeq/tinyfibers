#include <tinyfibers/rt/parking_lot.hpp>

#include <tinyfibers/rt/scheduler.hpp>

namespace tinyfibers::rt {

void ParkingLot::Park() {
  auto* scheduler = Scheduler::Current();

  waitee_ = scheduler->RunningFiber();
  scheduler->Suspend(waitee_);
}

void ParkingLot::Wake() {
  if (waitee_ != nullptr) {
    Scheduler::Current()->Resume(waitee_);
  }
}

}  // namespace tinyfibers::rt
