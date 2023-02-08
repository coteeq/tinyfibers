#include <tinyfibers/rt/wait_queue.hpp>

#include <tinyfibers/rt/scheduler.hpp>

#include <wheels/core/assert.hpp>

namespace tinyfibers::rt {

void WaitQueue::Park() {
  auto* scheduler = Scheduler::Current();

  Fiber* caller = scheduler->RunningFiber();
  waiters_.PushBack(caller);
  scheduler->Suspend(caller);
}

void WaitQueue::WakeOne() {
  if (waiters_.IsEmpty()) {
    return;
  }
  Scheduler::Current()->Resume(waiters_.PopFront());
}

void WaitQueue::WakeAll() {
  auto* scheduler = Scheduler::Current();

  while (!waiters_.IsEmpty()) {
    scheduler->Resume(waiters_.PopFront());
  }
}

WaitQueue::~WaitQueue() {
  WHEELS_ASSERT(waiters_.IsEmpty(), "WaitQueue is not empty");
}

}  // namespace tinyfibers::rt
