#pragma once

namespace tinyfibers::self {

// Transfers control to the current scheduler
// and puts the current fiber to the end of the run queue

void Yield();

}  // namespace tinyfibers::self
