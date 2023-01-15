#include <tinyfibers/run.hpp>

#include <fmt/core.h>

using namespace tinyfibers;

void Recurse(size_t depth) {
  fmt::print("Recurse @ depth {}\n", depth);
  Recurse(depth + 1);
}

int main() {
  RunScheduler([]() {
    Recurse(0);
  });

  return 0;
}
