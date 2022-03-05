#pragma once

#include <wheels/intrusive/list.hpp>

namespace tinyfibers {

// Forward declaration
class Fiber;

namespace detail {

// ~ Futex for cooperative _single-threaded_ fibers

class WaitQueue {
 public:
  ~WaitQueue();

  void Park();

  // Move one fiber to scheduler run queue
  void WakeOne();

  // Move all fibers to scheduler run queue
  void WakeAll();

 private:
  wheels::IntrusiveList<Fiber> waiters_;
};

}  // namespace detail

}  // namespace tinyfibers
