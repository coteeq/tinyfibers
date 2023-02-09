#include <tf/run.hpp>
#include <tf/sched/id.hpp>
#include <tf/sync/mutex.hpp>

#include <fmt/core.h>

using tf::RunScheduler;
using tf::GetId;

int main() {
  tf::RunScheduler([]() {
    fmt::println("Hi, I am Fiber #{}", tf::GetId());
  });
  return 0;
}
