#include <tinyfibers/sync/wait_group.hpp>

#include <wheels/support/assert.hpp>

namespace tinyfibers {

void WaitGroup::Spawn(FiberRoutine routine) {
  join_handles_.push_back(::tinyfibers::Spawn(std::move(routine)));
}

void WaitGroup::Wait() {
  for (auto& h : join_handles_) {
    h.Join();
  }
  join_handles_.clear();
}

WaitGroup::~WaitGroup() {
  WHEELS_VERIFY(join_handles_.empty(), "Explicit Join required");
}

}  // namespace tinyfibers
