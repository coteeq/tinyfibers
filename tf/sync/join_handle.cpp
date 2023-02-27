#include <tf/sync/join_handle.hpp>

#include <tf/rt/scheduler.hpp>

#include <wheels/core/assert.hpp>

#include <utility>

namespace tf {

static rt::Fiber* const kDetached = nullptr;
static rt::Fiber* const kCompleted = (rt::Fiber*)1;

JoinHandle::JoinHandle(rt::Fiber* fiber)
    : fiber_(fiber) {
  fiber_->SetWatcher(this);
}

JoinHandle::JoinHandle(JoinHandle&& that) {
  fiber_ = std::exchange(that.fiber_, kDetached);
  if (IsRunning()) {
    fiber_->SetWatcher(this);
  }
}

void JoinHandle::Join() {
  WHEELS_ASSERT(Joinable(), "Cannot Join detached fiber");

  if (IsRunning()) {
    waitee_.Park();
  }
  fiber_ = kDetached;
}

void JoinHandle::Detach() {
  WHEELS_ASSERT(Joinable(), "Cannot Detach already detached fiber");

  if (IsRunning()) {
    fiber_->SetWatcher(nullptr);
  }
  fiber_ = kDetached;
}

JoinHandle::~JoinHandle() {
  WHEELS_VERIFY(IsDetached(), "Explicit Join or Detach required");
}

bool JoinHandle::IsDetached() const {
  return fiber_ == kDetached;
}

bool JoinHandle::IsCompleted() const {
  return fiber_ == kCompleted;
}

bool JoinHandle::IsRunning() const {
  return !IsDetached() && !IsCompleted();
}

bool JoinHandle::Joinable() const {
  return !IsDetached();
}

void JoinHandle::OnCompleted() noexcept {
  fiber_ = kCompleted;
  waitee_.Wake();
}

}  // namespace tf
