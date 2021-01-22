#include <tinyfibers/runtime/join_handle.hpp>

#include <tinyfibers/runtime/scheduler.hpp>

namespace tinyfibers {

JoinHandle::JoinHandle(Fiber* fiber) : fiber_(fiber) {
  fiber_->SetWatcher(this);
}

JoinHandle::JoinHandle(JoinHandle&& that) {
  fiber_ = std::exchange(that.fiber_, nullptr);
  fiber_->SetWatcher(this);
  completed_ = that.completed_;
}

void JoinHandle::Join() {
  CheckAttached();

  if (!completed_) {
    // Wake me on completion
    waiter_.Park();
  }

  fiber_ = nullptr;
}

void JoinHandle::Detach() {
  CheckAttached();
  fiber_ = nullptr;
}

JoinHandle::~JoinHandle() {
  WHEELS_VERIFY(fiber_ == nullptr, "Explicit Join or Detach required");
}

void JoinHandle::CheckAttached() {
  WHEELS_VERIFY(fiber_, "Detached JoinHandle");
}

void JoinHandle::OnCompleted() {
  completed_ = true;
  waiter_.Wake();
}

}  // namespace tinyfibers