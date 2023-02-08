#pragma once

#include <tinyfibers/rt/fwd.hpp>
#include <tinyfibers/rt/state.hpp>
#include <tinyfibers/rt/id.hpp>
#include <tinyfibers/rt/watcher.hpp>

#include <sure/context.hpp>
#include <sure/stack.hpp>

#include <wheels/intrusive/list.hpp>

namespace tinyfibers::rt {

class Fiber : public wheels::IntrusiveListNode<Fiber>,
              private sure::ITrampoline {
  friend class Scheduler;

  using Routine = std::function<void()>;

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

  void Resume();

  ~Fiber();

 private:
  Fiber(Scheduler* scheduler, Routine routine, sure::Stack stack, FiberId id);
  void SetupContext();

  // sure::ITrampoline
  [[noreturn]] void Run() noexcept override;

 private:
  Scheduler* scheduler_;
  Routine routine_;
  sure::Stack stack_;
  sure::ExecutionContext context_;
  FiberState state_;
  FiberId id_;
  IFiberWatcher* watcher_{nullptr};
};

}  // namespace tinyfibers::rt
