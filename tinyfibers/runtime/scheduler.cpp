#include <tinyfibers/runtime/scheduler.hpp>

#include <wheels/support/assert.hpp>
#include <wheels/support/panic.hpp>

namespace tiny::fibers {

//////////////////////////////////////////////////////////////////////

static thread_local Scheduler* current_scheduler;

Scheduler* GetCurrentScheduler() {
  WHEELS_VERIFY(current_scheduler, "not in fiber context");
  return current_scheduler;
}

struct SchedulerScope {
  SchedulerScope(Scheduler* scheduler) {
    WHEELS_VERIFY(!current_scheduler,
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

Fiber* Scheduler::GetCurrentFiber() {
  WHEELS_VERIFY(running_ != nullptr, "Not in fiber context");
  return running_;
}

// Operations invoked by running fibers

void Scheduler::SwitchToScheduler(Fiber* me) {
  me->Context().SwitchTo(loop_context_);
}

// System calls

void Scheduler::Spawn(FiberRoutine routine) {
  auto* created = CreateFiber(std::move(routine));
  Schedule(created);
}

void Scheduler::Yield() {
  Fiber* caller = GetCurrentFiber();
  caller->SetState(FiberState::Runnable);
  SwitchToScheduler(caller);
}

void Scheduler::SleepFor(Duration duration) {
  // Intentionally ineffective implementation
  // Support for sleep in scheduler left as homework

  wheels::StopWatch stop_watch;
  do {
    Yield();
  } while (stop_watch.Elapsed() < duration);
}

void Scheduler::Suspend() {
  Fiber* caller = GetCurrentFiber();
  caller->SetState(FiberState::Suspended);
  SwitchToScheduler(caller);
}

void Scheduler::Resume(Fiber* that) {
  that->SetState(FiberState::Runnable);
  Schedule(that);
}

void Scheduler::Terminate() {
  Fiber* caller = GetCurrentFiber();
  caller->SetState(FiberState::Terminated);
  SwitchToScheduler(caller);
}

// Scheduling

void Scheduler::Run(FiberRoutine init) {
  SchedulerScope scope(this);
  Spawn(std::move(init));
  RunLoop();
}

void Scheduler::RunLoop() {
  while (!run_queue_.IsEmpty()) {
    Fiber* next = run_queue_.PopFront();
    Run(next);
    Reschedule(next);
  }
}

void Scheduler::Run(Fiber* fiber) {
  running_ = fiber;
  fiber->SetState(FiberState::Running);
  // Scheduler loop_context_ -> fiber->context_
  loop_context_.SwitchTo(fiber->Context());
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
  return Fiber::Create(std::move(routine));
}

void Scheduler::Destroy(Fiber* fiber) {
  delete fiber;
}

//////////////////////////////////////////////////////////////////////

Fiber* GetCurrentFiber() {
  return GetCurrentScheduler()->GetCurrentFiber();
}

}  // namespace tiny::fibers
