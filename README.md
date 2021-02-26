# Tiny Fibers

_tinyfibers_ – минималистичная библиотека файберов, написанная для образовательных целей.

## Цель

Продемонстрировать базовую механику работы потоков.

## Пример

```cpp
#include <tinyfibers/api.hpp>

#include <iostream>

using namespace tinyfibers;

int main() {
  // Стартуем планировщик и запускаем в нем файбер,
  // исполняющий переданную лямбду
  RunScheduler([]() {
    std::cout << "Hello from parent!" << std::endl;
    // Запускаем еще один файбер,
    // управление при этом остается у текущего файбера
    JoinHandle child = Spawn([]() {
      std::cout << "Hello from child!" << std::endl;
      self::Yield();  // Отпускаем управление, текущий файбер встает в конец очереди планировщика
    });
    child.Join();  // Блокируем текущий файбер до завершения дочернего
    std::cout << "Child finished" << std::endl;
  });
  // Вызов RunScheduler завершится когда не останется готовых исполняться файберов
  return 0;
}
```

Вывод:
```
Hello from parent!
Hello from child!
Child finished
```

См. [примеры](/examples) и [тесты](/tests/fibers.cpp).

## Ограничения 

- Библиотека однопоточная
- Поддерживается только x86-64
- Нет сети, каналов и т.п.

## References

- [System V ABI / AMD64](https://www.uclibc.org/docs/psABI-x86_64.pdf)
- [Context switch в Xv6](https://github.com/guilleiguaran/xv6/blob/master/swtch.S)
- [Boost.fiber](https://github.com/boostorg/fiber)
- [Project Loom: Fibers and Continuations for the Java Virtual Machine](https://cr.openjdk.java.net/~rpressler/loom/Loom-Proposal.html)

## Опции CMake

- `TINY_FIBERS_TESTS=ON` – тесты
- `TINY_FIBERS_EXAMPLES=ON` – примеры

## Сборка

```shell
# Clone repo
git clone https://gitlab.com/Lipovsky/tinyfibers.git
cd tinyfibers
# Generate build files
mkdir build && cd build
cmake -DTINY_FIBERS_EXAMPLES=ON ..
# Build `hello` example
make tinyfibers_example_hello
# Run example
./examples/hello/bin/tinyfibers_example_hello
```

## Зависимости

- [Wheels](https://gitlab.com/Lipovsky/wheels) – общие компоненты
- [Context](https://gitlab.com/Lipovsky/context) – контекст исполнения
