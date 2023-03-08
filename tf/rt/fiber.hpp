#pragma once

#include <tf/rt/fwd.hpp>
#include <tf/rt/routine.hpp>
#include <tf/rt/id.hpp>
#include <tf/rt/watcher.hpp>

#include <sure/context.hpp>
#include <sure/stack.hpp>

#include <wheels/intrusive/list.hpp>

namespace tf::rt {

class Fiber : public wheels::IntrusiveListNode<Fiber>,
              private sure::ITrampoline {
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

  void SetWatcher(IFiberWatcher* watcher) {
    watcher_ = watcher;
  }

  void Resume();

  ~Fiber();

 private:
  Fiber(Scheduler* scheduler,  //
        FiberRoutine routine,  //
        sure::Stack stack,     //
        FiberId id);

  void SetupExecutionContext();

  // sure::ITrampoline
  [[noreturn]] void Run() noexcept override;

 private:
  Scheduler* scheduler_;
  FiberRoutine routine_;
  sure::Stack stack_;
  sure::ExecutionContext context_;
  FiberId id_;
  IFiberWatcher* watcher_{nullptr};
};

}  // namespace tf::rt
