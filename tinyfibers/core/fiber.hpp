#pragma once

#include <sure/context.hpp>
#include <sure/stack.hpp>

#include <tinyfibers/core/api.hpp>
#include <tinyfibers/core/watcher.hpp>

#include <wheels/intrusive/list.hpp>

namespace tinyfibers {

enum class FiberState {
  Starting,
  Runnable,  // in run queue
  Running,
  Suspended,  // for example, in wait queue
  Terminated
};

class Fiber : public wheels::IntrusiveListNode<Fiber>,
              public sure::ITrampoline {
  friend class Scheduler;

 public:
  FiberId Id() const {
    return id_;
  }

  sure::ExecutionContext& Context() {
    return context_;
  }

  sure::Stack& Stack() {
    return stack_;
  }

  FiberState State() const {
    return state_;
  }

  void SetState(FiberState target) {
    state_ = target;
  }

  void SetWatcher(IFiberWatcher* watcher) {
    watcher_ = watcher;
  }

  ~Fiber();

 private:
  Fiber(FiberRoutine routine, sure::Stack&& stack, FiberId id);
  void SetupContext();

  // sure::ITrampoline
  [[noreturn]] void Run() noexcept override;

 private:
  FiberRoutine routine_;
  sure::Stack stack_;
  sure::ExecutionContext context_;
  FiberState state_;
  FiberId id_;
  IFiberWatcher* watcher_{nullptr};
};

}  // namespace tinyfibers
