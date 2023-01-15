#include <tinyfibers/run.hpp>
#include <tinyfibers/sched/spawn.hpp>
#include <tinyfibers/sched/yield.hpp>

#include <fmt/core.h>

using namespace tinyfibers;

void Breakpoint() {
}

void Bar() {
  fmt::print("Bar / before Yield\n");
  Breakpoint(); // Before first Yield
  self::Yield();
  fmt::print("Bar / after Yield\n");
  Breakpoint(); // After second Yield
}

void Foo() {
  Bar();
}

void Baz() {
  fmt::print("Baz / after first Yield\n");
  Breakpoint(); // After first Yield
  self::Yield();
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
