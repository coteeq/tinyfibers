#include <tinyfibers/sync/wait_group.hpp>

#include <tinyfibers/core/scheduler.hpp>

#include <wheels/support/assert.hpp>

namespace tinyfibers {

WaitGroup& WaitGroup::Spawn(FiberRoutine routine) {
  Fiber* newbie = GetCurrentScheduler()->Spawn(std::move(routine));
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

}  // namespace tinyfibers
