#include <tinyfibers/rt/wait_queue.hpp>

#include <tinyfibers/rt/scheduler.hpp>

#include <wheels/core/assert.hpp>

namespace tinyfibers::rt {

void WaitQueue::Park() {
  auto* scheduler = Scheduler::Current();
  Fiber* caller = Scheduler::Current()->RunningFiber();

  waiters_.PushBack(caller);
  scheduler->Suspend(caller);
}

void WaitQueue::WakeOne() {
  if (waiters_.IsEmpty()) {
    return;
  }
  waiters_.PopFront()->Resume();
}

void WaitQueue::WakeAll() {
  while (!waiters_.IsEmpty()) {
    waiters_.PopFront()->Resume();
  }
}

WaitQueue::~WaitQueue() {
  WHEELS_ASSERT(waiters_.IsEmpty(), "WaitQueue is not empty");
}

}  // namespace tinyfibers::rt
