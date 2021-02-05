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

## Зависимости

- [Wheels](https://gitlab.com/Lipovsky/wheels) – общие компоненты
- [Context](https://gitlab.com/Lipovsky/context) – контекст исполнения
