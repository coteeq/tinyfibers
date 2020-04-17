#include <tinyfibers/core/fiber.hpp>

#include <tinyfibers/core/scheduler.hpp>

#include <tinysupport/compiler.hpp>

namespace tiny::fiber {

//////////////////////////////////////////////////////////////////////

static FiberId GenerateId() {
  static FiberId next_id = 0;
  return ++next_id;
}

Fiber::Fiber(FiberRoutine routine, Stack&& stack, FiberId id)
    : routine_(std::move(routine)),
      stack_(std::move(stack)),
      state_(FiberState::Starting),
      id_(id) {
}

Fiber* Fiber::Create(FiberRoutine routine) {
  auto stack = Stack::Allocate();
  FiberId id = GenerateId();

  Fiber* fiber = new Fiber(std::move(routine), std::move(stack), id);

  fiber->SetupTrampoline();

  return fiber;
}

//////////////////////////////////////////////////////////////////////

static void FiberTrampoline() {
  // Fiber execution starts here

  Fiber* self = GetCurrentFiber();

  self->SetState(FiberState::Running);

  try {
    auto routine = self->UserRoutine();
    routine();
  } catch (...) {
    TINY_PANIC("Uncaught exception in fiber");
  }

  GetCurrentScheduler()->Terminate();  // never returns

  TINY_UNREACHABLE();
}

void Fiber::SetupTrampoline() {
  context_.Setup(
      /*stack=*/stack_.AsMemSpan(),
      /*trampoline=*/FiberTrampoline);
}

}  // namespace tiny::fiber
