#pragma once

#include <tinyfibers/runtime/api.hpp>
#include <tinyfibers/runtime/fiber.hpp>

#include <context/context.hpp>

#include <wheels/support/time.hpp>
#include <wheels/support/id.hpp>

#include <functional>

namespace tinyfibers {

using FiberQueue = wheels::IntrusiveList<Fiber>;

class Scheduler {
 public:
  Scheduler();

  // One-shot
  void Run(FiberRoutine init);

  // System calls

  void Spawn(FiberRoutine routine);
  void Yield();
  // Sleep for _at_least_ delay
  void SleepFor(Duration delay);
  void Suspend();
  void Resume(Fiber* fiber);
  void Terminate();

  Fiber* GetCurrentFiber();

  void SetDeadlockHandler(std::function<void()> handler);

 private:
  void RunLoop();

  // Context switches
  // Fiber context -> scheduler (thread) context
  void SwitchToScheduler(Fiber* me);
  // Scheduler context -> fiber context
  void SwitchToFiber(Fiber* fiber);

  // Switch to `fiber` and run it until this fiber calls Yield or terminates
  void Step(Fiber* fiber);
  // ~ Handle system call (Yield / SleepFor / Terminate)
  void Reschedule(Fiber* fiber);
  // Add fiber to run queue
  void Schedule(Fiber* fiber);

  Fiber* CreateFiber(FiberRoutine routine);
  void Destroy(Fiber* fiber);

  void CheckDeadlock();

 private:
  context::ExecutionContext loop_context_;
  FiberQueue run_queue_;
  Fiber* running_{nullptr};

  wheels::IdGenerator ids_;

  size_t alive_count_{0};

  std::function<void()> deadlock_handler_;
};

//////////////////////////////////////////////////////////////////////

Scheduler* GetCurrentScheduler();
Fiber* GetCurrentFiber();

}  // namespace tinyfibers
