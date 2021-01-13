#include <tinyfibers/runtime/fiber.hpp>

#include <tinyfibers/runtime/scheduler.hpp>
#include <tinyfibers/runtime/stack_allocator.hpp>

#include <wheels/support/compiler.hpp>
#include <wheels/support/panic.hpp>
#include <wheels/support/exception.hpp>

namespace tiny::fibers {

//////////////////////////////////////////////////////////////////////

static FiberId GenerateId() {
  static FiberId next_id = 0;
  return ++next_id;
}

Fiber::Fiber(FiberRoutine routine, context::Stack&& stack, FiberId id)
    : routine_(std::move(routine)),
      stack_(std::move(stack)),
      state_(FiberState::Starting),
      id_(id) {
}

Fiber* Fiber::Create(FiberRoutine routine) {
  auto stack = AllocateStack();
  FiberId id = GenerateId();

  Fiber* fiber = new Fiber(std::move(routine), std::move(stack), id);

  fiber->SetupTrampoline();

  return fiber;
}

Fiber::~Fiber() {
  ReleaseStack(std::move(stack_));
}

//////////////////////////////////////////////////////////////////////

static void FiberTrampoline() {
  // Fiber execution starts here

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

}  // namespace tiny::fibers
