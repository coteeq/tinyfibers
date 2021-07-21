# Tiny Fibers

Минималистичная библиотека файберов, написанная для образовательных целей.

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
- [Context](https://gitlab.com/Lipovsky/context) – контекст исполнения

## Документация

### `RunScheduler`

Вызов функции `RunScheduler` разворачивает _планировщик_ (_scheduler_) и запускает в нем файбер, который исполняет переданную функцию:


```cpp
RunScheduler([]() {
  std::cout << "I'm fiber!" << std::endl;
});
```

Планировщик поддерживает очередь (_run queue_) готовых исполняться файберов. 
На каждом шаге цикла планирования он извлекает файбер из головы этой очереди и запускает его. Запущенный файбер исполняется до тех пор, пока либо не завершит исполнение своей функции, либо добровольно не отдаст управление (например, вызвав `Yield` ) другому файберу.  

Вызов `RunScheduler` – блокирующий, он завершается тогда, когда планировщик исчерпает работу, т.е. когда завершится последний файбер.

### `Yield`

Вызов `self::Yield()` снимает бегущий файбер с "процессора" и помещает его в конец очереди планировщика. После этого планировщик запускает первый в очереди на исполнение файбер.

### `Spawn`

Функция `Spawn` создает новый файбер и помещает его в конец очереди планировщика.

Сам по себе вызов функции `Spawn` не приводит к передаче управления и моментальному исполнению нового файбера: новый файбер лишь планируется на исполнение, а управление остается у файбера, который вызвал `Spawn`.

```cpp
bool started = false;
JoinHandle h = Spawn([&]() {
  started = true;
  // Do something
});
assert(!started)
```

#### `JoinHandle`

Вызов `Spawn` возвращает объект `JoinHandle`, ассоциированный с запущенным файбером. 
С помощью `JoinHandle` можно структурировать работу родительского / дочернего файбера.

##### `Join`

Блокирует файбер, вызвавший `Join`, до тех пор, пока не завершится файбер, с которым ассоциирован данный `JoinHandle`:

```cpp
JoinHandle child = Spawn([]() {
  // ...
});
child.Join();  // Дожидаемся завершения запущенного файбера, отдаем управление
```

##### `Detach`

"Отвязывает" экземпляр `JoinHandle` от ассоциированного с ним файбера, последний отправляется в "свободное плавание".

Методы `Join` и `Detach` – взаимоисключающие, вызвать можно только один из них и только один раз.

Пользователь обязан вызвать или `Join` или `Detach`.

### Синхронизация

- `Mutex` 
- `CondVar`