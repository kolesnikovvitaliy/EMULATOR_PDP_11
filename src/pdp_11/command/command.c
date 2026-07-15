/**
 * @file command.c
 * @brief 🚀 Подсистема управления и декодирования команд процессора PDP-11
 * @details Данный файл содержит ядро декодера инструкций эмулятора. Он
 * отвечает за выделение памяти под структуры команд, инициализацию шаблонов, а
 * также за парсинг режимов адресации (мод) и извлечение операндов из машинного
 * слова.
 *
 * @author Ваше Имя / Команда разработки
 * @date 2026
 */

#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/command/commands_list.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "utils/logger/logger.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Внешний массив шаблонов базовых команд процессора
 */
extern command_t template_commands[];

/**
 * @brief 📦 Инициализатор контейнера: Выделение памяти под массив команд
 * @details Аллоцирует массив указателей на команды. Размер массива жестко
 * завязан на глобальную константу @ref commands_list.
 *
 * @return command_t** Указатель на массив указателей команд, либо `NULL` при
 * критическом сбое памяти.
 *
 * @pre Константа @ref commands_list должна быть корректно определена и
 * инициализирована.
 */
command_t **
command_new(void)
{
    return (command_t **) malloc(
        commands_list
        * sizeof(command_t *)); // Выделение памяти для объекта command_t
}

/**
 * @brief 🔨 Фабрика команд: Глубокое копирование и инициализация объектов
 * @details Проходит по всему списку выделенных указателей, аллоцирует память
 * под каждую конкретную инструкцию и заливает туда дефолтные маски и
 * обработчики из шаблонов.
 *
 * @param[in,out] commands Указатель на ранее созданный массив указателей
 * команд.
 *
 * @attention В случае неудачи `malloc` пропускает ячейку, предотвращая запись
 * по `NULL`-адресу.
 */
void
command_create(command_t **commands)
{
    for (int i = 0; i < commands_list; i++) {
        commands[i] = (command_t *) malloc(sizeof(command_t));
        if (commands[i] != NULL) {
            *commands[i] = template_commands[i];
        }
    }
}

/**
 * @brief 🪓 Утилизатор памяти: Деструктор
 * списка команд
 * @details Освобождает память, выделенную под каждую индивидуальную команду
 * внутри цикла.
 *
 * @param[in] commands Массив указателей на уничтожаемые команды.
 *
 * @note Сам массив верхнего уровня должен быть освобожден вызывающим кодом
 * отдельно через `free()`.
 */
void
command_destroy(command_t **commands)
{
    for (int i = 0; i < commands_list; i++) {
        free(commands[i]);
    }
}

/**
 * @brief 📑 Внутренний трассировщик: Логирование текущей инструкции
 * @details Выводит отладочную информацию о выполняемой команде в удобочитаемом
 * восьмеричном формате, принятом в документации по PDP-11.
 *
 * @param[in] addr Адрес в памяти (PC), где лежит инструкция.
 * @param[in] word_command 16-битное машинное слово команды.
 * @param[in] name_command Текстовая строка с мнемоникой (например, "MOV",
 * "ADD").
 */
void
__print_command(address_word_t addr, word_t word_command, byte_t *name_command)
{
    PRINT_RESULT("%06o %06o : %s", addr, word_command, name_command);
}

/**
 * @brief 🔍 Синтаксический анализатор мод адресации (Парсер операндов)
 * @details Самая важная низкоуровневая функция. Анализирует младшие 6 бит
 * переданного фрагмента команды. Выделяет номер регистра (биты 0-2) и режим
 * адресации (биты 3-5). В зависимости от режима вычисляет эффективный адрес и
 * считывает значение.
 *
 * ### Поддерживаемые режимы (моды):
 * - **Мода 0 (Регистровая):** Операнд находится прямо в регистре `R_n`.
 * - **Мода 1 (Косвенно-регистровая):** В регистре лежит адрес операнда
 * `(R_n)`.
 * - **Мода 2 (Автоинкрементная):** То же, что мода 1, но регистр увеличивается
 * на 2 после чтения `(R_n)+`.
 *   - _Особый случай:_ Если регистр — `R7 (PC)`, мода работает как чтение
 * константы (Immediate `#value`).
 *
 * @param[in,out] pdp  Указатель на контекст процессора.
 * @param[in] word_command Фрагмент машинного слова, содержащий 6 бит описания
 * операнда.
 *
 * @return @ref arg_t Заполненная структура аргумента (вычисленный адрес и
 * актуальное значение).
 *
 * @todo Дописать оставшиеся моды адресации (3-7).
 * @todo Исправить инкремент для байтовых операций в моде 2 (сейчас всегда
 * жестко +2).
 */
