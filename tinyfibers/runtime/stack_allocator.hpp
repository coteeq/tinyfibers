#pragma once

#include <context/stack.hpp>

#include <vector>

namespace tinyfibers {

class StackAllocator {
  using Stack = context::Stack;

 public:
  Stack Allocate();
  void Release(Stack stack);

 private:
  static Stack AllocateNew();
  Stack TakeFromPool();

 private:
  std::vector<Stack> pool_;
};

}  // namespace tinyfibers
