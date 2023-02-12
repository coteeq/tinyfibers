#include <tf/run.hpp>
#include <tf/sched/spawn.hpp>
#include <tf/sched/sleep_for.hpp>
#include <tf/sched/id.hpp>
#include <tf/sync/mutex.hpp>

#include <fmt/core.h>

using namespace std::chrono_literals;

using tf::RunScheduler;
using tf::Spawn;
using tf::SleepFor;
using tf::GetId;
using tf::JoinHandle;
using tf::Mutex;

int main() {
  RunScheduler([] {
    Mutex mutex;

    fmt::println("Starting");

    JoinHandle h2 = Spawn([&mutex] {
      mutex.Lock();
      {
        // Critical section
        SleepFor(3s);
      }
      mutex.Unlock();
    });

    JoinHandle h3 = Spawn([&mutex] {
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
