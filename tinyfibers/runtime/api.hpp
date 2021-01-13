#pragma once

#include <wheels/support/time.hpp>
#include <wheels/support/function.hpp>

namespace tiny::fibers {

using wheels::Duration;

//////////////////////////////////////////////////////////////////////

using FiberRoutine = wheels::UniqueFunction<void()>;

using FiberId = size_t;

//////////////////////////////////////////////////////////////////////

// Runs 'init' routine in fiber scheduler in the current thread
void RunScheduler(FiberRoutine init);

//////////////////////////////////////////////////////////////////////

// This fiber functions

// Starts a new fiber managed by the current scheduler and
// puts this fiber to the end of the run queue.
// Does not transfer control to the scheduler.
void Spawn(FiberRoutine routine);

// Transfers control to the current scheduler
// and puts the current fiber to the end of the run queue
void Yield();

// Blocks the execution of the current fiber for at least
// the specified 'd'
void SleepFor(Duration d);

// Returns the id of the current fiber
FiberId GetFiberId();

}  // namespace tiny::fibers
