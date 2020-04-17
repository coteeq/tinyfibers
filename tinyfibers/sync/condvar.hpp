#pragma once

#include <tinyfibers/core/wait_queue.hpp>
#include <tinyfibers/sync/mutex.hpp>

// std::unique_lock
#include <mutex>

namespace tiny::fiber {

class ConditionVariable {
 public:
  void Wait(Mutex& mutex) {
    mutex.Unlock();
    wait_queue_.Park();
    mutex.Lock();
  }

  void NotifyOne() {
    wait_queue_.WakeOne();
  }

  void NotifyAll() {
    wait_queue_.WakeAll();
  }

  // std::condition_variable interface

  using Lock = std::unique_lock<Mutex>;

  void Wait(Lock& lock) {
    Wait(*lock.mutex());
  }

  template <typename Predicate>
  void Wait(Lock& lock, Predicate predicate) {
    while (!predicate()) {
      Wait(lock);
    }
  }

 private:
  WaitQueue wait_queue_;
};

}  // namespace tiny::fiber
