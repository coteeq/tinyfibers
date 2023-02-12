#include <tf/run.hpp>

#include <tf/sched/sleep_for.hpp>
#include <tf/sync/wait_group.hpp>

#include <fmt/core.h>

#include <chrono>

using tf::RunScheduler;
using tf::SleepFor;
using tf::WaitGroup;

using namespace std::chrono_literals;

std::vector<int> SleepSort(std::vector<int> input) {
  std::vector<int> output;

  WaitGroup wg;

  for (int value : input) {
    wg.Spawn([&, value] {
      SleepFor(1s * value);
      output.push_back(value);
    });
  }

  wg.Wait();

  return output;
}

int main() {
  RunScheduler([]() {
    auto sorted = SleepSort({1, 3, 7, 2, 5, 4, 9, 8, 6});

    fmt::print("Sorted: ");
    for (size_t i = 0; i < sorted.size(); ++i) {
      fmt::print("{}, ", sorted[i]);
    }
    fmt::print("\n");
  });
  return 0;
}
