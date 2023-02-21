#include <tf/sync/wait_group.hpp>

#include <tf/rt/scheduler.hpp>

#include <wheels/core/assert.hpp>

namespace tf {

WaitGroup& WaitGroup::Spawn(rt::FiberRoutine routine) {
  rt::Fiber* newbie = rt::Scheduler::Current()->Spawn(std::move(routine));
  newbie->SetWatcher(this);
  ++alive_;
  return *this;
}

void WaitGroup::Wait() {
  if (alive_ > 0) {
    wait_queue_.Park();
  }
}

void WaitGroup::OnCompleted() noexcept {
  if (--alive_ == 0) {
    wait_queue_.WakeAll();
  }
}

WaitGroup::~WaitGroup() {
  WHEELS_VERIFY(alive_ == 0, "Wait required");
}

}  // namespace tf
