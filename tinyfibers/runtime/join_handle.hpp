#pragma once

#include <tinyfibers/runtime/watcher.hpp>
#include <tinyfibers/runtime/parking_lot.hpp>

namespace tinyfibers {

class Fiber;

// An owned permission to join on a fiber
// ~ https://doc.rust-lang.org/std/thread/struct.JoinHandle.html

namespace detail {

class JoinState : public IFiberWatcher {
 public:
  // Client
  void Join();

  // Fiber
  void OnCompleted() override;

 private:
  bool completed_{false};
  ParkingLot waitee_;
};

}  // namespace detail

class JoinHandle {
 public:
  JoinHandle(Fiber* fiber);

  // Movable
  JoinHandle(JoinHandle&& that) = default;
  JoinHandle& operator=(JoinHandle&& that) = default;

  // Non-copyable
  JoinHandle(const JoinHandle& that) = delete;
  JoinHandle& operator=(const JoinHandle& that) = delete;

  // Mutually exclusive, one-shot
  void Join();
  void Detach();

  ~JoinHandle();

 private:
  std::shared_ptr<detail::JoinState> state_;
};

}  // namespace tinyfibers
