#include <tf/run.hpp>

#include <fmt/core.h>

using tf::RunScheduler;

void Recurse(size_t depth) {
  fmt::println("Recurse @ depth {}", depth);
  Recurse(depth + 1);
}

int main() {
  RunScheduler([]() {
    Recurse(0);
  });

  return 0;
}
