#include <tinyfibers/runtime/api.hpp>

#include <tinyfibers/runtime/scheduler.hpp>

namespace tinyfibers {

//////////////////////////////////////////////////////////////////////

void RunScheduler(FiberRoutine init) {
  Scheduler scheduler;
  scheduler.Run(std::move(init));
}

//////////////////////////////////////////////////////////////////////

void Spawn(FiberRoutine routine) {
  GetCurrentScheduler()->Spawn(std::move(routine));
}

namespace self {

void Yield() {
  GetCurrentScheduler()->Yield();
}

void SleepFor(Duration delay) {
  GetCurrentScheduler()->SleepFor(delay);
}

FiberId GetId() {
  return GetCurrentFiber()->Id();
}

}  // namespace self

}  // namespace tinyfibers
