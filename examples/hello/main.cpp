#include <tinyfibers/run.hpp>
#include <tinyfibers/sched/spawn.hpp>

#include <iostream>

using namespace tinyfibers;

int main() {
  RunScheduler([]() {
    std::cout << "Hello from parent!" << std::endl;
    JoinHandle child = Spawn([]() {
      std::cout << "Hello from child!" << std::endl;
    });
    child.Join();
    std::cout << "Child finished" << std::endl;
  });
  return 0;
}
