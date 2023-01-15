#include <tinyfibers/run.hpp>

#include <tinyfibers/sched/spawn.hpp>
#include <tinyfibers/sched/yield.hpp>

#include <iostream>

using namespace tinyfibers;

int main() {
  RunScheduler([]() {
    // run queue
    JoinHandle h1 = Spawn([]() {
      // <--
      std::cout << "2" << std::endl;
      // run queue: f0
      self::Yield();  // <--
      std::cout << "4" << std::endl;
    });

    std::cout << "1" << std::endl;
    self::Yield();  // <--
    // run queue: f1
    std::cout << "3" << std::endl;

    JoinHandle h2 = Spawn([]() {
      std::cout << "5" << std::endl;
    });

    // running = init, run queue: f1, f2

    self::Yield();  // <-- [execution] context switch

    std::cout << "6" << std::endl;

    h1.Join();
    h2.Join();
  });

  return 0;
}
