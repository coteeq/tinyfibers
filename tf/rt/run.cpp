#include <tf/rt/run.hpp>

#include <tf/rt/scheduler.hpp>

namespace tf::rt {

void RunScheduler(std::function<void()> init) {
  Scheduler scheduler;
  scheduler.Run(std::move(init));
}

}  // namespace tf::rt
