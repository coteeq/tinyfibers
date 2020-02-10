#include <tinyfiber/core/api.hpp>

#include <tinyfiber/core/scheduler.hpp>

namespace tinyfiber {

//////////////////////////////////////////////////////////////////////

void RunScheduler(FiberRoutine main) {
  Scheduler scheduler;
  scheduler.Run(main);
}

//////////////////////////////////////////////////////////////////////

void Spawn(FiberRoutine routine) {
  GetCurrentScheduler()->Spawn(routine);
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

}  // namespace tinyfiber
