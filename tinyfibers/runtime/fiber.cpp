#include <tinyfibers/runtime/fiber.hpp>

#include <tinyfibers/runtime/scheduler.hpp>

#include <wheels/support/compiler.hpp>
#include <wheels/support/panic.hpp>
#include <wheels/support/exception.hpp>

namespace tinyfibers {

//////////////////////////////////////////////////////////////////////

Fiber::Fiber(FiberRoutine routine, context::Stack&& stack, FiberId id)
    : routine_(std::move(routine)),
      stack_(std::move(stack)),
      state_(FiberState::Starting),
      id_(id) {
  SetupTrampoline();
}

//////////////////////////////////////////////////////////////////////

static void FiberTrampoline() {
  // Fiber execution starts here

  // No RAII here!

  Fiber* fiber = GetCurrentFiber();

  fiber->SetState(FiberState::Running);

  try {
    fiber->InvokeUserRoutine();
  } catch (...) {
    WHEELS_PANIC(
        "Uncaught exception in fiber: " << wheels::CurrentExceptionMessage());
  }

  GetCurrentScheduler()->Terminate();  // Never returns

  WHEELS_UNREACHABLE();
}

void Fiber::SetupTrampoline() {
  context_.Setup(
      /*stack=*/stack_.AsMemSpan(),
      /*trampoline=*/FiberTrampoline);
}

}  // namespace tinyfibers
