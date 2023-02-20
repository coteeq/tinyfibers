#include <tf/rt/parking_lot.hpp>

#include <tf/rt/scheduler.hpp>

#include <utility>

namespace tf::rt {

void ParkingLot::Park() {
  auto* scheduler = Scheduler::Current();

  waitee_ = scheduler->RunningFiber();
  scheduler->Suspend();
}

void ParkingLot::Wake() {
  Fiber* waitee = std::exchange(waitee_, nullptr);

  if (waitee != nullptr) {
    waitee->Resume();
  }
}

}  // namespace tf::rt
