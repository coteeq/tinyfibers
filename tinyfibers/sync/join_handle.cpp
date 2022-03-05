#include <tinyfibers/sync/join_handle.hpp>

#include <tinyfibers/core/scheduler.hpp>

namespace tinyfibers {

namespace detail {

void JoinState::Join() {
  if (!completed_) {
    // Wake me on completion
    waitee_.Park();
  }
}

void JoinState::OnCompleted() {
  completed_ = true;
  waitee_.Wake();
}

}   // namespace detail

JoinHandle::JoinHandle(Fiber* fiber)
    : state_(std::make_shared<detail::JoinState>()) {
  fiber->SetWatcher(state_);
}

void JoinHandle::Join() {
  state_->Join();
  state_.reset();
}

void JoinHandle::Detach() {
  state_.reset();
}

JoinHandle::~JoinHandle() {
  WHEELS_VERIFY(!state_, "Explicit Join or Detach required");
}

}  // namespace tinyfibers
