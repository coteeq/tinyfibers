#include <tinyfibers/rt/run.hpp>

#include <tinyfibers/rt/scheduler.hpp>

namespace tinyfibers::rt {

void RunScheduler(std::function<void()> init) {
  Scheduler scheduler;
  scheduler.Run(std::move(init));
}

}  // namespace tinyfibers::rt
