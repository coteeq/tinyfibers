#pragma once

#include <tinyfibers/core/api.hpp>

#include <vector>

namespace tinyfibers {

class WaitGroup {
 public:
  WaitGroup& Spawn(FiberRoutine routine);
  void Wait();

  ~WaitGroup();

 private:
  std::vector<JoinHandle> join_handles_;
};

}  // namespace tinyfibers
