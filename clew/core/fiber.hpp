#pragma once

#include <clew/context/context.hpp>
#include <clew/context/stack.hpp>

#include <clew/core/api.hpp>

#include <clew/support/intrusive_list.hpp>

namespace clew {
namespace fiber {

//////////////////////////////////////////////////////////////////////

enum class FiberState { Starting, Runnable, Running, Suspended, Terminated };

class Fiber : public IntrusiveListNode<Fiber> {
 public:
  size_t Id() const {
    return id_;
  }

  const FiberStack& Stack() const {
    return stack_;
  }

  ExecutionContext& Context() {
    return context_;
  }

  FiberState State() const {
    return state_;
  }

  void SetState(FiberState target) {
    state_ = target;
  }

  FiberRoutine UserRoutine() {
    return routine_;
  }

  static Fiber* Create(FiberRoutine routine);
  static void SetupTrampoline(Fiber* fiber);

 private:
  FiberStack stack_;
  ExecutionContext context_;
  FiberState state_;
  FiberRoutine routine_;
  FiberId id_;
};

}  // namespace fiber
}  // namespace clew
