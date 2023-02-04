#include <tinyfibers/run.hpp>
#include <tinyfibers/sched/spawn.hpp>
#include <tinyfibers/sched/yield.hpp>

#include <fmt/core.h>

using namespace tinyfibers;

void Breakpoint() {
}

void Bar() {
  fmt::println("Bar / before Yield");
  Breakpoint(); // Before first Yield
  self::Yield();
  fmt::println("Bar / after Yield");
  Breakpoint(); // After second Yield
}

void Foo() {
  Bar();
}

void Baz() {
  fmt::println("Baz / after first Yield");
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
