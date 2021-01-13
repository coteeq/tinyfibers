#pragma once

#include <wheels/support/memspan.hpp>

#include <cstdlib>
#include <cstdint>

namespace tiny::context {

using wheels::MemSpan;

typedef void (*Trampoline)();

struct ExecutionContext {
  // Execution context saved on top of suspended fiber/thread stack
  void* rsp_;

  // Prepare execution context for running trampoline function
  void Setup(MemSpan stack, Trampoline trampoline);

  // Save the current execution context to 'this' and jump to the
  // 'target' context. 'target' context created directly by Setup or
  // by another target.SwitchTo(other) call.
  void SwitchTo(ExecutionContext& target);
};

}  // namespace tiny::context