arg_t
__get_args(struct pdp_11_t *pdp, word_t word_command)
{

    arg_t  res          = { 0, 0 };
    word_t num_register = word_command & 07;
    byte_t mode         = (word_command >> 3) & 07;

    switch (mode) {
        /**
         * @name      Режим адресации: Регистровый прямой (Register Direct)
         * @anchor    addressing_mode_0
         * @see       http://pdp-11.org.ru
         * @{
         */

        /**
         * @brief     Обработка **Режима 0** адресации PDP-11 (Прямая
         * регистрация).
         * @details   Данный `case` выполняется, когда метод адресации операнда
         * равен 0. Это самый быстрый режим работы процессора, так как он **не
         * обращается к оперативной памяти**. Операндом является
         * непосредственно содержимое выбранного регистра общего назначения
         * (РОН).
         *
         * ### Схема работы (Data Flow):
         * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         * [ Регистр (num_register) ] ---------------> [ res.value ]
         *    содержит сам операнд                       готовое значение для
         * операции
         *              |
         *              v
         *          [ res.addr ] (сохраняет индекс регистра для возможной
         * записи)
         * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         *
         * @param[in]  pdp          Указатель на структуру виртуальной машины
         * PDP-11.
         * @param[in]  num_register Номер целевого регистра общего назначения
         * (R0..R7).
         * @param[out] res.addr     В данном режиме — не адрес памяти, а
         * **номер (индекс) регистра**. Это критически важно для
         * инструкций-приемников (destination), чтобы знать, в какой РОН
         * записать результат.
         * @param[out] res.value    Считанное 16-битное значение
         * непосредственно из регистра @p res.addr.
         *
         * @note       Этот режим обозначается в ассемблере просто именем
         * регистра: `Rn`.
         * @warning    В режиме 0 значение @p res.addr трактуется кодом
         * выполнения команды как индекс в массиве регистров, а не как
         * указатель на ОЗУ. Попытка передать @p res.addr в функции типа
         * `w_read()` или `w_write()` приведет к критической ошибке эмуляции!
         *
         * @code
         * // Пример на ассемблере PDP-11:
         * MOV R1, R2      ; Скопировать чистое содержимое регистра R1 в
         * регистр R2 ADD R0, R3      ; Прибавить значение из R0 к значению в
         * R3
         * @endcode
         */
    case 0:
        // мода 0, R1
        /* Шаг 1: Эффективным "адресом" становится сам номер регистра */
        res.addr = num_register;

        /* Шаг 2: Извлекаем 16-битное значение напрямую из этого регистра */
        res.value = pdp_reg_get_var(pdp, res.addr);

        /* Шаг 3: Логируем операцию в стандартном для ассемблера формате Rn */
        PRINT_RESULT("R%d ", res.addr);

        break;
        /** @} */
        // мода 1, (R1)
        /**
         * @name      Режим адресации: Регистровый косвенный (Register
         * Indirect)
         * @anchor    addressing_mode_1
         * @see       http://pdp-11.org.ru
         * @{
         */

        /**
         * @brief     Обработка **Режима 1** адресации PDP-11 (Косвенная
         регистрация).
         * @details   Данный `case` выполняется, когда метод адресации операнда
         равен 1.
         *            В этом режиме указанный регистр содержит не сам операнд,
         а его
         *            **16-битный память-адрес**. Процессор считывает этот
         адрес из
         *            регистра, а затем выполняет физическое чтение данных из
         памяти.
         *
         * ### Схема работы (Data Flow):
         * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         * [ Регистр (num_register) ] -------> [ Память (res.addr) ] -------> [
         res.value ]
         *   содержит адрес ссылки                хранит целевое значение
         готовый операнд
         * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         *
         * @param[in]  pdp          Указатель на структуру виртуальной машины
         PDP-11.
         * @param[in]  num_register Номер целевого регистра общего назначения
         (R0..R7).
         * @param[out] res.addr     Вычисленный эффективный адрес в памяти
         процессора.
         * @param[out] res.value    Считанное 16-битное значение (слово) по
         адресу @p res.addr.
         *
         * @note       Этот режим часто обозначается в ассемблере как `(Rn)`.

         *
         * @code
         * // Пример на ассемблере PDP-11:
         * MOV (R1), R2    ; Переслать значение из адреса, лежащего в R1, в
         регистр R2
         * @endcode
         */
    case 1:
        /* Шаг 1: Извлекаем из регистра адрес ячейки памяти */
        res.addr = pdp_reg_get_var(pdp, num_register);
        /* Шаг 2: Выполняем чтение 16-битного слова из шины памяти по этому
         * адресу */
        res.value = w_read(pdp, res.addr);
        /* Шаг 3: Логируем операцию для отладки в формате (Rn) */
        PRINT_RESULT("(R%d) ", num_register);
        break;
        /** @} */
    // мода 2, (R1)+ или #3
    case 2:
        res.addr = pdp_reg_get_var(pdp, num_register);
        // ss -откуда, dd - куда;
        pdp_reg_set_var(pdp, num_register, (address_word_t)(res.addr + 2));

        // печать разной мнемоники для PC и других регистров
        if (num_register == 7) {
            res.value = w_read(pdp, (address_word_t)(res.addr + 2));
            PRINT_RESULT("#%o ", res.value);
        } else {
            res.value = w_read(pdp, (address_word_t)(res.addr));
            PRINT_RESULT("(R%d)+ ", num_register);
        }

        break;
    case 3:; //  для объявления типа данных address_word_t требуется " ; "
             //  после метки
        address_word_t temp_value_register
            = (word_t) pdp_reg_get_var(pdp, num_register); // 1002

        address_word_t inc_addr_offset
            = (address_word_t)(temp_value_register + 2); // 1004

        // ss -откуда, dd - куда;
        pdp_reg_set_var(pdp, num_register, (address_word_t)(inc_addr_offset));

        if (num_register == 7) {

            address_word_t addr_reg_pc = (address_word_t)(
                w_read(pdp, (address_word_t)(inc_addr_offset)));

            res.addr  = addr_reg_pc;
            res.value = w_read(pdp, (address_word_t)(res.addr));

            PRINT_RESULT("@#%o ", res.addr);
        } else {

            address_word_t addr_top = (address_word_t)(
                w_read(pdp, (address_word_t)(temp_value_register)));

            res.addr  = addr_top;
            res.value = w_read(pdp, (address_word_t)(res.addr));

            PRINT_RESULT("@(R%d)+ ", num_register);
        }
        // __command_reg_dump(pdp);
        break;
    case 4:; //  для объявления типа данных word_t требуется " ; " после метки
        word_t t_var_reg = pdp_reg_get_var(pdp, num_register);

        pdp_reg_set_var(
            pdp, num_register, (address_word_t)(t_var_reg - 2)); // TODO: +1

        res.addr = (address_word_t)(pdp_reg_get_var(pdp, num_register));

        res.value = w_read(pdp, (address_word_t)(res.addr));

        PRINT_RESULT("-(R%d) ", num_register);
        break;
    case 5:; //  для объявления типа данных word_t требуется " ; " после метки

        temp_value_register = (word_t) pdp_reg_get_var(pdp, num_register);

        inc_addr_offset = (address_word_t)(temp_value_register - 2);

        address_word_t addr_top
            = (address_word_t)(w_read(pdp, (address_word_t)(inc_addr_offset)));

        pdp_reg_set_var(pdp, num_register, (address_word_t)(inc_addr_offset));

        res.addr  = addr_top;
        res.value = w_read(pdp, (address_word_t)(res.addr));
        // ss -откуда, dd - куда;

        PRINT_RESULT("@-(R%d) ", num_register);

        break;
    case 6:;
        address_word_t addr_in_pc  = (address_word_t) pdp_reg_get_var(pdp, 7);
        word_t         word_in_mem = w_read(pdp, (word_t)(addr_in_pc + 2));
        pdp_reg_set_var(pdp, 7, (address_word_t)(addr_in_pc + 2));

        res.addr = pdp_reg_get_var(pdp, num_register);
        res.addr = (word_t)(res.addr + word_in_mem);

        res.value = w_read(pdp, res.addr);

        if (num_register == 7) {
            addr_in_pc = (address_word_t)(pdp_reg_get_var(pdp, 7));
            pdp_reg_set_var(pdp, 7, (address_word_t)(addr_in_pc + 2));
            addr_in_pc  = (address_word_t)(pdp_reg_get_var(pdp, 7));
            word_in_mem = (word_t)(addr_in_pc + word_in_mem);

            res.value = word_in_mem;
            PRINT_RESULT("%o ", res.value);
        } else {

            PRINT_RESULT("%d(R%d) ", word_in_mem, num_register);
        }
        break;
    case 7:;
        word_t temp_addr = 00;
        addr_in_pc       = (address_word_t) pdp_reg_get_var(pdp, 7);
        pdp_reg_set_var(pdp, 7, (address_word_t)(addr_in_pc + 2));

        addr_in_pc = (address_word_t) pdp_reg_get_var(pdp, 7);

        word_t offset = w_read(pdp, (word_t)(addr_in_pc));

        pdp_reg_set_var(pdp, 7, (address_word_t)(addr_in_pc + 2));

        if (num_register == 7) {
            res.addr = pdp_reg_get_var(pdp, 7);

            res.addr = (word_t)(offset + res.addr);

            temp_addr = w_read(pdp, res.addr);

            res.value = w_read(pdp, temp_addr);

            PRINT_RESULT("@%o ", res.addr);

        } else {

            res.addr = pdp_reg_get_var(pdp, num_register);

            res.addr = (word_t)(offset + res.addr);

            res.addr = w_read(pdp, res.addr);

            res.value = w_read(pdp, res.addr);

            PRINT_RESULT("@%o(R%d) ", offset, num_register);
        }

        break;
    //мы еще не дописали другие моды
    default:
        ERROR("\nMode %d not implemented yet!\n", mode);
        exit(1);
    }
    return res;
}

