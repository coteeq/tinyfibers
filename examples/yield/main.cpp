#include <tf/run.hpp>

#include <tf/sched/spawn.hpp>
#include <tf/sched/yield.hpp>

#include <fmt/core.h>

using tf::RunScheduler;
using tf::Spawn;
using tf::Yield;
using tf::JoinHandle;

int main() {
  RunScheduler([]() {
    JoinHandle h1 = Spawn([] {
      fmt::print("->2");
      Yield();
      fmt::print("->4");
    });

    fmt::print("1");
    Yield();
    fmt::print("->3");

    JoinHandle h2 = Spawn([] {
      fmt::print("->5");
    });

    Yield();

    fmt::print("->6");

    h1.Join();
    h2.Join();
  });

  // Output: 1->2->3->4->5

  return 0;
}
