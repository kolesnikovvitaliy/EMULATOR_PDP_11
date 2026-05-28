#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/command/commands_list.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "utils/logger/logger.h"

#include <stdio.h>
#include <stdlib.h>

extern command_t template_commands[];

command_t **
command_new()
{
    return (command_t **) malloc(
        commands_list
        * sizeof(command_t *)); // Выделение паамяти для обЪекта command_t
}

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

void
command_destroy(command_t **commands)
{
    for (int i = 0; i < commands_list; i++) {
        free(commands[i]);
    }
}

void
__print_command(address_word_t addr, word_t word_command, byte_t *name_command)
{
    PRINT_RESULT("%06o %06o : %s", addr, word_command, name_command);
}

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
        pdp_reg_set_var(pdp, num_register, t_var_reg); // TODO: +1
        res.addr = pdp_reg_get_var(pdp, num_register); // в регистре адрес
        res.value = w_read(pdp, res.addr); // по адресу - значение
        // печать разной мнемоники для PC и других регистров
        if (num_register == 7) {
            PRINT_RESULT("#%o ", res.value);
        } else {
            PRINT_RESULT("(R%d)+ ", num_register);
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
