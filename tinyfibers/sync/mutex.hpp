#pragma once

#include <tinyfibers/runtime/wait_queue.hpp>

#include <wheels/support/assert.hpp>

namespace tinyfibers {

class Mutex {
 public:
  void Lock() {
    while (locked_) {
      wait_queue_.Park();
    }
    locked_ = true;
  }

  bool TryLock() {
    if (!locked_) {
      locked_ = true;
      return true;
    }
    return false;
  }

  void Unlock() {
    WHEELS_VERIFY(locked_, "Unlocking mutex that is not locked!");
    locked_ = false;
    wait_queue_.WakeOne();
  }

  // std::lock_guard / std::unique_lock compatibility
  // BasicLockable concept

  void lock() {  // NOLINT
    Lock();
  }

  void unlock() {  // NOLINT
    Unlock();
  }

 private:
  bool locked_{false};
  WaitQueue wait_queue_;
};

}  // namespace tinyfibers
