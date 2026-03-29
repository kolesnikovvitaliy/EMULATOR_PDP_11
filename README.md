# STEPIC PDP-11 Emulator
#
Документирвание  проекта "https://kolesnikovvitaliy.github.io/EMULATOR_PDP_11/"
#
## Учебный проект по реализации эмулятора 16-битной вычислительной машины **PDP-11**.
## Проект написан на языке Си с применением **объектно-ориентированного подхода**.

## Особенности реализации
Несмотря на использование чистого C, архитектура проекта следует принципам ООП:
*   **Инкапсуляция**: Состояние процессора, памяти и устройств ввода-вывода скрыто внутри соответствующих структур.
*   **Модульность**: Каждый компонент системы (CPU, Bus, Memory) представлен как отдельный "объект".
*   **Полиморфизм**: Обработка различных типов адресации и инструкций через таблицы переходов и указатели на функции.

## Архитектура проекта
Проект разделен на логические блоки:

*   `Memory`: Объект памяти, отвечающий за хранение данных и проверку границ доступа.
*   `IO`: Эмуляция периферийных устройств(терминал).
*   `REGISTER`: ОбЪеты дополнительной быстрой памяти регистры  R0...R7
*   `LOGGER`: Система логирования
### Реализация в стиле ООП
Каждый программный модуль представляет собой "класс",
где структура хранит состояние (поля),
а функции, принимающие указатель на эту структуру,
выступают в роли методов.
*   **PDP_11(Эмулятор)**
    *   `struct pdp_11_t` - Интерфейс эмулятора.
    *   `pdp_load_data(struct pdp_11_t* pdp, byte_t* filename)` - загрузка образа программы в объект памяти.
    *   `pdp_mem_dump(struct pdp_11_t* pdp, address_word_t addr, word_t size)` - форматированный вывод данных из памяти.
    *   `b_write(struct pdp_11_t* , const address_byte_t, byte_t)` - пишем  байт по адрессу.
    *   `b_read(struct pdp_11_t* , const address_byte_t)` - читаем байт по адрессу и возвращаем его.
    *   `w_write(struct pdp_11_t* , const address_word_t, word_t)` - пишем слово по адрессу.
    *   `w_read(struct pdp_11_t* , const address_word_t)` - читаем слово по адрессу и возвращаем его.

*   **Memory(Память)**
    *   `struct mem_t` — Интерфейс для работы с памятью.
    *   `byte_write(struct mem_t*, address_byte_t, byte_t)` - пишем  значение байт по адресу.
    *   `byte_read(struct mem_t*, address_byte_t)` - читаем байт по адрессу и возвращаем его.
    *   `word_write(struct mem_t*, address_word_t, word_t)` - пишем значение слово по адрессу.
    *   `word_read(struct mem_t*, address_word_t)` - читаем слово по адрессу и возвращаем его.
*   **IO (Устройства)**
    *   `struct dev_io_t` — Интерфейс для работы с устойством Ввода-Вывода.
    *   `dev_io_load_data(struct pdp_11_t*,struct dev_io_t*, byte_t*)`  - загрузка образа программы в объект памяти.
    *   `dev_io_mem_dump(struct pdp_11_t*, address_word_t, word_t)`  - форматированный вывод данных из памяти.
*   **REGISTER(Регистры)**
    *   `Регистров от R0 до R7 Эмулирующих бытрую память процессора`
*   **LOGGER(Сиситема Логирования)**
    *    `ERROR`: Сообщения об ошибках
    *    `INFO`: Дополнительная информация
    *    `WARNING`: Предупреждения
    *    `TRACE`: Сообщения о выполняемых функциях
    *    `DEBUG`: Сообщения необходимые для разработки


## Ключевые компоненты

### Memory (Объект памяти)
Реализован как абстракция над массивом байтов. Поддерживает методы:
- `b_read / w_read`: Чтение байта или слова.
- `b_write / w_write`: Запись байта или слова.

### I/O (Ввод-вывод)
Используется механизм **MMIO** (Memory Mapped I/O).
Определяется наличие префикса -t и производиться чтение данных из файла или терминала

### REGISTER (Регистры)
ОбЪект Быстрой встроенной памяти процессора, регистры R0...R7

### LOGGER (Система логирования)
Система сообщений и журналирования о программных событиях

## Сборка и запуск
Для сборки проекта используется `make`:

```bash
git clone https://github.com/kolesnikovvitaliy/EMULATOR_PDP_11.git
cd EMULATOR_PDP_11
make
```
Для сборки проекта Windows:

```bash
git clone https://github.com/kolesnikovvitaliy/EMULATOR_PDP_11.git
cd EMULATOR_PDP_11
mingw32-gcc -O2 -g3 -Wall -Wextra -Wpedantic -std=c11 -Iinclude -o pdp.out src/main.c src/pdp_11/utils/utils.c src/pdp_11/utils/logger/logger.c src/pdp_11/run.c src/pdp_11/memory/mem.c src/pdp_11/memory/buf_byte/buf_byte.c src/pdp_11/memory/buf_word/buf_word.c src/pdp_11/register/register.c src/pdp_11/size_buffer.c src/pdp_11/device_io/device_io.c src/pdp_11/device_io/terminal_io/dev_terminal_io.c src/pdp_11/device_io/files_io/dev_files_io.c src/pdp_11/pdp_11.c src/pdp_11/command/command.c test/test_pdp/test_io/test_io.c test/test_pdp/test_io/test_io_files/test_io_files.c test/test_pdp/test_io/test_io_terminal/test_io_terminal.c test/test_pdp/test_pdp.c test/test_pdp/test_memory/test_mem_word/test_word.c test/test_pdp/test_memory/test_mem_byte/test_byte.c test/test_pdp/test_memory/test_memory.c test/test.c
```


## Запуск
```bash
./pdp.out -t ./data/test.txt
```

## Для запуска модульных тестов
### Закомменитровать строки запуска и активации в файле main.c и run.c
### Раскомментировать строки с тестами.
```bash
make
./pdp.out -t ./data/test.txt
```
