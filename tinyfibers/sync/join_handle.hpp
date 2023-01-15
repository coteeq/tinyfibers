#pragma once

#include <tinyfibers/rt/watcher.hpp>
#include <tinyfibers/sync/parking_lot.hpp>

namespace tinyfibers {

class Fiber;

// An owned permission to join on a fiber
// ~ https://doc.rust-lang.org/std/thread/struct.JoinHandle.html

class JoinHandle : public IFiberWatcher {
  friend class Fiber;

 public:
  JoinHandle(Fiber* fiber);

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
  Fiber* fiber_;
  bool completed_{false};
  detail::ParkingLot waitee_;
};

}  // namespace tinyfibers
