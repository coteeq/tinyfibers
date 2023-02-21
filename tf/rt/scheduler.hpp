#pragma once

#include <tf/rt/fiber.hpp>
#include <tf/rt/id_generator.hpp>
#include <tf/rt/stack_allocator.hpp>

#include <sure/context.hpp>

#include <chrono>
#include <functional>

namespace tf::rt {

// Asymmetric control transfer:
// RunLoop: S -> F_init -> S -> F1 -> S -> F2 -> S -> ...
// 1) S -> F (SwitchTo(Fiber*))
// 2) F -> S (SwitchToScheduler)

class Scheduler {
 public:
  Scheduler();

  // One-shot
  void Run(FiberRoutine init);
  void Run(FiberRoutine init, size_t fuel);

  static Scheduler* Current();

  // System calls

  Fiber* Spawn(FiberRoutine);
  void Yield();
  // Sleep for _at_least_ `delay`
  void SleepFor(std::chrono::milliseconds delay);
  void Suspend();
  void Resume(Fiber*);
  void Terminate();

  Fiber* RunningFiber() const;

  void SetDeadlockHandler(std::function<void()> handler);

 private:
  // `fuel` limits number of iterations
  void RunLoop(size_t& fuel);

  // Context switches

  // Scheduler (loop) context -> fiber context
  void SwitchTo(Fiber*);
  // Fiber context -> scheduler (loop) context
  void SwitchToScheduler();
  void ExitToScheduler();

  // Switch to fiber and run it until this fiber calls Yield or terminates
  void Step(Fiber*);
  // ~ Handle system call (Yield / SleepFor / Terminate)
  void Dispatch(Fiber*);
  // Add fiber to run queue
  void Schedule(Fiber*);

  Fiber* CreateFiber(FiberRoutine);
  void Destroy(Fiber*);

  void CheckDeadlock();

 private:
  sure::ExecutionContext loop_context_;  // Thread context
  wheels::IntrusiveList<Fiber> run_queue_;
  Fiber* running_{nullptr};

  // Resources
  IdGenerator ids_;
  StackAllocator stacks_;

  size_t alive_count_{0};
  std::function<void()> deadlock_handler_;
};

}  // namespace tf::rt
