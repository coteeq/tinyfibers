# Tiny Fibers

_tinyfibers_ – минималистичная библиотека файберов, написанная для образовательных целей.

## Цель

Продемонстрировать базовую механику работы потоков.

## Пример

```cpp
#include <tinyfibers/runtime/api.hpp>

#include <iostream>

tinyfibers::RunScheduler([]() {
  std::cout << "Hello from fiber!" << std::endl;
  tinyfibers::Yield();  // Reschedule current fiber to the end of the scheduler run queue
});
```

См. [тесты](/tests/fibers.cpp).

## Ограничения 

- Библиотека однопоточная
- Поддерживается только x86-64
- Нет сети, каналов и т.п.

## Зависимости

- [Wheels](https://gitlab.com/Lipovsky/wheels) – общие компоненты
- [Context](https://gitlab.com/Lipovsky/context) – контекст исполнения
