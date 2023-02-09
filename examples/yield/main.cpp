#include <tinyfibers/run.hpp>

#include <tinyfibers/sched/spawn.hpp>
#include <tinyfibers/sched/yield.hpp>

#include <fmt/core.h>

using namespace tinyfibers;

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
