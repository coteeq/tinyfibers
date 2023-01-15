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

    fmt::print("Starting\n");

    JoinHandle h2 = Spawn([&mutex]() {
      mutex.Lock();
      {
        // Critical section
        self::SleepFor(3s);
      }
      mutex.Unlock();
    });

    JoinHandle h3 = Spawn([&mutex]() {
      fmt::print("Try to lock mutex from Fiber #{}\n", self::GetId());
      mutex.Lock();  // <-- Blocks for 3s
      {
        fmt::print("Mutex locked by Fiber #{}\n", self::GetId());
      }
      mutex.Unlock();
    });

    h2.Join();
    h3.Join();
  });

  return 0;
}
