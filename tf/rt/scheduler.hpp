#pragma once

#include <tf/rt/fiber.hpp>
#include <tf/rt/id_generator.hpp>
#include <tf/rt/poller.hpp>
#include <tf/rt/stack_allocator.hpp>
#include <tf/rt/syscall.hpp>

#include <sure/context.hpp>

#include <chrono>

namespace tf::rt {

// Asymmetric control transfer:
// RunLoop: S -> F_init -> S -> F1 -> S -> F2 -> S -> ...
// 1) S -> F (SwitchTo(Fiber*))
// 2) F -> S (SwitchToScheduler)

class Scheduler {
 public:
  Scheduler();
  explicit Scheduler(IPoller* poller);

  // One-shot
  void Run(FiberRoutine init);

  static Scheduler* Current();

  // System calls

  Fiber* Spawn(FiberRoutine);
  void Yield();
  // Sleep for _at_least_ `delay`
  void SleepFor(std::chrono::milliseconds delay);
  void Suspend();
  void Resume(Fiber*);
  [[noreturn]] void Terminate();

  Fiber* RunningFiber() const;

 private:
  void RunLoop();

  // Context switches

  // Scheduler (loop) context -> fiber context
  void SwitchTo(Fiber*);
  // Fiber context -> scheduler (loop) context
  void SwitchToScheduler(Handler);
  [[noreturn]] void ExitToScheduler(Handler);

  // Switch to fiber and run it until this fiber returns control via system call
  Handler Step(Fiber*);
  // Add fiber to run queue
  void Schedule(Fiber*);

  Fiber* CreateFiber(FiberRoutine);
  void Destroy(Fiber*);

 private:
  sure::ExecutionContext loop_context_;  // Thread context
  wheels::IntrusiveList<Fiber> run_queue_;
  Fiber* running_{nullptr};
  Handler handler_;

  IPoller* poller_{nullptr};

  // Resources
  IdGenerator ids_;
  StackAllocator stacks_;
};

}  // namespace tf::rt
