#pragma once

#include <tinyfibers/rt/fiber.hpp>
#include <tinyfibers/rt/stack_allocator.hpp>

#include <sure/context.hpp>

#include <chrono>

namespace tinyfibers::rt {

// Asymmetric control transfer:
// RunLoop: S -> F_init -> S -> F1 -> S -> F2 -> S -> ...
// 1) S -> F (SwitchToFiber)
// 2) F -> S (SwitchToScheduler)

class Scheduler {
  using FiberRoutine = std::function<void()>;

 public:
  Scheduler();

  // One-shot
  void Run(FiberRoutine init);

  // System calls

  Fiber* Spawn(FiberRoutine routine);
  void Yield();
  // Sleep for _at_least_ `delay`
  void SleepFor(std::chrono::milliseconds delay);
  void Suspend();
  void Resume(Fiber* fiber);
  void Terminate();

  Fiber* GetCurrentFiber();

 private:
  void RunLoop();

  // Context switches
  // Fiber context -> scheduler (thread) context
  void SwitchToScheduler(Fiber* me);
  void ExitToScheduler(Fiber* me);
  // Scheduler context -> fiber context
  void SwitchToFiber(Fiber* fiber);

  // Switch to `fiber` and run it until this fiber calls Yield or terminates
  void Step(Fiber* fiber);
  // ~ Handle system call (Yield / SleepFor / Terminate)
  void Dispatch(Fiber* fiber);
  // Add fiber to run queue
  void Schedule(Fiber* fiber);

  Fiber* CreateFiber(FiberRoutine routine);
  void Destroy(Fiber* fiber);

 private:
  sure::ExecutionContext loop_context_;  // Thread context!
  wheels::IntrusiveList<Fiber> run_queue_;
  Fiber* running_{nullptr};

  // Resources
  size_t next_id_{0};
  StackAllocator stacks_;
};

//////////////////////////////////////////////////////////////////////

Scheduler* CurrentScheduler();
Fiber* CurrentFiber();

}  // namespace tinyfibers::rt
