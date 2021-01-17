#pragma once

#include <tinyfibers/runtime/api.hpp>
#include <tinyfibers/runtime/fiber.hpp>

#include <context/context.hpp>

#include <wheels/support/time.hpp>
#include <wheels/support/id.hpp>

namespace tinyfibers {

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
  void SwitchToFiber(Fiber* fiber);

  // Switch to `fiber` and run it until this fiber calls Yield or terminates
  void Step(Fiber* fiber);
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

  wheels::IdGenerator ids_;
};

//////////////////////////////////////////////////////////////////////

Scheduler* GetCurrentScheduler();
Fiber* GetCurrentFiber();

}  // namespace tinyfibers
