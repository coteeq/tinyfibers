#include <tf/run.hpp>
#include <tf/sched/spawn.hpp>
#include <tf/sched/yield.hpp>

#include <fmt/core.h>

using tf::RunScheduler;
using tf::Yield;
using tf::Spawn;
using tf::JoinHandle;

void Breakpoint() {
}

void Bar() {
  fmt::println("Bar / before Yield");
  Breakpoint(); // Before first Yield
  Yield();
  fmt::println("Bar / after Yield");
  Breakpoint(); // After second Yield
}

void Foo() {
  Bar();
}

void Baz() {
  fmt::println("Baz / after first Yield");
  Breakpoint(); // After first Yield
  Yield();
}

int main() {
  RunScheduler([]() {
    JoinHandle h = Spawn([]() {
      Baz();
    });

    Foo();

    h.Join();
  });

  return 0;
}
