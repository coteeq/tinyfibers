#include <tf/sched/spawn.hpp>
#include <tf/sched/yield.hpp>
#include <tf/sched/sleep_for.hpp>
#include <tf/sched/id.hpp>

#include <tf/rt/scheduler.hpp>

namespace tf {

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

}  // namespace tf
