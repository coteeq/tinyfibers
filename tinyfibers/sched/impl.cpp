#include <tinyfibers/sched/spawn.hpp>
#include <tinyfibers/sched/yield.hpp>
#include <tinyfibers/sched/sleep_for.hpp>
#include <tinyfibers/sched/id.hpp>

#include <tinyfibers/rt/scheduler.hpp>

namespace tinyfibers {

JoinHandle Spawn(std::function<void()> routine) {
  Fiber* fiber = GetCurrentScheduler()->Spawn(std::move(routine));
  return JoinHandle{fiber};
}

namespace self {

void Yield() {
  GetCurrentScheduler()->Yield();
}

void SleepFor(std::chrono::milliseconds delay) {
  GetCurrentScheduler()->SleepFor(delay);
}

FiberId GetId() {
  return GetCurrentFiber()->Id();
}

}  // namespace self

}  // namespace tinyfibers
