#pragma once

#include <tf/rt/wait_queue.hpp>

#include <wheels/core/assert.hpp>

#include <utility>

namespace tf {

class Mutex {
 public:
  void Lock() {
    while (locked_) {
      wait_queue_.Park();
    }
    locked_ = true;
  }

  bool TryLock() {
    return !std::exchange(locked_, true);
  }

  void Unlock() {
    WHEELS_VERIFY(locked_, "Unlocking mutex that is not locked!");
    locked_ = false;
    wait_queue_.WakeOne();
  }

  // std::lock_guard / std::unique_lock compatibility
  // Lockable concept

  void lock() {  // NOLINT
    Lock();
  }

  bool try_lock() {  // NOLINT
    return TryLock();
  }

  void unlock() {  // NOLINT
    Unlock();
  }

 private:
  bool locked_{false};
  rt::WaitQueue wait_queue_;
};

}  // namespace tf
