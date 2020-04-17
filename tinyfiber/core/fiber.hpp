#pragma once

#include <tinyfiber/context/context.hpp>
#include <tinyfiber/context/stack.hpp>

#include <tinyfiber/core/api.hpp>

#include <tinyfiber/support/intrusive_list.hpp>

namespace tiny::fiber {

//////////////////////////////////////////////////////////////////////

enum class FiberState {
  Starting,
  Runnable,
  Running,
  Suspended,  // in wait queue
  Terminated
};

class Fiber : public IntrusiveListNode<Fiber> {
 public:
  size_t Id() const {
    return id_;
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

 private:
  Fiber(FiberRoutine routine, Stack&& stack, FiberId id);

  void SetupTrampoline();

 private:
  FiberRoutine routine_;
  Stack stack_;
  ExecutionContext context_;
  FiberState state_;
  FiberId id_;
};

}  // namespace tiny::fiber
