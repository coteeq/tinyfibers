#include <tinyfibers/run.hpp>
#include <tinyfibers/sched/spawn.hpp>

#include <fmt/core.h>

using namespace tinyfibers;

int main() {
  RunScheduler([]() {
    fmt::print("Hello from parent\n");
    JoinHandle child = Spawn([]() {
      fmt::print("Hello from child\n");
    });
    child.Join();
    fmt::print("Child finished\n");
  });
  return 0;
}
