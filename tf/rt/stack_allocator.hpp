#pragma once

#include <sure/stack.hpp>

#include <optional>
#include <vector>

namespace tf::rt {

class StackAllocator {
  using Stack = sure::Stack;

 public:
  Stack Allocate();
  void Release(Stack);

 private:
  static Stack AllocateNew();
  std::optional<Stack> TryTakeFromPool();

 private:
  std::vector<Stack> pool_;
};

}  // namespace tf::rt
