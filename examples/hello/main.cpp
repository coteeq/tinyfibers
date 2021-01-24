#include <tinyfibers/runtime/api.hpp>

#include <iostream>

using namespace tinyfibers;

int main() {
  RunScheduler([]() {
    std::cout << "Hello from main!" << std::endl;
    JoinHandle child = Spawn([]() {
      std::cout << "Hello from child!" << std::endl;
      self::Yield();
    });
    child.Join();
  });
  return 0;
}
