#include <tinyfibers/rt/parking_lot.hpp>

#include <tinyfibers/rt/scheduler.hpp>

namespace tinyfibers::rt {

void ParkingLot::Park() {
  auto* scheduler = Scheduler::Current();
  Fiber* caller = scheduler->RunningFiber();

  waitee_ = caller;
  scheduler->Suspend(caller);
}

void ParkingLot::Wake() {
  if (waitee_ != nullptr) {
    waitee_->Resume();
  }
}

}  // namespace tinyfibers::rt
