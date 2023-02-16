#pragma once

#include <tf/rt/run.hpp>
#include <tf/rt/wait_queue.hpp>
#include <tf/rt/watcher.hpp>

#include <vector>

namespace tf {

class WaitGroup : public rt::IFiberWatcher {
 public:
  WaitGroup& Spawn(rt::FiberRoutine routine);
  void Wait();

  ~WaitGroup();

 private:
  void OnCompleted() override;

 private:
  size_t alive_{0};
  rt::WaitQueue wait_queue_;
};

}  // namespace tf
