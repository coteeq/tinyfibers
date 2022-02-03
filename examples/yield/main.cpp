#include <tinyfibers/api.hpp>

#include <iostream>

using namespace tinyfibers;

int main() {
  RunScheduler([]() {
    JoinHandle h1 = Spawn([]() {
      std::cout << "2" << std::endl;
      self::Yield();
      std::cout << "4" << std::endl;
    });

    std::cout << "1" << std::endl;
    self::Yield();
    std::cout << "3" << std::endl;

    JoinHandle h2 = Spawn([]() {
      std::cout << "5" << std::endl;
    });

    // running = init, run queue: f1, f2

    self::Yield();

    std::cout << "6" << std::endl;

    h1.Join();
    h2.Join();
  });

  return 0;
}
