#include <tinyfibers/sched/spawn.hpp>
#include <tinyfibers/sched/yield.hpp>
#include <tinyfibers/sched/sleep_for.hpp>
#include <tinyfibers/sched/id.hpp>

#include <tinyfibers/rt/scheduler.hpp>

namespace tinyfibers {

JoinHandle Spawn(std::function<void()> routine) {
  rt::Fiber* fiber = rt::CurrentScheduler()->Spawn(std::move(routine));
  return JoinHandle{fiber};
}

namespace self {

void Yield() {
  rt::CurrentScheduler()->Yield();
}

void SleepFor(std::chrono::milliseconds delay) {
  rt::CurrentScheduler()->SleepFor(delay);
}

FiberId GetId() {
  return rt::CurrentFiber()->Id();
}

}  // namespace self

}  // namespace tinyfibers
