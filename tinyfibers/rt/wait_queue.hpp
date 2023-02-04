#pragma once

#include <tinyfibers/rt/fwd.hpp>

#include <wheels/intrusive/list.hpp>

namespace tinyfibers::rt {

// ~ Futex for cooperative single-threaded fibers

class WaitQueue {
 public:
  ~WaitQueue();

  void Park();

  // Move one fiber to scheduler run queue
  void WakeOne();

  // Move all fibers to scheduler run queue
  void WakeAll();

 private:
  void Resume(Fiber* fiber);

 private:
  wheels::IntrusiveList<Fiber> waiters_;
};

}  // namespace tinyfibers::rt
