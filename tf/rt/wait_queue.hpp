#pragma once

#include <tf/rt/fwd.hpp>

#include <wheels/intrusive/list.hpp>

namespace tf::rt {

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
  wheels::IntrusiveList<Fiber> waiters_;
};

}  // namespace tf::rt
