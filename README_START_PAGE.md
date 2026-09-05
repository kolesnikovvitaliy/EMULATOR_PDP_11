# {#mainpage}
# 📟 PDP-11 Emulator   *В РАЗРАБОТКЕ*
### *Учебный проект по реализации легендарной 16-битной вычислительной машины*

---
[--> Документирвание  проекта](https://kolesnikovvitaliy.github.io/EMULATOR_PDP_11)
---


## 🛠 Ключевые возможности системы

Проект реализует полноценный цикл работы процессора и периферии:


| Функция | Описание реализации | Акценты |
| :--- | :--- | :---: |
| 🧠 **Эмуляция CPU** | Полная поддержка системы команд архитектуры **PDP-11**. | `MOV`, `ADD`, `SOB`, `BR` |
| 📟 **Регистровая модель** | Реализация **8 регистров** общего назначения (R0-R7). | `SP`, `PC`, `PSW` |
| 📂 **Загрузчик (Loader)** | Автоматическое чтение и выполнение программ. | **Octal Format** |
| 🔍 **Трассировка** | Интерактивный мониторинг состояния системы в реальном времени. | `Flags`, `Registers` |

---
## 🚀 Сборка и запуск

### 1. Требования
Для сборки вам понадобится компилятор `gcc` или `clang` с поддержкой стандарта C99.


## Запуск
Для сборки проекта используется `make`:

```bash
git clone https://github.com/kolesnikovvitaliy/EMULATOR_PDP_11.git
cd EMULATOR_PDP_11
make
```
## или
```bash
gcc -O2 -g3 -Iinclude -o pdp.out src/main.c src/pdp_11/utils/utils.c src/pdp_11/utils/logger/logger.c src/pdp_11/run.c src/pdp_11/memory/mem.c src/pdp_11/memory/buf_byte/buf_byte.c src/pdp_11/memory/buf_word/buf_word.c src/pdp_11/register/register.c src/pdp_11/size_buffer.c src/pdp_11/device_io/device_io.c src/pdp_11/device_io/terminal_io/dev_terminal_io.c src/pdp_11/device_io/files_io/dev_files_io.c src/pdp_11/pdp_11.c src/pdp_11/command/commands/JSR/jsr.c src/pdp_11/command/commands/ROL/rol.c src/pdp_11/command/commands/CLR/clr.c src/pdp_11/command/commands/ASH/ash.c src/pdp_11/command/commands/SOB/sob.c src/pdp_11/command/commands/ADCB/adcb.c src/pdp_11/command/commands/BPL/bpl.c src/pdp_11/command/commands/DIV/div.c src/pdp_11/command/commands/ADC/adc.c src/pdp_11/command/commands/COM/com.c src/pdp_11/command/commands/ASHC/ashc.c src/pdp_11/command/commands/ASL/asl.c src/pdp_11/command/commands/BIC/bic.c src/pdp_11/command/commands/INC/inc.c src/pdp_11/command/commands/TST/tst.c src/pdp_11/command/commands/HALT/halt.c src/pdp_11/command/commands/UNKNOWN/unknown.c src/pdp_11/command/commands/ADD/add.c src/pdp_11/command/commands/JMP/jmp.c src/pdp_11/command/commands/TSTB/tstb.c src/pdp_11/command/commands/BR/br.c src/pdp_11/command/commands/MOV/mov.c src/pdp_11/command/commands/BEQ/beq.c src/pdp_11/command/commands/ASR/asr.c src/pdp_11/command/commands/ROR/ror.c src/pdp_11/command/commands/RTS/rts.c src/pdp_11/command/commands/MUL/mul.c src/pdp_11/command/command.c src/pdp_11/command/commands.c src/pdp_11/command/commands_list.c test/test_pdp/test_io/test_io.c test/test_pdp/test_io/test_io_files/test_io_files.c test/test_pdp/test_io/test_io_terminal/test_io_terminal.c test/test_pdp/test_pdp.c test/test_pdp/test_memory/test_mem_word/test_word.c test/test_pdp/test_memory/test_mem_byte/test_byte.c test/test_pdp/test_memory/test_memory.c test/test_pdp/test_reg/test_reg.c test/test_pdp/test_command/commands/mov/test_mov.c test/test_pdp/test_command/commands/halt/test_halt.c test/test_pdp/test_command/modes/test_mode0/test_mode0.c test/test_pdp/test_command/modes/test_mode3/test_mode3.c test/test_pdp/test_command/modes/test_mode1/test_mode1_toreg/test_mode1_toreg.c test/test_pdp/test_command/modes/test_mode1/test_mode1_reg_to_mem/test_mode1_reg_to_mem.c test/test_pdp/test_command/modes/test_mode1/test_mode1_mem_to_mem/test_mode1_mem_to_mem.c test/test_pdp/test_command/modes/test_mode1/test_mode1.c test/test_pdp/test_command/modes/test_mode6/test_mode6.c test/test_pdp/test_command/modes/test_mode5/test_mode5.c test/test_pdp/test_command/modes/test_mode4/test_mode4.c test/test_pdp/test_command/modes/test_mode2/test_mode2.c test/test_pdp/test_command/modes/test_mode7/test_mode7.c test/test_pdp/test_command/test_command.c test/test.c
```
Для сборки проекта Windows:

```bash
git clone https://github.com/kolesnikovvitaliy/EMULATOR_PDP_11.git
cd EMULATOR_PDP_11
```
```bash
mingw32-gcc -O2 -g3 -Iinclude  -o pdp.exe src/main.c src/pdp_11/utils/utils.c src/pdp_11/utils/logger/logger.c src/pdp_11/run.c src/pdp_11/memory/mem.c src/pdp_11/memory/buf_byte/buf_byte.c src/pdp_11/memory/buf_word/buf_word.c src/pdp_11/register/register.c src/pdp_11/size_buffer.c src/pdp_11/device_io/device_io.c src/pdp_11/device_io/terminal_io/dev_terminal_io.c src/pdp_11/device_io/files_io/dev_files_io.c src/pdp_11/pdp_11.c src/pdp_11/command/commands/JSR/jsr.c src/pdp_11/command/commands/ROL/rol.c src/pdp_11/command/commands/CLR/clr.c src/pdp_11/command/commands/ASH/ash.c src/pdp_11/command/commands/SOB/sob.c src/pdp_11/command/commands/ADCB/adcb.c src/pdp_11/command/commands/BPL/bpl.c src/pdp_11/command/commands/DIV/div.c src/pdp_11/command/commands/ADC/adc.c src/pdp_11/command/commands/COM/com.c src/pdp_11/command/commands/ASHC/ashc.c src/pdp_11/command/commands/ASL/asl.c src/pdp_11/command/commands/BIC/bic.c src/pdp_11/command/commands/INC/inc.c src/pdp_11/command/commands/TST/tst.c src/pdp_11/command/commands/HALT/halt.c src/pdp_11/command/commands/UNKNOWN/unknown.c src/pdp_11/command/commands/ADD/add.c src/pdp_11/command/commands/JMP/jmp.c src/pdp_11/command/commands/TSTB/tstb.c src/pdp_11/command/commands/BR/br.c src/pdp_11/command/commands/MOV/mov.c src/pdp_11/command/commands/BEQ/beq.c src/pdp_11/command/commands/ASR/asr.c src/pdp_11/command/commands/ROR/ror.c src/pdp_11/command/commands/RTS/rts.c src/pdp_11/command/commands/MUL/mul.c src/pdp_11/command/command.c src/pdp_11/command/commands.c src/pdp_11/command/commands_list.c test/test_pdp/test_io/test_io.c test/test_pdp/test_io/test_io_files/test_io_files.c test/test_pdp/test_io/test_io_terminal/test_io_terminal.c test/test_pdp/test_pdp.c test/test_pdp/test_memory/test_mem_word/test_word.c test/test_pdp/test_memory/test_mem_byte/test_byte.c test/test_pdp/test_memory/test_memory.c test/test_pdp/test_reg/test_reg.c test/test_pdp/test_command/commands/mov/test_mov.c test/test_pdp/test_command/commands/halt/test_halt.c test/test_pdp/test_command/modes/test_mode0/test_mode0.c test/test_pdp/test_command/modes/test_mode3/test_mode3.c test/test_pdp/test_command/modes/test_mode1/test_mode1_toreg/test_mode1_toreg.c test/test_pdp/test_command/modes/test_mode1/test_mode1_reg_to_mem/test_mode1_reg_to_mem.c test/test_pdp/test_command/modes/test_mode1/test_mode1_mem_to_mem/test_mode1_mem_to_mem.c test/test_pdp/test_command/modes/test_mode1/test_mode1.c test/test_pdp/test_command/modes/test_mode6/test_mode6.c test/test_pdp/test_command/modes/test_mode5/test_mode5.c test/test_pdp/test_command/modes/test_mode4/test_mode4.c test/test_pdp/test_command/modes/test_mode2/test_mode2.c test/test_pdp/test_command/modes/test_mode7/test_mode7.c test/test_pdp/test_command/test_command.c test/test.c
```

## 💎 Параметры запуска

Используйте флаги командной строки для управления поведением эмулятора и уровнем детализации отчетов:


| 💠 Флаг | 🧊 Название | 🌐 Описание |
| :---: | :--- | :--- |
| `-f` | **READ_FILE**| Чтение данных из файла. Отсутствие флага означает ввод программы в терминале. |
| `-t` | **Trace** | Включает пошаговую трассировку: вывод состояния всех регистров и флагов `PSW` после каждой исполненной команды. Отсутствие флага выключает трассировку, запускается стандартный режим исполнения. |
| `-d` | **Debug/Tests** | Запуск встроенного набора модульных тестов для верификации корректности работы памяти, регистров и системы команд. |

---
## Запуск:
```bash
./pdp.exe -f ./data/test.txt
```

## Для вывода трассировки выполнения программы:
```bash
./pdp.exe -f -t ./data/test.txt
```

## Для запуска модульных тестов:
```bash
./pdp.exe -f -t -d ./data/test.txt
```
---


### 🚀 Детальный обзор:

*   **`Core`** — Поддержка арифметических, логических операций и инструкций ветвления.
*   **`Memory Management`** — Реализация работы со стеком (`SP`) и счетчиком команд (`PC`).
*   **`Binary Support`** — Работа с классическим восьмеричным дампом памяти, характерным для ретро-компьютеров.
*   **`Debugger`** — Детальный вывод флагов состояния процессора (**PSW**) после каждой инструкции для глубокой отладки.


---

## 💻 О проекте
Проект представляет собой высокоточный эмулятор архитектуры **PDP-11**, написанный на языке **Си**.
> **Ключевая особенность:** Строгое соблюдение **объектно-ориентированного подхода (OOD)** в рамках процедурного языка.

---

## 🛠️ Архитектурные особенности
Несмотря на использование чистого **C**, внутренняя логика проекта построена на трех столпах ООП:


| Принцип | Реализация в проекте |
| :--- | :--- |
| 🔒 **Инкапсуляция** | Состояние процессора, памяти и устройств ввода-вывода (I/O) инкапсулировано внутри специализированных структур `struct`. Прямой доступ к данным ограничен. |
| 🧩 **Модульность** | Система разделена на независимые "объекты": `COMMAND`, `I/O`, `Memory`. Каждый компонент можно тестировать и отлаживать отдельно. |
| 🎭 **Полиморфизм** | Гибкая обработка различных типов адресации и инструкций реализована через **таблицы переходов** и **указатели на функции**. |

---

## 🚀 Технологический стек
*   **Language:** `C-89`
*   **Approach:** Object-Oriented Design (OOD)
*   **Architecture:** DEC PDP-11 (16-bit)
*   **Toolchain:** GCC / Make

---
*Developed with focus on clean code and architectural excellence.*

# 🏗️ Архитектура системы

Проект разделен на четкие логические блоки, каждый из которых отвечает за свою часть эмуляции:

*   💾 **`Memory`** — Объект памяти: хранение данных и строгая проверка границ доступа.
*   🔌 **`IO`** — Эмуляция периферийных устройств (терминал, ввод-вывод).
*   📟 **`REGISTER`** — Быстрая память: регистры общего назначения `R0-R7`.
*   📜 **`LOGGER`** — Многоуровневая система отладки и мониторинга.
*   ⚙️ **`COMMAND`** — Ядро процессора: декодирование и выполнение инструкций.

---

## 🛠️ Реализация в стиле ООП
Система построена на принципах объектно-ориентированного программирования в среде С:
> Структуры хранят **состояние** (поля), а функции, принимающие указатель на структуру (`this`), выступают в роли **методов**.


### 🎮 [PDP_11] Эмулятор
`struct pdp_11_t` — Центральный интерфейс управления.


| Метод | Описание |
| :--- | :--- |
| `pdp_load_data(...)` | Загрузка образа программы в память. |
| `pdp_mem_dump(...)` | Форматированный дамп области памяти. |
| `b_write / b_read` | Запись и чтение **байта** по адресу. |
| `w_write / w_read` | Запись и чтение **слова** по адресу. |

---
### 💾 [Memory] Модуль памяти
`struct mem_t` — Низкоуровневый интерфейс работы с ОЗУ.
- `byte_write / byte_read` — Побайтовый доступ.
- `word_write / word_read` — Пословный доступ (2 байта).

---
### 🔌 [IO] Устройства Ввода-Вывода
`struct dev_io_t` — Взаимодействие с внешним миром.
- `dev_io_load_data(...)` — Загрузка образа программы через интерфейс устройства.
- `dev_io_mem_dump(...)` — Визуализация состояния памяти устройства.
---

### 📟 [REGISTER] Регистры
Реализация **8 регистров** общего назначения:
*   `R0 - R5` — Регистры общего назначения.
*   `R6 (SP)` — Указатель стека (*Stack Pointer*).
*   `R7 (PC)` — Счетчик команд (*Program Counter*).
---
### 📜 [LOGGER] Система логирования
Используются макросы для управления выводом:
*   ⚪ `PRINT_RESULT` — Замена стандартному `printf`.
*   🔴 `ERROR_LOG` — Критические ошибки.
*   🟢 `INFO_LOG` — Общая информация.
*   🟡 `WARNING_LOG` — Предупреждения о нештатных ситуациях.
*   🔵 `TRACE_LOG` — Трассировка вызова функций.
*   🟣 `DEBUG_LOG` — Подробные данные для разработки.

---
### ⚙️ [COMMAND] Исполнение команд
> 🚧 **В процессе разработки**

> Чтение и выполнение программ в восьмеричном (octal) формате.

> Текущая задача: Реализация полного цикла *Fetch-Decode-Execute*.

## ⚙️ Система команд (Instruction Set)
## На текущем этапе реализованы следующие основные инструкции:
| Мнемоника | Опкод | Описание | Тип |
| :--- | :--- | :--- | :--- |
| **MOV** | 1SSDD | Пересылка данных (Source → Destination) | Double Operand |
| **ADD** | 06SSDD | Сложение (Source + Destination → Destination) | Double Operand |
| **SUB** | 16SSDD | Вычитание (Dest - Source → Destination) | Double Operand |
| **CLR** | 0050DD | Очистка (0 → Destination) | Single Operand |
| **INC** | 0052DD | Инкремент (Dest + 1 → Destination) | Single Operand |
| **BR** | 0004XX | Безусловный переход | Branch |
| **SOB** | 077RXX | Вычесть 1 и перейти, если не ноль | Program Control |
| **HALT** | 000000 | Остановка процессора | Control |

#### Примечание: SS — адрес источника, DD — адрес назначения, XX — смещение.
🔍 Особенности реализации адресации
Эмулятор поддерживает стандартные режимы адресации PDP-11 для каждого операнда:


| Режим | Название (En) | Название (Ru) | Синтаксис | Описание |
| :---: | :--- | :--- | :--- | :--- |
| **0** | Register | Регистровый | `Rn` | Прямая работа с регистрами `R0–R7`. |
| **1** | Register Deferred | Регистровый косвенный | `@Rn` или `(Rn)` | Регистр содержит адрес операнда в памяти. |
| **2** | Autoincrement | Автоинкрементный | `(Rn)+` | Регистр содержит адрес, после обращения увеличивается на 1 или 2. Используется для немедленных значений (`#val` через `PC`). |
| **3** | Autoincrement Deferred | Автоинкрементный косвенный | `@(Rn)+` | Регистр содержит адрес адреса. После обращения увеличивается на 2. Используется для абсолютной адресации (`@#val` через `PC`). |
| **4** | Autodecrement | Автодекрементный | `-(Rn)` | Регистр сначала уменьшается на 1 или 2, затем используется как адрес операнда. |
| **5** | Autodecrement Deferred | Автодекрементный косвенный | `@-(Rn)` | Регистр сначала уменьшается на 2, затем используется как адрес, где лежит адрес операнда. |
| **6** | Index | Индексный | `X(Rn)` | Адрес равен сумме содержимого регистра `Rn` и смещения `X`. Используется для относительной адресации (`val` через `PC`). |
| **7** | Index Deferred | Индексный косвенный | `@X(Rn)` | Адрес равен сумме `Rn` и `X`, но указывает на ячейку, где хранится адрес операнда. Используется для относительной косвенной адресации (`@val` через `PC`). |



       ┌──────────────────────────┐
       │         Fetch            │  1. Чтение слова по адресу PC
       │  (Чтение из памяти)      │     PC = PC + 2
       └────────────┬─────────────┘
                    ▼
       ┌──────────────────────────┐
       │         DECODE           │  2. Определение типа команды
       │  (Парсинг опкода)        │     Выделение Mode и Register
       └────────────┬─────────────┘
                    ▼
       ┌──────────────────────────┐
       │    GET OPERANDS (Src/Dst)│  3. Вычисление эффективных адресов
       │  (Режимы адресации 0-7)  │     Загрузка значений операндов
       └────────────┬─────────────┘
                    ▼
       ┌──────────────────────────┐
       │        EXECUTE           │  4. Арифметико-логическая операция
       │  (ALU / Регистры)        │     Обновление флагов PSW (N, Z, V, C)
       └────────────┬─────────────┘
                    └─────────────────── Повтор цикла ───►


🚩 Регистр состояния процессора (PSW)

Регистр PSW (адрес 177776) хранит текущий статус выполнения и приоритет процессора.
Особое внимание в эмуляторе уделено флагам условий (Condition Codes):


| Бит | Флаг | Название | Описание |
| :---: | :---: | :--- | :--- |
| **3** | **N** | Отрицательный (Negative) | Устанавливается, если результат операции отрицательный (старший бит равен 1). |
| **2** | **Z** | Ноль (Zero) | Устанавливается, если результат операции равен нулю. |
| **1** | **V** | Переполнение (oVerflow) | Устанавливается при арифметическом переполнении для чисел со знаком. |
| **0** | **C** | Перенос (Carry) | Устанавливается при переносе из старшего разряда или заёме (для беззнаковых чисел). |


#### Эти флаги критически важны для работы инструкций ветвления (например, BEQ, BNE, BMI).





# 📋 Описание проекта

### 🔍 Пример логирования
В проекте реализована наглядная система отладки для контроля за состоянием процессора:

> 🟩 **[INFO_LOG]** — Общая информация о загрузке дампа.
> 🟦 **[DEBUG_LOG]** — Состояние регистров `(R0-R7)` после каждой инструкции.
> 🟥 **[ERROR_LOG]** — Ошибки обращения к памяти или неизвестные опкоды.

---

### 📈 План разработки (Roadmap)
- [x] 🏗️ **Этап 1:** Базовая структура проекта и логгер.
- [x] ⚙️ **Этап 2:** Полная реализация системы команд (*Instruction Set*).
- [x] ⌨️ **Этап 3:** Эмуляция терминала и простейшего ввода/вывода.
- [x] 🧪 **Этап 4:** Запуск оригинальных тестов **PDP-11**.

---

### 🤝 Контакты
**Автор:** `Vitaliy Kolesnikov`
**Телефон:** `8(938)165-83-47`
**Email:** `kolesnikovvitaliy@mail.ru`

Проект находится в **активной стадии разработки**. Если у вас есть идеи или вы нашли баг — смело создавайте **Issue** или **Pull Request**!

---
*Developed with ❤️ for Retro Computing*
