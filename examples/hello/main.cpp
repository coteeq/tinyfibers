#include <tinyfibers/runtime/api.hpp>

#include <iostream>

using namespace tinyfibers;

int main() {
  RunScheduler([]() {
    std::cout << "Hello from main!" << std::endl;
    JoinHandle h = Spawn([]() {
      std::cout << "Hello from child!" << std::endl;
      self::Yield();
    });
    h.Join();
  });
  return 0;
}
