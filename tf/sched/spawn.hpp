#pragma once

#include <tf/rt/routine.hpp>
#include <tf/sync/join_handle.hpp>

#include <functional>

namespace tf {

// Creates and schedules a new fiber managed by the current scheduler and
// puts this fiber to the end of the run queue.
// Does not transfer control to the scheduler.

JoinHandle Spawn(rt::FiberRoutine fun);

}  // namespace tf
