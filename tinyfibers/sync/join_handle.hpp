#pragma once

#include <tinyfibers/rt/fwd.hpp>
#include <tinyfibers/rt/watcher.hpp>
#include <tinyfibers/rt/parking_lot.hpp>

namespace tinyfibers {

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

}  // namespace tinyfibers
