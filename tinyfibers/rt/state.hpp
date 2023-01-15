#pragma once

namespace tinyfibers {

enum class FiberState {
  Starting,
  Runnable,  // in run queue
  Running,
  Suspended,  // for example, in wait queue
  Terminated
};

}  // namespace tinyfibers