/**
 * @brief 🌌 Мастер Декодирования: Извлечение Операндов Двухадресной Команды
 *
 * Эта функция — сердце конвейера PDP-11. Она препарирует 16-битное слово
 * команды, вырывая из него суть: информацию о том, "откуда" (Source)
 * придут данные и "куда" (Destination) они отправятся после исполнения.
 *
 * @param[in,out] pdp  Указатель на святая святых — структуру эмулятора
 * @ref pdp_11_t. Может измениться, если аргументы потребуют автоинкремента
 * регистров.
 * @param[in] word_command  Сырое 16-битное слово инструкции, считанное из
 * памяти.
 *
 * @return @ref op_code_t Структура, содержащая два полностью разрешенных
 * операнда (ss и dd).
 *
 * @note
 * В архитектуре PDP-11 для двухадресных команд:
 * - **Биты 11-6:** Определяют источник (Source).
 * - **Биты 5-0:**  Определяют приемник (Destination).
 *
 * @warning
 * Функция вызывает @ref __get_args, что может привести к побочным
 * эффектам: изменению указателя стека или программного счетчика (PC) при
 * использовании косвенной адресации!
 *
 * @code
 * // Пример того, как рождается магия:
 * word_t mov_instr = 012701; // MOV #val, R1
 * op_code_t op = __get_mr(pdp, mov_instr);
 * // Теперь op.ss знает всё о константе, а op.dd — о регистре R1.
 * @endcode
 */
