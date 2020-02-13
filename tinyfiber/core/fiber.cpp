#include <tinyfiber/core/fiber.hpp>

#include <tinyfiber/core/scheduler.hpp>

#include <tinyfiber/support/compiler.hpp>

namespace tinyfiber {

//////////////////////////////////////////////////////////////////////

static FiberId GenerateId() {
  static FiberId next_id = 0;
  return ++next_id;
}

Fiber* Fiber::Create(FiberRoutine routine) {
  auto* fiber = new Fiber();

  fiber->stack_ = Stack::Allocate();
  fiber->id_ = GenerateId();
  fiber->routine_ = std::move(routine);
  fiber->state_ = FiberState::Starting;

  SetupTrampoline(fiber);

  return fiber;
}

//////////////////////////////////////////////////////////////////////

static void FiberTrampoline() {
  Fiber* self = GetCurrentFiber();

  self->SetState(FiberState::Running);

  auto routine = self->UserRoutine();
  try {
    routine();
  } catch (...) {
    TINY_PANIC("Uncaught exception in fiber");
  }

  GetCurrentScheduler()->Terminate();  // never returns

  TINY_UNREACHABLE();
}

void Fiber::SetupTrampoline(Fiber* fiber) {
  fiber->context_.Setup(
      /*stack=*/fiber->stack_.AsMemSpan(),
      /*trampoline=*/FiberTrampoline);
}

}  // namespace tinyfiber
