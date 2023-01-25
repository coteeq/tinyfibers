#include <tinyfibers/sched/spawn.hpp>
#include <tinyfibers/sched/yield.hpp>
#include <tinyfibers/sched/sleep_for.hpp>
#include <tinyfibers/sched/id.hpp>

#include <tinyfibers/rt/scheduler.hpp>

namespace tinyfibers {

JoinHandle Spawn(std::function<void()> routine) {
  Fiber* fiber = CurrentScheduler()->Spawn(std::move(routine));
  return JoinHandle{fiber};
}

namespace self {

void Yield() {
  CurrentScheduler()->Yield();
}

void SleepFor(std::chrono::milliseconds delay) {
  CurrentScheduler()->SleepFor(delay);
}

FiberId GetId() {
  return CurrentFiber()->Id();
}

}  // namespace self

}  // namespace tinyfibers
