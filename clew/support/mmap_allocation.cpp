#include <clew/support/mmap_allocation.hpp>

#include <clew/support/assert.hpp>

#include <sys/mman.h>

namespace clew {

#define CHECK_RESULT(ret, error) VERIFY(ret != -1, error)

//////////////////////////////////////////////////////////////////////

static size_t PagesToBytes(size_t count) {
  static const size_t kPageSize = 4096;

  return count * kPageSize;
}

MmapAllocation MmapAllocation::AllocatePages(size_t count) {
  size_t size = PagesToBytes(count);

  void* start = mmap(/*addr=*/nullptr, /*length=*/size,
                     /*prot=*/PROT_READ | PROT_WRITE,
                     /*flags=*/MAP_PRIVATE | MAP_ANONYMOUS,
                     /*fd=*/-1, /*offset=*/0);

  VERIFY(start != MAP_FAILED, "Cannot allocate " << count << " pages");

  return MmapAllocation{(char*)start, size};
}

void MmapAllocation::ProtectPages(size_t offset, size_t count) {
  int ret = mprotect(/*addr=*/(void*)(start_ + PagesToBytes(offset)),
                     /*len=*/PagesToBytes(count),
                     /*prot=*/PROT_NONE);
  CHECK_RESULT(
      ret, "Cannot protect pages [" << offset << ", " << offset + count << ")");
}

MmapAllocation::MmapAllocation(MmapAllocation&& that) {
  start_ = that.start_;
  size_ = that.size_;
  that.Reset();
}

MmapAllocation& MmapAllocation::operator=(MmapAllocation&& that) {
  Release();
  start_ = that.start_;
  size_ = that.size_;
  that.Reset();
  return *this;
}

void MmapAllocation::Release() {
  if (start_ == nullptr) {
    return;
  }

  int ret = munmap((void*)start_, size_);
  CHECK_RESULT(ret, "Cannot unmap allocated pages");
}

void MmapAllocation::Reset() {
  start_ = nullptr;
  size_ = 0;
}

}  // namespace clew
