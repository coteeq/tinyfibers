#pragma once

#include <clew/core/wait_queue.hpp>

#include <clew/support/assert.hpp>

namespace clew {
namespace fiber {

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
    CLEW_VERIFY(locked_, "Unlocking mutex that is not locked!");
    locked_ = false;
    if (!wait_queue_.IsEmpty()) {
      wait_queue_.WakeOne();
    }
  }

 private:
  bool locked_{false};
  WaitQueue wait_queue_;
};

}  // namespace fiber
}  // namespace clew
