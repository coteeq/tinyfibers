#include <tinyfiber/core/scheduler.hpp>

#include <tinyfiber/support/compiler.hpp>

namespace tinyfiber {

//////////////////////////////////////////////////////////////////////

static thread_local Fiber* current_fiber;

Fiber* GetCurrentFiber() {
  return current_fiber;
}

static inline Fiber* GetAndResetCurrentFiber() {
  auto* f = current_fiber;
  current_fiber = nullptr;
  return f;
}

static inline void SetCurrentFiber(Fiber* f) {
  current_fiber = f;
}

//////////////////////////////////////////////////////////////////////

static thread_local Scheduler* current_scheduler;

Scheduler* GetCurrentScheduler() {
  TINY_VERIFY(current_scheduler, "not in fiber context");
  return current_scheduler;
}

struct SchedulerScope {
  SchedulerScope(Scheduler* scheduler) {
    TINY_VERIFY(!current_scheduler,
                "cannot run scheduler from another scheduler");
    current_scheduler = scheduler;
  }

  ~SchedulerScope() {
    current_scheduler = nullptr;
  }
};

//////////////////////////////////////////////////////////////////////

Scheduler::Scheduler() {
}

// Operations invoked by running fibers

void Scheduler::SwitchToScheduler() {
  Fiber* caller = GetAndResetCurrentFiber();
  caller->Context().SwitchTo(loop_context_);
}

// System calls

void Scheduler::Spawn(FiberRoutine routine) {
  auto* created = CreateFiber(routine);
  Schedule(created);
}

void Scheduler::Yield() {
  Fiber* caller = GetCurrentFiber();
  caller->SetState(FiberState::Runnable);
  SwitchToScheduler();
}

void Scheduler::SleepFor(Duration duration) {
  // Intentionally ineffective implementation
  // Support for sleep in scheduler left as homework

  Timer timer;
  do {
    Yield();
  } while (timer.Elapsed() < duration);
}

void Scheduler::Suspend() {
  Fiber* caller = GetCurrentFiber();
  caller->SetState(FiberState::Suspended);
  SwitchToScheduler();
}

void Scheduler::Resume(Fiber* that) {
  that->SetState(FiberState::Runnable);
  Schedule(that);
}

void Scheduler::Terminate() {
  Fiber* caller = GetCurrentFiber();
  caller->SetState(FiberState::Terminated);
  SwitchToScheduler();
}

// Scheduling

void Scheduler::Run(FiberRoutine init) {
  SchedulerScope scope(this);
  Spawn(init);
  RunLoop();
  CheckDeadlock();
}

void Scheduler::RunLoop() {
  while (!run_queue_.IsEmpty()) {
    Fiber* next = run_queue_.PopFront();
    SwitchTo(next);
    Reschedule(next);
  }
}

void Scheduler::CheckDeadlock() {
  if (run_queue_.IsEmpty() && alive_count_ > 0) {
    throw DeadlockDetected();
  }
}

void Scheduler::SwitchTo(Fiber* fiber) {
  SetCurrentFiber(fiber);
  fiber->SetState(FiberState::Running);
  // Scheduler loop_context_ -> fiber->context_
  loop_context_.SwitchTo(fiber->Context());
}

void Scheduler::Reschedule(Fiber* fiber) {
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
      TINY_PANIC("Unexpected fiber state");
      break;
  }
}

void Scheduler::Schedule(Fiber* fiber) {
  run_queue_.PushBack(fiber);
}

Fiber* Scheduler::CreateFiber(FiberRoutine routine) {
  ++alive_count_;
  return Fiber::Create(routine);
}

void Scheduler::Destroy(Fiber* fiber) {
  --alive_count_;
  delete fiber;
}

}  // namespace tinyfiber
