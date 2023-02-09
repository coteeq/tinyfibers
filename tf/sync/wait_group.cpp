#include <tf/sync/wait_group.hpp>

#include <tf/rt/scheduler.hpp>

#include <wheels/core/assert.hpp>

namespace tf {

WaitGroup& WaitGroup::Spawn(std::function<void()> routine) {
  rt::Fiber* newbie = rt::Scheduler::Current()->Spawn(std::move(routine));
  newbie->SetWatcher(this);
  ++active_;
  return *this;
}

void WaitGroup::Wait() {
  if (active_ > 0) {
    parking_lot_.Park();
  }
}

void WaitGroup::OnCompleted() {
  if (--active_ == 0) {
    // Last fiber
    parking_lot_.Wake();
  }
}

WaitGroup::~WaitGroup() {
  WHEELS_VERIFY(active_ == 0, "Wait required");
}

}  // namespace tf
