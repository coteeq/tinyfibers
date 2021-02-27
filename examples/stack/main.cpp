#include <tinyfibers/api.hpp>

#include <iostream>

using namespace tinyfibers;

void Bar() {
  std::cout << "Bar / before Yield" << std::endl;
  // BP: Before first Yield
  self::Yield();
  std::cout << "Bar / after Yield" << std::endl;
  // BP: After second Yield
}

void Foo() {
  Bar();
}

void Baz() {
  std::cout << "Baz / after first Yield" << std::endl;
  // BP: After first Yield
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
