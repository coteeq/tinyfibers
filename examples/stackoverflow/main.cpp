#include <tinyfibers/api.hpp>

#include <iostream>

using namespace tinyfibers;

void Recurse(size_t depth) {
  std::cout << "Recurse @ depth = " << depth << std::endl;
  Recurse(depth + 1);
}

int main() {
  RunScheduler([]() {
    Recurse(0);
  });
  return 0;
}
