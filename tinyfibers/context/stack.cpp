#include <tinyfibers/context/stack.hpp>

#include <utility>

namespace tiny::context {

Stack::Stack(MmapAllocation allocation) : allocation_(std::move(allocation)) {
}

Stack Stack::Allocate(size_t pages) {
  auto allocation = MmapAllocation::AllocatePages(pages + 1);
  allocation.ProtectPages(/*offset=*/0, /*count=*/1);
  return Stack{std::move(allocation)};
}

char* Stack::Bottom() const {
  return (char*)((std::uintptr_t*)allocation_.End() - 1);
}

MemSpan Stack::AsMemSpan() const {
  return allocation_.AsMemSpan();
}

}  // namespace tiny::context
