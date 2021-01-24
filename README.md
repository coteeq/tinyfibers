# Tiny Fibers

_tinyfibers_ – минималистичная библиотека файберов, написанная для образовательных целей.

## Цель

Продемонстрировать базовую механику работы потоков.

## Пример

```cpp
#include <tinyfibers/runtime/api.hpp>

#include <iostream>

using namespace tinyfibers;

int main() {
  RunScheduler([]() {
    std::cout << "Hello from main!" << std::endl;
    JoinHandle h = Spawn([]() {
      std::cout << "Hello from child!" << std::endl;
      self::Yield();
    });
    h.Join();
  });
  return 0;
}

```

См. [примеры](/examples) и [тесты](/tests/fibers.cpp).

## Ограничения 

- Библиотека однопоточная
- Поддерживается только x86-64
- Нет сети, каналов и т.п.

## Зависимости

- [Wheels](https://gitlab.com/Lipovsky/wheels) – общие компоненты
- [Context](https://gitlab.com/Lipovsky/context) – контекст исполнения
