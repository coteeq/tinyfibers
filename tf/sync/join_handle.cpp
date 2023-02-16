#include <tf/sync/join_handle.hpp>

#include <tf/rt/scheduler.hpp>

#include <utility>

namespace tf {

static rt::Fiber* kCompletedFiber = (rt::Fiber*)1;

JoinHandle::JoinHandle(rt::Fiber* fiber)
    : fiber_(fiber) {
  fiber_->SetWatcher(this);
}

JoinHandle::JoinHandle(JoinHandle&& that) {
  fiber_ = std::exchange(that.fiber_, nullptr);
  if (IsAlive()) {
    fiber_->SetWatcher(this);
  }
}

void JoinHandle::Join() {
  WHEELS_ASSERT(Joinable(), "Cannot Join detached fiber");

  if (IsAlive()) {
    waitee_.Park();
  }
  fiber_ = nullptr;  // Detached
}

void JoinHandle::Detach() {
  WHEELS_ASSERT(Joinable(), "Cannot Detach already detached fiber");

  if (IsAlive()) {
    fiber_->SetWatcher(nullptr);
  }
  fiber_ = nullptr;  // Detached
}

JoinHandle::~JoinHandle() {
  WHEELS_VERIFY(IsDetached(), "Explicit Join or Detach required");
}

bool JoinHandle::IsDetached() const {
  return fiber_ == nullptr;
}

bool JoinHandle::IsCompleted() const {
  return fiber_ == kCompletedFiber;
}

bool JoinHandle::IsAlive() const {
  return !IsDetached() && !IsCompleted();
}

bool JoinHandle::Joinable() const {
  return !IsDetached();
}

void JoinHandle::OnCompleted() {
  fiber_ = kCompletedFiber;
  waitee_.Wake();
}

}  // namespace tf
