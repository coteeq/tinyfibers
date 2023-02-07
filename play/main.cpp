#include <tinyfibers/run.hpp>
#include <tinyfibers/sched/id.hpp>
#include <tinyfibers/sync/mutex.hpp>

#include <fmt/core.h>

using namespace tinyfibers;

int main() {
  RunScheduler([]() {
    fmt::println("Hi, I am Fiber #{}", GetId());
  });
  return 0;
}
