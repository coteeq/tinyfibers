#include <tinyfibers/api.hpp>

#include <tinyfibers/sync/wait_group.hpp>

#include <iostream>

using namespace tinyfibers;

std::vector<int> SleepSort(std::vector<int> ints) {
  std::vector<int> sorted_ints;

  WaitGroup wg;
  for (int value : ints) {
    wg.Spawn([&, value]() {
      self::SleepFor(std::chrono::seconds(value));
      sorted_ints.push_back(value);
    });
  }
  wg.Wait();

  return sorted_ints;
}

int main() {
  RunScheduler([]() {
    auto sorted_ints = SleepSort({1, 3, 7, 2, 5, 4, 9, 8, 6});

    std::cout << "Sorted: ";
    for (size_t i = 0; i < sorted_ints.size(); ++i) {
      std::cout << sorted_ints[i] << ", ";
    }
    std::cout << std::endl;
  });
  return 0;
}
