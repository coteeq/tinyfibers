#pragma once

namespace tf {

// Transfers control to the current scheduler
// and puts this fiber to the end of the run queue

void Yield();

}  // namespace tf
