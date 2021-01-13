#include <tinyfibers/runtime/api.hpp>

#include <tinyfibers/runtime/scheduler.hpp>

namespace tiny::fibers {

//////////////////////////////////////////////////////////////////////

void RunScheduler(FiberRoutine init) {
  Scheduler scheduler;
  scheduler.Run(std::move(init));
}

//////////////////////////////////////////////////////////////////////

void Spawn(FiberRoutine routine) {
  GetCurrentScheduler()->Spawn(std::move(routine));
}

void Yield() {
  GetCurrentScheduler()->Yield();
}

void SleepFor(Duration delay) {
  GetCurrentScheduler()->SleepFor(delay);
}

FiberId GetFiberId() {
  return GetCurrentFiber()->Id();
}

}  // namespace tiny::fibers
