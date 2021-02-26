#include <tinyfibers/api.hpp>

using namespace tinyfibers;

void Bar() {
  // BP: Before first Yield
  self::Yield();
  // BP: After second Yield
}

void Foo() {
  Bar();
}

void Baz() {
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
