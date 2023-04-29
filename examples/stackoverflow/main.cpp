#include <tf/run.hpp>

#include <fmt/core.h>

using tf::RunScheduler;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winfinite-recursion"
void Recurse(size_t depth) {
  fmt::println("Recurse @ depth {}", depth);
  Recurse(depth + 1);
}
#pragma clang diagnostic pop

int main() {
  RunScheduler([] {
    Recurse(0);
  });

  return 0;
}