op_code_t
__get_mr(struct pdp_11_t *pdp, word_t word_command, byte_t param)
{

    word_t temp_word_command = word_command;
    word_t flag              = 0;

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };

    // ss -откуда, dd - куда;
    // Выделяем 6 бит источника (сдвиг на 6 вправо)
    if (param & NO_PARAMS) {
        return opcode;
    }
    // if (param & (HAS_SS  HAS_DD)) {
    //     opcode.ss         = __get_args(pdp, word_command >> 6);
    //     opcode.dd         = __get_args(pdp, word_command);
    //     temp_word_command = temp_word_command >> 12;
    //     flag              = 1;
    // }
    if (param & HAS_SS) {
        opcode.ss         = __get_args(pdp, word_command >> 6);
        temp_word_command = temp_word_command >> 6;
    }
    // Выделяем 6 бит приемника (маскирование происходит внутри __get_args)
    if (param & HAS_DD) {
        opcode.dd         = __get_args(pdp, word_command);
        temp_word_command = temp_word_command >> 6;
    }

    // if (param & (HAS_DD)) {
    //     opcode.dd         = __get_args(pdp, temp_word_command & 077);
    //     temp_word_command = temp_word_command >> 6;
    // }
    // // Выделяем 6 бит приемника (маскирование происходит внутри __get_args)
    // if (param & (HAS_SS)) {
    //     opcode.ss         = __get_args(pdp, temp_word_command & 077);
    //     temp_word_command = temp_word_command >> 6;
    // }

    // Выделяем 6 бит приемника (маскирование происходит внутри __get_args)

    if (param & HAS_NN) {
        opcode.dd         = __get_args(pdp, temp_word_command & 077);
        temp_word_command = temp_word_command >> 6;
    }

    if (param & HAS_R) {
        opcode.dd         = __get_args(pdp, temp_word_command & 07);
        temp_word_command = temp_word_command >> 3;
    }
    // Выделяем 6 бит приемника (маскирование происходит внутри __get_args)

    if (param & HAS_N) {
        opcode.dd         = __get_args(pdp, temp_word_command & 07);
        temp_word_command = temp_word_command >> 3;
    }
    if (param & HAS_XX) {
        opcode.ss         = __get_args(pdp, temp_word_command & 0377);
        temp_word_command = temp_word_command >> 8;
    }
    /*
        TODO:
        B	0 for word, 1 for byte (1 bit)
        DD	Destination field (6 bits)
        N	Number (3 bits)
        NN	Number (6 bits)
        R	Register (3 bits, R0-5/SP/PC)
        SS	Source field (6 bits)
        TT	Number (8 bits)
        XX	Offset (8 bits, -128 to +127)*/

    return opcode;
}
