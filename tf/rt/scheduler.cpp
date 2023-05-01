#include <tf/rt/scheduler.hpp>

#include <wheels/core/assert.hpp>
#include <wheels/core/panic.hpp>
#include <wheels/core/stop_watch.hpp>

namespace tf::rt {

//////////////////////////////////////////////////////////////////////

static Scheduler* current_scheduler = nullptr;

Scheduler* Scheduler::Current() {
  WHEELS_VERIFY(current_scheduler, "Not in fiber scheduler");
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
}

Scheduler::Scheduler(IPoller* poller) {
  poller_ = poller;
}

Fiber* Scheduler::RunningFiber() const {
  WHEELS_VERIFY(running_ != nullptr, "Not in fiber context");
  return running_;
}

void Scheduler::SwitchToScheduler(Handler handler) {
  handler_ = handler;
  running_->Context().SwitchTo(loop_context_);
}

void Scheduler::ExitToScheduler(Handler handler) {
  handler_ = handler;
  running_->Context().ExitTo(loop_context_);
}

// System calls

Fiber* Scheduler::Spawn(FiberRoutine routine) {
  Fiber* newbie = CreateFiber(std::move(routine));
  Schedule(newbie);
  return newbie;
}

void Scheduler::Yield() {
  auto schedule = [this](Fiber* me) {
    Schedule(me);
  };
  SwitchToScheduler(/*handler=*/schedule);
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
  auto nop = [](Fiber*) {};
  SwitchToScheduler(/*handler=*/nop);
}

void Scheduler::Resume(Fiber* fiber) {
  Schedule(fiber);
}

void Scheduler::Terminate() {
  auto destroy = [this](Fiber* me) {
    Destroy(me);
  };
  ExitToScheduler(/*handler=*/destroy);  // Leave this execution context forever
}

// Scheduling

void Scheduler::Run(FiberRoutine init) {
  SchedulerScope scope(this);
  Spawn(std::move(init));
  RunLoop();
}

void Scheduler::RunLoop() {
  while (run_queue_.NonEmpty() || (poller_ && poller_->HasPending())) {
    if (run_queue_.NonEmpty()) {
      Fiber* next = run_queue_.PopFront();
      auto handler = Step(next);
      handler(next);  // Dispatch
    }
    if (poller_) {
      if (auto* fiber = poller_->TryPoll(); fiber) {
        Schedule(fiber);
      }
    }
  }
}

Handler Scheduler::Step(Fiber* fiber) {
  running_ = fiber;
  SwitchTo(fiber);
  running_ = nullptr;
  return handler_;
}

void Scheduler::SwitchTo(Fiber* fiber) {
  loop_context_.SwitchTo(fiber->Context());
}

void Scheduler::Schedule(Fiber* fiber) {
  run_queue_.PushBack(fiber);
}

Fiber* Scheduler::CreateFiber(FiberRoutine routine) {
  // Allocate resources
  auto stack = stacks_.Allocate();
  auto id = ids_.Generate();

  return new Fiber(this, std::move(routine), std::move(stack), id);
}

void Scheduler::Destroy(Fiber* fiber) {
  // Release resources
  stacks_.Release(std::move(fiber->Stack()));

  delete fiber;
}

}  // namespace tf::rt
