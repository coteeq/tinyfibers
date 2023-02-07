#pragma once

namespace tinyfibers {

// Transfers control to the current scheduler
// and puts the current fiber to the end of the run queue

void Yield();

}  // namespace tinyfibers
