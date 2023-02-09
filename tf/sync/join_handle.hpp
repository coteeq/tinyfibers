#pragma once

#include <tf/rt/fwd.hpp>
#include <tf/rt/watcher.hpp>
#include <tf/rt/parking_lot.hpp>

namespace tf {

// An owned permission to join on a fiber
// ~ https://doc.rust-lang.org/std/thread/struct.JoinHandle.html

class JoinHandle : public rt::IFiberWatcher {
  friend class Fiber;

 public:
  JoinHandle(rt::Fiber* fiber);

  // Movable
  JoinHandle(JoinHandle&& that);

  // Non-copyable
  JoinHandle(const JoinHandle& that) = delete;
  JoinHandle& operator=(const JoinHandle& that) = delete;

  // Mutually exclusive, one-shot
  void Join();
  void Detach();

  ~JoinHandle();

 private:
  void CheckAttached();
  void OnCompleted() override;

 private:
  rt::Fiber* fiber_;
  bool completed_{false};
  rt::ParkingLot waitee_;
};

}  // namespace tf
