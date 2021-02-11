#pragma once

#include <wheels/support/noncopyable.hpp>

#include <memory>

namespace tinyfibers {

class WaitQueue : private wheels::NonCopyable {
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
