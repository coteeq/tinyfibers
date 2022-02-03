#include <tinyfibers/api.hpp>
#include <tinyfibers/sync/mutex.hpp>

#include <iostream>

using namespace tinyfibers;

int main() {
  RunScheduler([]() {
    std::cout << "Hi, I am Fiber #" << self::GetId() << std::endl;
  });
  return 0;
}
