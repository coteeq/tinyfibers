#include <tf/rt/scheduler.hpp>

#include <wheels/core/assert.hpp>
#include <wheels/core/panic.hpp>
#include <wheels/core/stop_watch.hpp>

#include <limits>

namespace tf::rt {

//////////////////////////////////////////////////////////////////////

static Scheduler* current_scheduler = nullptr;

Scheduler* Scheduler::Current() {
  WHEELS_VERIFY(current_scheduler, "Not in fiber context");
  return current_scheduler;
}

struct SchedulerScope {
  SchedulerScope(Scheduler* scheduler) {
    WHEELS_VERIFY(!current_scheduler,
                  "Cannot run scheduler from another scheduler");
    current_scheduler = scheduler;
  }

  ~SchedulerScope() {
    current_scheduler = nullptr;
  }
};

//////////////////////////////////////////////////////////////////////

Scheduler::Scheduler() {
  SetDeadlockHandler([]() {
    WHEELS_PANIC("Deadlock detected in fiber scheduler");
  });
}

Fiber* Scheduler::RunningFiber() {
  WHEELS_VERIFY(running_ != nullptr, "Not in fiber context");
  return running_;
}

void Scheduler::SwitchToScheduler() {
  running_->Context().SwitchTo(loop_context_);
}

void Scheduler::ExitToScheduler() {
  running_->Context().ExitTo(loop_context_);
}

// System calls

Fiber* Scheduler::Spawn(FiberRoutine routine) {
  Fiber* newbie = CreateFiber(std::move(routine));
  Schedule(newbie);
  return newbie;
}

void Scheduler::Yield() {
  running_->SetState(FiberState::Runnable);
  SwitchToScheduler();
}

void Scheduler::SleepFor(std::chrono::milliseconds delay) {
  // Intentionally ineffective implementation
  // Support for sleep in scheduler left as homework

  wheels::StopWatch stop_watch;
  do {
    Yield();
  } while (stop_watch.Elapsed() < delay);
}

void Scheduler::Suspend() {
  running_->SetState(FiberState::Suspended);
  SwitchToScheduler();
}

void Scheduler::Resume(Fiber* fiber) {
  WHEELS_ASSERT(fiber->State() == FiberState::Suspended,
                "Expected fiber in Suspended state");
  fiber->SetState(FiberState::Runnable);
  Schedule(fiber);
}

void Scheduler::Terminate() {
  running_->SetState(FiberState::Terminated);
  // Leave this context forever
  ExitToScheduler();
}

// Scheduling

void Scheduler::Run(FiberRoutine init) {
  Run(std::move(init), std::numeric_limits<size_t>::max());
}

void Scheduler::Run(FiberRoutine init, size_t fuel) {
  SchedulerScope scope(this);
  Spawn(std::move(init));
  RunLoop(fuel);
  if (fuel > 0) {
    CheckDeadlock();
  }
}

void Scheduler::RunLoop(size_t& fuel) {
  while (!run_queue_.IsEmpty() && fuel-- > 0) {
    Fiber* next = run_queue_.PopFront();
    Step(next);
    Dispatch(next);  // ~ Handle syscall
  }
}

void Scheduler::Step(Fiber* fiber) {
  running_ = fiber;
  fiber->SetState(FiberState::Running);
  SwitchTo(fiber);
  running_ = nullptr;
}

void Scheduler::SwitchTo(Fiber* fiber) {
  loop_context_.SwitchTo(fiber->Context());
}

void Scheduler::Dispatch(Fiber* fiber) {
  switch (fiber->State()) {
    case FiberState::Runnable:  // From Yield
      Schedule(fiber);
      break;
    case FiberState::Suspended:  // From Suspend
      // Do nothing
      break;
    case FiberState::Terminated:  // From Terminate
      Destroy(fiber);
      break;
    default:
      WHEELS_PANIC("Unexpected fiber state");
      break;
  }
}

void Scheduler::Schedule(Fiber* fiber) {
  run_queue_.PushBack(fiber);
}

Fiber* Scheduler::CreateFiber(FiberRoutine routine) {
  ++alive_count_;

  // Allocate resources
  auto stack = stacks_.Allocate();
  auto id = ids_.Generate();

  // Create fiber
  return new Fiber(this, std::move(routine), std::move(stack), id);
}

void Scheduler::Destroy(Fiber* fiber) {
  --alive_count_;
  stacks_.Release(std::move(fiber->Stack()));
  delete fiber;
}

void Scheduler::SetDeadlockHandler(std::function<void()> handler) {
  deadlock_handler_ = handler;
}

void Scheduler::CheckDeadlock() {
  if (alive_count_ > 0 && run_queue_.IsEmpty()) {
    // Deadlock
    deadlock_handler_();
    WHEELS_PANIC("Deadlock handler returns");
  }
}

}  // namespace tf::rt
