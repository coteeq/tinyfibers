# <sub>Tiny</sub>Fibers

Минималистичная библиотека файберов, написанная для образовательных целей.

## Цель

Продемонстрировать базовую механику работы потоков.

## Файберы

Файберы (_fibers_) – легковесные потоки, реализованные целиком в пространстве пользователя.

### Особенности реализации

Файберы – _кооперативные_, они отдают управление только добровольно и явно (например, с помощью вызова `Yield`).

Файберы исполняются в одном потоке операционной системы (_carrier thread_), который служит для них виртуальным процессором.

Однопоточный планировщик + кооперативность дают детерминизм.

## Пример

```cpp
#include <tinyfibers/run.hpp>
#include <tinyfibers/sched/spawn.hpp>

#include <fmt/core.h>

using namespace tinyfibers;

int main() {
  // Стартуем планировщик и запускаем в нем первый файбер,
  // который будет исполнять переданную лямбду
  RunScheduler([]() {
    fmt::println("Parent");
    // Запускаем еще один файбер,
    // управление при этом остается у текущего файбера
    JoinHandle child = Spawn([]() {
      fmt::println("Child");
    });
    child.Join();  // Блокируем текущий файбер до завершения дочернего
    fmt::println("Child completed");
  });
  // Вызов RunScheduler завершится когда не останется готовых исполняться файберов
  
  return 0;
}
```

Вывод:
```
Parent
Child
Child completed
```

См. [примеры](/examples) и [тесты](/tests/fibers.cpp).

## Документация

- [API](/docs/api.md)
- [Диаграммы](https://disk.yandex.ru/d/QCh7Yk-FwSIjtA)

## Ограничения 

- Библиотека однопоточная
- Нет сети, каналов и т.п.

## References

### Context switch

- [System V ABI / AMD64](https://www.uclibc.org/docs/psABI-x86_64.pdf)
- [Context switch в Xv6](https://github.com/mit-pdos/xv6-public/blob/master/swtch.S)

### Fibers

- [Boost.fiber](https://github.com/boostorg/fiber)
- [folly::fibers](https://github.com/facebook/folly/blob/master/folly/fibers/README.md)
- [Marl](https://github.com/google/marl/)  
- [Project Loom: Fibers and Continuations for the Java Virtual Machine](https://cr.openjdk.java.net/~rpressler/loom/Loom-Proposal.html)

### Scheduler

- [xv6](https://github.com/mit-pdos/xv6-public/blob/eeb7b415dbcb12cc362d0783e41c3d1f44066b17/proc.c#L323)
- [Linux](https://github.com/torvalds/linux/blob/291009f656e8eaebbdfd3a8d99f6b190a9ce9deb/kernel/sched/core.c#L4921)
- [Golang](https://golang.org/src/runtime/proc.go)

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
- [Sure](https://gitlab.com/Lipovsky/sure) – контекст исполнения

### Внешние

- [Fmt](https://github.com/fmtlib/fmt) – форматированный вывод
