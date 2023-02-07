#include <tinyfibers/run.hpp>
#include <tinyfibers/sched/spawn.hpp>
#include <tinyfibers/sched/sleep_for.hpp>
#include <tinyfibers/sched/id.hpp>
#include <tinyfibers/sync/mutex.hpp>

#include <fmt/core.h>

using namespace tinyfibers;
using namespace std::chrono_literals;

int main() {
  RunScheduler([]() {
    Mutex mutex;

    fmt::println("Starting");

    JoinHandle h2 = Spawn([&mutex]() {
      mutex.Lock();
      {
        // Critical section
        SleepFor(3s);
      }
      mutex.Unlock();
    });

    JoinHandle h3 = Spawn([&mutex]() {
      fmt::println("Try to lock mutex from Fiber #{}", GetId());
      mutex.Lock();  // <-- Blocks for 3s
      {
        fmt::println("Mutex locked by Fiber #{}", GetId());
      }
      mutex.Unlock();
    });

    h2.Join();
    h3.Join();
  });

  return 0;
}
