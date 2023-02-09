#include <tf/run.hpp>
#include <tf/sched/spawn.hpp>

#include <fmt/core.h>

using tf::RunScheduler;
using tf::Spawn;
using tf::JoinHandle;

int main() {
  RunScheduler([]() {
    fmt::println("Parent");
    JoinHandle child = Spawn([]() {
      fmt::println("Child");
    });
    child.Join();
    fmt::println("Child completed");
  });
  return 0;
}
