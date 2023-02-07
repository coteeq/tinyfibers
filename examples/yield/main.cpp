#include <tinyfibers/run.hpp>

#include <tinyfibers/sched/spawn.hpp>
#include <tinyfibers/sched/yield.hpp>

#include <fmt/core.h>

using namespace tinyfibers;

int main() {
  RunScheduler([]() {
    // run queue
    JoinHandle h1 = Spawn([]() {
      // <--
      fmt::print("2");
      // run queue: f0
      Yield();  // <--
      fmt::print("4");
    });

    fmt::print("1");
    Yield();  // <--
    // run queue: f1
    fmt::print("3");

    JoinHandle h2 = Spawn([]() {
      fmt::print("5");
    });

    // running = init, run queue: f1, f2

    Yield();  // <-- [execution] context switch

    fmt::print("6");

    h1.Join();
    h2.Join();
  });

  return 0;
}
