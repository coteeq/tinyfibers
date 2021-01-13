#pragma once

#include <tinyfibers/runtime/api.hpp>
#include <tinyfibers/runtime/fiber.hpp>

#include <wheels/support/time.hpp>

namespace tiny::fibers {

using FiberQueue = wheels::IntrusiveList<Fiber>;

class Scheduler {
 public:
  Scheduler();

  // One-shot
  void Run(FiberRoutine init);

  // From fiber context

  void Spawn(FiberRoutine routine);
  void Yield();
  // Sleep for _at_least_ delay
  void SleepFor(Duration delay);
  void Suspend();
  void Resume(Fiber* that);
  void Terminate();

  Fiber* GetCurrentFiber();

 private:
  void RunLoop();

  // Context switch: current fiber -> scheduler
  void SwitchToScheduler(Fiber* me);
  // Switch to `fiber` and run it until this fiber calls Yield or terminates
  void Run(Fiber* fiber);
  // ~ Handle system call (Yield / SleepFor / Terminate)
  void Reschedule(Fiber* fiber);
  // Add fiber to run queue
  void Schedule(Fiber* fiber);

  Fiber* CreateFiber(FiberRoutine routine);
  void Destroy(Fiber* fiber);

 private:
  context::ExecutionContext loop_context_;
  FiberQueue run_queue_;
  Fiber* running_{nullptr};
};

//////////////////////////////////////////////////////////////////////

Scheduler* GetCurrentScheduler();
Fiber* GetCurrentFiber();

}  // namespace tiny::fibers
