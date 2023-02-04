#include <tinyfibers/rt/fiber.hpp>

#include <tinyfibers/rt/scheduler.hpp>

#include <wheels/core/compiler.hpp>
#include <wheels/core/panic.hpp>
#include <wheels/core/exception.hpp>

namespace tinyfibers::rt {

Fiber::Fiber(Routine routine, sure::Stack stack, FiberId id)
    : routine_(std::move(routine)),
      stack_(std::move(stack)),
      state_(FiberState::Starting),
      id_(id) {
  SetupContext();
}

Fiber::~Fiber() {
  if (watcher_ != nullptr) {
    watcher_->OnCompleted();
  }
}

void Fiber::Run() noexcept {
  // Fiber execution starts here

  SetState(FiberState::Running);

  try {
    routine_();
  } catch (...) {
    WHEELS_PANIC(
        "Uncaught exception in fiber: " << wheels::CurrentExceptionMessage());
  }

  CurrentScheduler()->Terminate();  // Never returns

  WHEELS_UNREACHABLE();
}

void Fiber::SetupContext() {
  context_.Setup(
      /*stack=*/stack_.MutView(),
      /*trampoline=*/this);
}

}  // namespace tinyfibers::rt
