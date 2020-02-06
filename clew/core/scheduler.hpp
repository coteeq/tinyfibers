#pragma once

#include <clew/core/api.hpp>
#include <clew/core/fiber.hpp>
#include <clew/support/time.hpp>

namespace twist {
namespace fiber {

using FiberQueue = IntrusiveList<Fiber>;

class Scheduler {
 public:
  Scheduler();

  void Run(FiberRoutine main);

  // System calls
  // Called in fiber context

  void Spawn(FiberRoutine routine);
  void Yield();
  void SleepFor(Duration duration);
  void Suspend();
  void Resume(Fiber* that);
  void Terminate();

 private:
  void RunLoop();
  void CheckDeadlock();

  // Context switch: current fiber -> scheduler
  void SwitchToScheduler();

  // Context switch: scheduler -> fiber
  void SwitchTo(Fiber* fiber);
  void Reschedule(Fiber* fiber);
  void Schedule(Fiber* fiber);

  Fiber* CreateFiber(FiberRoutine routine);
  void Destroy(Fiber* fiber);

 private:
  ExecutionContext loop_context_;
  FiberQueue run_queue_;
  size_t alive_count_{0};
};

//////////////////////////////////////////////////////////////////////

Fiber* GetCurrentFiber();
Scheduler* GetCurrentScheduler();

}  // namespace fiber
}  // namespace twist
