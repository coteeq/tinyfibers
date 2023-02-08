#include <tinyfibers/sched/spawn.hpp>
#include <tinyfibers/sched/yield.hpp>
#include <tinyfibers/sched/sleep_for.hpp>
#include <tinyfibers/sched/id.hpp>

#include <tinyfibers/rt/scheduler.hpp>

namespace tinyfibers {

JoinHandle Spawn(std::function<void()> routine) {
  rt::Fiber* fiber = rt::Scheduler::Current()->Spawn(std::move(routine));
  return JoinHandle{fiber};
}

void Yield() {
  rt::Scheduler::Current()->Yield();
}

void SleepFor(std::chrono::milliseconds delay) {
  rt::Scheduler::Current()->SleepFor(delay);
}

FiberId GetId() {
  return rt::Scheduler::Current()->RunningFiber()->Id();
}

}  // namespace tinyfibers
