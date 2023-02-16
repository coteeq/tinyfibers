#pragma once

#include <tf/rt/routine.hpp>

namespace tf::rt {

// Runs 'init' routine in fiber scheduler in the current thread
void RunScheduler(FiberRoutine init);

}  // namespace tf::rt
