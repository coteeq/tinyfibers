#include <tinyfibers/core/wait_queue.hpp>

#include <tinyfibers/core/scheduler.hpp>

namespace tiny::fibers {

static inline void Suspend() {
  GetCurrentScheduler()->Suspend();
}

static inline void Resume(Fiber* f) {
  GetCurrentScheduler()->Resume(f);
}

class WaitQueue::Impl {
 public:
  void Park() {
    Fiber* self = GetCurrentFiber();
    wait_queue_.PushBack(self);
    Suspend();
  }

  void WakeOne() {
    if (wait_queue_.IsEmpty()) {
      return;
    }
    Fiber* f = wait_queue_.PopFront();
    Resume(f);
  }

  void WakeAll() {
    while (!wait_queue_.IsEmpty()) {
      Fiber* f = wait_queue_.PopFront();
      Resume(f);
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
