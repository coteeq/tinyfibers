#include <tinyfibers/api.hpp>

#include <iostream>

using namespace tinyfibers;

int main() {
  RunScheduler([]() {
    std::cout << "Hello from parent!" << std::endl;
    JoinHandle child = Spawn([]() {
      std::cout << "Hello from child!" << std::endl;
      self::Yield();
    });
    child.Join();
    std::cout << "Child finished" << std::endl;
  });
  return 0;
}
