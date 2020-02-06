#include <clew/context/stack.hpp>

#include <utility>

namespace clew {
namespace fiber {

static const size_t kStackPages = 8;  // 8KB stacks

FiberStack::FiberStack(MmapAllocation allocation)
    : allocation_(std::move(allocation)) {
}

FiberStack FiberStack::Allocate() {
  MmapAllocation allocation = MmapAllocation::AllocatePages(kStackPages);
  allocation.ProtectPages(/*offset=*/0, /*count=*/1);
  return FiberStack{std::move(allocation)};
}

char* FiberStack::Bottom() const {
  return (char*)((std::uintptr_t*)allocation_.End() - 1);
}

MemSpan FiberStack::AsMemSpan() const {
  return allocation_.AsMemSpan();
}

}  // namespace fiber
}  // namespace clew
