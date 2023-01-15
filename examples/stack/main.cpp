#include <tinyfibers/rt/run.hpp>
#include <tinyfibers/sched/spawn.hpp>
#include <tinyfibers/sched/yield.hpp>

#include <iostream>

using namespace tinyfibers;

void Breakpoint() {
}

void Bar() {
  std::cout << "Bar / before Yield" << std::endl;
  Breakpoint(); // Before first Yield
  self::Yield();
  std::cout << "Bar / after Yield" << std::endl;
  Breakpoint(); // After second Yield
}

void Foo() {
  Bar();
}

void Baz() {
  std::cout << "Baz / after first Yield" << std::endl;
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
