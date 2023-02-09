#pragma once

#include <tf/sync/join_handle.hpp>

#include <functional>

namespace tf {

// Starts a new fiber managed by the current scheduler and
// puts this fiber to the end of the run queue.
// Does not transfer control to the scheduler.

JoinHandle Spawn(std::function<void()> fun);

}  // namespace tf
