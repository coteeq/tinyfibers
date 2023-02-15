#include <tf/sync/join_handle.hpp>

#include <tf/rt/scheduler.hpp>

#include <utility>

namespace tf {

JoinHandle::JoinHandle(rt::Fiber* fiber)
    : fiber_(fiber) {
  fiber_->SetWatcher(this);
}

JoinHandle::JoinHandle(JoinHandle&& that) {
  fiber_ = std::exchange(that.fiber_, nullptr);
  completed_ = that.completed_;
  if (!completed_) {
    // Keep watching
    fiber_->SetWatcher(this);
  }
}

void JoinHandle::Join() {
  CheckAttached();

  if (!completed_) {
    // Wake me on completion
    waitee_.Park();
  }

  fiber_ = nullptr;
}

void JoinHandle::Detach() {
  CheckAttached();
  fiber_->SetWatcher(nullptr);
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
  waitee_.Wake();
}

}  // namespace tf
