#include <tf/rt/stack_allocator.hpp>

#include <wheels/core/size_literals.hpp>

#include <vector>

using sure::Stack;
using namespace wheels::size_literals;

namespace tf::rt {

//////////////////////////////////////////////////////////////////////

static const size_t kDefaultStackSize = 64_KiB;

//////////////////////////////////////////////////////////////////////

Stack StackAllocator::Allocate() {
  if (auto stack = TryTakeFromPool()) {
    return std::move(*stack);
  }
  return AllocateNew();
}

void StackAllocator::Release(Stack stack) {
  pool_.push_back(std::move(stack));
}

Stack StackAllocator::AllocateNew() {
  return Stack::AllocateBytes(/*at_least=*/kDefaultStackSize);
}

std::optional<Stack> StackAllocator::TryTakeFromPool() {
  if (pool_.empty()) {
    return std::nullopt;
  }

  Stack stack = std::move(pool_.back());
  pool_.pop_back();
  return stack;
}

}  // namespace tf::rt
