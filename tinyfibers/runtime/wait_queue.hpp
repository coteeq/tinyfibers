#pragma once

#include <memory>

namespace tinyfibers {

class WaitQueue {
 public:
  WaitQueue();
  ~WaitQueue();

  void Park();

  void WakeOne();
  void WakeAll();

 private:
  // https://en.cppreference.com/w/cpp/language/pimpl
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

}  // namespace tinyfibers
