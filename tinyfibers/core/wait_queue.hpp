#pragma once

#include <memory>

namespace tiny::fibers {

class WaitQueue {
 public:
  WaitQueue();
  ~WaitQueue();

  void Park();

  void WakeOne();
  void WakeAll();
  bool IsEmpty() const;

 private:
  // https://en.cppreference.com/w/cpp/language/pimpl
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

}  // namespace tiny::fibers
