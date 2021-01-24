#include <tinyfibers/runtime/scheduler.hpp>

#include <tinyfibers/runtime/stacks.hpp>

#include <wheels/support/assert.hpp>
#include <wheels/support/panic.hpp>

#include <limits>

namespace tinyfibers {

//////////////////////////////////////////////////////////////////////

static thread_local Scheduler* current_scheduler;

Scheduler* GetCurrentScheduler() {
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

Fiber* Scheduler::GetCurrentFiber() {
  WHEELS_VERIFY(running_ != nullptr, "Not in fiber context");
  return running_;
}

void Scheduler::SwitchToScheduler(Fiber* me) {
  me->Context().SwitchTo(loop_context_);
}

void Scheduler::SwitchToFiber(Fiber* fiber) {
  loop_context_.SwitchTo(fiber->Context());
}

// System calls

Fiber* Scheduler::Spawn(FiberRoutine routine) {
  Fiber* newbie = CreateFiber(std::move(routine));
  Schedule(newbie);
  return newbie;
}

void Scheduler::Yield() {
  Fiber* caller = GetCurrentFiber();
  caller->SetState(FiberState::Runnable);
  SwitchToScheduler(caller);
}

void Scheduler::SleepFor(Duration delay) {
  // Intentionally ineffective implementation
  // Support for sleep in scheduler left as homework

  wheels::StopWatch stop_watch;
  do {
    Yield();
  } while (stop_watch.Elapsed() < delay);
}

void Scheduler::Suspend() {
  Fiber* caller = GetCurrentFiber();
  caller->SetState(FiberState::Suspended);
  SwitchToScheduler(caller);
}

void Scheduler::Resume(Fiber* fiber) {
  WHEELS_ASSERT(fiber->State() == FiberState::Suspended,
                "Expected fiber in Suspended state");
  fiber->SetState(FiberState::Runnable);
  Schedule(fiber);
}

void Scheduler::Terminate() {
  Fiber* caller = GetCurrentFiber();
  caller->SetState(FiberState::Terminated);

  // Leave this context forever
  caller->Context().ExitTo(loop_context_);
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
    Reschedule(next);  // ~ Handle syscall
  }
}

void Scheduler::Step(Fiber* fiber) {
  running_ = fiber;
  fiber->SetState(FiberState::Running);
  SwitchToFiber(fiber);
  running_ = nullptr;
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
      WHEELS_PANIC("Unexpected fiber state");
      break;
  }
}

void Scheduler::Schedule(Fiber* fiber) {
  run_queue_.PushBack(fiber);
}

Fiber* Scheduler::CreateFiber(FiberRoutine routine) {
  ++alive_count_;
  auto stack = AllocateStack();
  FiberId id = ids_.NextId();
  return new Fiber(std::move(routine), std::move(stack), id);
}

void Scheduler::Destroy(Fiber* fiber) {
  --alive_count_;
  ReleaseStack(std::move(fiber->Stack()));
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

//////////////////////////////////////////////////////////////////////

Fiber* GetCurrentFiber() {
  return GetCurrentScheduler()->GetCurrentFiber();
}

void SetDeadlockHandler(std::function<void()> handler) {
  GetCurrentScheduler()->SetDeadlockHandler(handler);
}

}  // namespace tinyfibers
