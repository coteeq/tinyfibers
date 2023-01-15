#include <tinyfibers/run.hpp>

#include <tinyfibers/sched/sleep_for.hpp>
#include <tinyfibers/sync/nursery.hpp>

#include <fmt/core.h>

using namespace tinyfibers;

std::vector<int> SleepSort(std::vector<int> ints) {
  std::vector<int> sorted_ints;

  Nursery nursery;
  for (int value : ints) {
    nursery.Spawn([&, value]() {
      self::SleepFor(std::chrono::seconds(value));
      sorted_ints.push_back(value);
    });
  }
  nursery.Wait();

  return sorted_ints;
}

int main() {
  RunScheduler([]() {
    auto sorted_ints = SleepSort({1, 3, 7, 2, 5, 4, 9, 8, 6});

    fmt::print("Sorted: ");
    for (size_t i = 0; i < sorted_ints.size(); ++i) {
      fmt::print("{}, ", sorted_ints[i]);
    }
    fmt::print("\n");
  });
  return 0;
}
