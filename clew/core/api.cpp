#include <clew/core/api.hpp>

#include <clew/core/scheduler.hpp>

namespace clew {
namespace fiber {

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

}  // namespace fiber
}  // namespace clew
