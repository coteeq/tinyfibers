#pragma once

#include <clew/support/memspan.hpp>

namespace clew {

class MmapAllocation {
 public:
  MmapAllocation() {
    Reset();
  }

  // Allocate `count` pages of zeroed memory
  static MmapAllocation AllocatePages(size_t count);

  // Moving
  MmapAllocation(MmapAllocation&& that);
  MmapAllocation& operator=(MmapAllocation&& that);

  ~MmapAllocation() {
    Release();
  }

  char* Start() const {
    return start_;
  }

  char* End() const {
    return start_ + size_;
  }

  size_t Size() const {
    return size_;
  }

  MemSpan AsMemSpan() const {
    return MemSpan(start_, size_);
  }

  // Protect range of pages
  // Protected pages cannot be read, written or executed
  void ProtectPages(size_t offset, size_t count);

  void Release();

 private:
  MmapAllocation(char* start, size_t size) : start_(start), size_(size) {
  }

  void Reset();

 private:
  // Aligned to page boundary
  char* start_;
  size_t size_;
};

}  // namespace clew
