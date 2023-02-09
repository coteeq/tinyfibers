#include <tf/rt/parking_lot.hpp>

#include <tf/rt/scheduler.hpp>

namespace tf::rt {

void ParkingLot::Park() {
  auto* scheduler = Scheduler::Current();

  waitee_ = scheduler->RunningFiber();
  scheduler->Suspend();
}

void ParkingLot::Wake() {
  if (waitee_ != nullptr) {
    waitee_->Resume();
  }
}

}  // namespace tf::rt
