#include <tinyfibers/core/api.hpp>

#include <tinyfibers/core/scheduler.hpp>

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

void SleepFor(Duration duration) {
  GetCurrentScheduler()->SleepFor(duration);
}

FiberId GetFiberId() {
  return GetCurrentFiber()->Id();
}

}  // namespace tiny::fibers
