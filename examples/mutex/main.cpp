#include <tinyfibers/api.hpp>
#include <tinyfibers/sync/mutex.hpp>

#include <iostream>

using namespace tinyfibers;
using namespace std::chrono_literals;

int main() {
  RunScheduler([]() {
    Mutex mutex;

    std::cout << "Starting" << std::endl;

    JoinHandle h2 = Spawn([&mutex]() {
      mutex.Lock();
      {
        // Critical section
        self::SleepFor(3s);
      }
      mutex.Unlock();
    });

    JoinHandle h3 = Spawn([&mutex]() {
      std::cout << "Try to lock mutex from Fiber #" << self::GetId() << std::endl;
      mutex.Lock();  // <-- Blocks for 3s
      {
        std::cout << "Mutex locked by Fiber #" << self::GetId() << std::endl;
      }
      mutex.Unlock();
    });

    h2.Join();
    h3.Join();
  });
  return 0;
}
