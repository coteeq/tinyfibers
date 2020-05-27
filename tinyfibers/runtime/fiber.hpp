#pragma once

#include <tinyfibers/context/context.hpp>
#include <tinyfibers/context/stack.hpp>

#include <tinyfibers/runtime/api.hpp>

#include <tinysupport/intrusive_list.hpp>

namespace tiny::fibers {

//////////////////////////////////////////////////////////////////////

enum class FiberState {
  Starting,
  Runnable,
  Running,
  Suspended,  // in wait queue
  Terminated
};

class Fiber : public support::IntrusiveListNode<Fiber> {
 public:
  size_t Id() const {
    return id_;
  }

  context::ExecutionContext& Context() {
    return context_;
  }

  FiberState State() const {
    return state_;
  }

  void SetState(FiberState target) {
    state_ = target;
  }

  void InvokeUserRoutine() {
    routine_();
  }

  static Fiber* Create(FiberRoutine routine);

  ~Fiber();

 private:
  Fiber(FiberRoutine routine, context::Stack&& stack, FiberId id);

  void SetupTrampoline();

 private:
  FiberRoutine routine_;
  context::Stack stack_;
  context::ExecutionContext context_;
  FiberState state_;
  FiberId id_;
};

}  // namespace tiny::fibers
