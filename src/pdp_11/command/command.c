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
command_new()
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
    word_t num_register = word_command & 7;
    byte_t mode         = (word_command >> 3) & 7;

    switch (mode) {
    case 0:
        res.addr  = num_register;
        res.value = pdp_reg_get_var(pdp, res.addr);

        // ss -откуда, dd - куда;

        PRINT_RESULT("R%d ", res.addr);

        break;
    // мода 1, (R1)
    case 1:
        res.addr = pdp_reg_get_var(pdp, num_register); // в регистре адрес
        res.value = w_read(pdp, res.addr); // по адресу - значение
        // ss -откуда, dd - куда;

        PRINT_RESULT("(R%d) ", num_register);
        break;
    // мода 2, (R1)+ или #3
    case 2:;
        word_t t_var_reg = pdp_reg_get_var(pdp, num_register);
        t_var_reg        = (word_t)(t_var_reg + 2);
        res.addr         = t_var_reg;

        pdp_reg_set_var(
            pdp, num_register, (address_word_t)(res.addr)); // TODO: +1

        // печать разной мнемоники для PC и других регистров
        if (num_register == 7) {
            res.value = w_read(pdp, (address_word_t)(res.addr));
            PRINT_RESULT("#%o ", res.value);
        } else {
            res.value = w_read(pdp, (address_word_t)(res.addr - 2));
            PRINT_RESULT("(R%d)+ ", num_register);
        }
        break;
    case 3:; //  для объявления типа данных word_t требуется " ; " после метки
        word_t temp_value_register
            = (word_t) pdp_reg_get_var(pdp, num_register);

        address_word_t addr_1
            = (address_word_t)(temp_value_register + 2); // adr = reg[n]

        // ss -откуда, dd - куда;

        if (num_register == 7) {

            res.addr = w_read(pdp, (address_word_t)(addr_1));

            res.value = w_read(
                pdp, (address_word_t)(res.addr)); // добавилось еще одно
            pdp_reg_set_var(
                pdp, num_register, (address_word_t)(temp_value_register + 2));
            PRINT_RESULT("@#%o ", res.value);

        } else {

            res.addr = w_read(pdp, (address_word_t)(addr_1));

            res.value
                = w_read(pdp,
                         (address_word_t)(
                             res.addr)); // добавилось еще одно разыменование
            pdp_reg_set_var(
                pdp, num_register, (address_word_t)(temp_value_register));
            PRINT_RESULT("\n@(R%d)+ ", num_register);
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
__get_mr(struct pdp_11_t *pdp, word_t word_command)
{
    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    // ss -откуда, dd - куда;
    // Выделяем 6 бит источника (сдвиг на 6 вправо)
    opcode.ss = __get_args(pdp, word_command >> 6);

    // Выделяем 6 бит приемника (маскирование происходит внутри __get_args)
    opcode.dd = __get_args(pdp, word_command);
    return opcode;
}
