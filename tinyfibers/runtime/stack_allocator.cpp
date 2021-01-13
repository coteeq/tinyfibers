#include <tinyfibers/runtime/stack_allocator.hpp>

#include <vector>

namespace tiny::fibers {

using context::Stack;

//////////////////////////////////////////////////////////////////////

static const size_t kDefaultStackSizeInPages = 8;

//////////////////////////////////////////////////////////////////////

// Simple stack pooling

class StackAllocator {
 public:
  Stack Allocate() {
    if (!pool_.empty()) {
      Stack stack = std::move(pool_.back());
      pool_.pop_back();
      return stack;
    }
    return Stack::Allocate(kDefaultStackSizeInPages);
  }

  void Release(Stack stack) {
    pool_.push_back(std::move(stack));
  }

 private:
  std::vector<Stack> pool_;
};

//////////////////////////////////////////////////////////////////////

static StackAllocator allocator;

Stack AllocateStack() {
  return allocator.Allocate();
}

void ReleaseStack(Stack stack) {
  allocator.Release(std::move(stack));
}

}  // namespace tiny::fibers
