#include <tinyfibers/rt/wait_queue.hpp>

#include <tinyfibers/rt/scheduler.hpp>

#include <wheels/core/assert.hpp>

namespace tinyfibers::rt {

void WaitQueue::Park() {
  Fiber* caller = CurrentFiber();
  waiters_.PushBack(caller);
  CurrentScheduler()->Suspend(caller);
}

void WaitQueue::WakeOne() {
  if (waiters_.IsEmpty()) {
    return;
  }
  Resume(waiters_.PopFront());
}

void WaitQueue::WakeAll() {
  while (!waiters_.IsEmpty()) {
    Resume(waiters_.PopFront());
  }
}

void WaitQueue::Resume(Fiber* fiber) {
  CurrentScheduler()->Resume(fiber);
}

WaitQueue::~WaitQueue() {
  WHEELS_ASSERT(waiters_.IsEmpty(), "WaitQueue is not empty");
}

}  // namespace tinyfibers::rt
