#include <tinyfibers/runtime/wait_queue.hpp>

#include <tinyfibers/runtime/scheduler.hpp>

namespace tiny::fibers {

static inline void Suspend() {
  GetCurrentScheduler()->Suspend();
}

static inline void Resume(Fiber* fiber) {
  GetCurrentScheduler()->Resume(fiber);
}

class WaitQueue::Impl {
 public:
  void Park() {
    Fiber* caller = GetCurrentFiber();
    wait_queue_.PushBack(caller);
    Suspend();
  }

  void WakeOne() {
    if (wait_queue_.IsEmpty()) {
      return;
    }
    Fiber* fiber = wait_queue_.PopFront();
    Resume(fiber);
  }

  void WakeAll() {
    while (!wait_queue_.IsEmpty()) {
      Fiber* fiber = wait_queue_.PopFront();
      Resume(fiber);
    }
  }

  bool IsEmpty() const {
    return wait_queue_.IsEmpty();
  }

 private:
  FiberQueue wait_queue_;
};

WaitQueue::WaitQueue() : pimpl_(std::make_unique<Impl>()) {
}

WaitQueue::~WaitQueue() {
}

void WaitQueue::Park() {
  pimpl_->Park();
}

void WaitQueue::WakeOne() {
  pimpl_->WakeOne();
}

void WaitQueue::WakeAll() {
  pimpl_->WakeAll();
}

bool WaitQueue::IsEmpty() const {
  return pimpl_->IsEmpty();
}

}  // namespace tiny::fibers
