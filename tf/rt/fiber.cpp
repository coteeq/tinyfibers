#include <tf/rt/fiber.hpp>

#include <tf/rt/scheduler.hpp>

#include <wheels/core/compiler.hpp>
#include <wheels/core/panic.hpp>
#include <wheels/core/exception.hpp>

namespace tf::rt {

Fiber::Fiber(Scheduler* scheduler,  //
             FiberRoutine routine,  //
             sure::Stack stack,     //
             FiberId id)
    : scheduler_(scheduler),
      routine_(std::move(routine)),
      stack_(std::move(stack)),
      id_(id) {
  SetupExecutionContext();
}

Fiber::~Fiber() {
  if (watcher_ != nullptr) {
    watcher_->OnCompleted();
  }
}

void Fiber::Run() noexcept {
  // Fiber execution starts here

  try {
    routine_();
  } catch (...) {
    WHEELS_PANIC(
        "Uncaught exception in fiber: " << wheels::CurrentExceptionMessage());
  }

  scheduler_->Terminate();  // Never returns

  WHEELS_UNREACHABLE();
}

void Fiber::SetupExecutionContext() {
  context_.Setup(
      /*stack=*/stack_.MutView(),
      /*trampoline=*/this);
}

void Fiber::Resume() {
  scheduler_->Resume(this);
}

}  // namespace tf::rt
