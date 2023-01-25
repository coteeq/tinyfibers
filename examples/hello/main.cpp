#include <tinyfibers/run.hpp>
#include <tinyfibers/sched/spawn.hpp>

#include <fmt/core.h>

using namespace tinyfibers;

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
