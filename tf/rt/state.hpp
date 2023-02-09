#pragma once

namespace tf::rt {

enum class FiberState {
  Starting,
  Runnable,  // in run queue
  Running,
  Suspended,  // for example, in wait queue
  Terminated
};

}  // namespace tf::rt
