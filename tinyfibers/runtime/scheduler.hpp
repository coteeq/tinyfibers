#pragma once

#include <tinyfibers/runtime/api.hpp>
#include <tinyfibers/runtime/fiber.hpp>
#include <tinyfibers/runtime/stacks.hpp>

#include <context/context.hpp>

#include <chrono>

#include <functional>

namespace tinyfibers {

// Asymmetric control transfer:
// RunLoop: S -> F_init -> S -> F1 -> S -> F2 -> S -> ...
// 1) S -> F (SwitchToFiber)
// 2) F -> S (SwitchToScheduler)

class Scheduler {
 public:
  Scheduler();

  // One-shot
  void Run(FiberRoutine init);
  void Run(FiberRoutine init, size_t fuel);

  // System calls

  Fiber* Spawn(FiberRoutine routine);
  void Yield();
  // Sleep for _at_least_ delay
  void SleepFor(std::chrono::milliseconds delay);
  void Suspend();
  void Resume(Fiber* fiber);
  void Terminate();

  Fiber* GetCurrentFiber();

  void SetDeadlockHandler(std::function<void()> handler);

 private:
  // `fuel` limits number of iterations
  void RunLoop(size_t& fuel);

  // Context switches
  // Fiber context -> scheduler (thread) context
  void SwitchToScheduler(Fiber* me);
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

  void CheckDeadlock();

 private:
  context::ExecutionContext loop_context_;  // Thread context!
  wheels::IntrusiveList<Fiber> run_queue_;
  Fiber* running_{nullptr};

  // Resources
  size_t next_id_{0};
  StackAllocator stacks_;

  size_t alive_count_{0};
  std::function<void()> deadlock_handler_;
};

//////////////////////////////////////////////////////////////////////

Scheduler* GetCurrentScheduler();
Fiber* GetCurrentFiber();

}  // namespace tinyfibers
