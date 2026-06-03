/**
 * @file test_command.c
 * @brief Запуск тестов исполнения команд процессором.
 */
#include "tests/test_pdp/test_command/test_command.h"

#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "tests/test.h"
#include "tests/test_pdp/test_command/modes/test_mode0/test_mode0.h"
#include "tests/test_pdp/test_command/modes/test_mode1/test_mode1.h"
#include "tests/test_pdp/test_command/modes/test_mode2/test_mode2.h"
#include "tests/test_pdp/test_pdp.h"
#include "utils/logger/logger.h"
#include "utils/utils.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Тестирование выполнения команд процессора PDP-11.
 *
 * Функция загружает тестовую последовательность команд в память (на данный
 * момент жестко задано по адресу 01000) и запускает их выполнение для проверки
 * корректности работы эмулятора.
 *
 * @param[in,out] pdp Указатель на структуру состояния процессора PDP-11.
 *
 * @return Всегда возвращает 0 при завершении теста.
 *
 * @todo Реализовать чтение тестовой программы из внешнего файла.
 * @todo Раскомментировать и интегрировать основной цикл выполнения do_command.
 */

int
test_command(struct pdp_11_t *pdp)
{
    assert(pdp);

    pdp_11_t *     ptr_pdp = (pdp_11_t *) pdp;
    address_word_t addr    = 01000;

    word_t *ptr_pc = ptr_pdp->PC;
    *ptr_pc        = addr;

    //----------------------------------------------------------------------;
    INFO("THE COMMAND TEST STARTED", "");
    //////////////////////////////////////////////////////////////////////////;

    // TODO
    // test_mode2();

    // Тест команды mov;
    // w_write(pdp, addr, (word_t) 0010503);
    test_mode0(pdp);
    test_mode1(pdp);
    test_mode2(pdp);
    PRINT_RESULT("\x1b[F", "");
    INFO("THE COMMAND TEST WAS SUCCESSFUL\n", "");

    return 0;
}
// Вспомагательна функция получения команды: для тестов;
command_t *
__ptr_command(pdp_11_t *pdp, command_t **commands, const address_word_t addr)
{
    extern byte_t commands_list;
    word_t *      ptr_pc = pdp->PC;
    *ptr_pc              = addr;
    word_t word_command;

    word_command = w_read((struct pdp_11_t *) pdp, *ptr_pc);

    for (int i = 1; i < commands_list; i++) {
        if ((word_command & commands[i]->mask) == commands[i]->opcode) {
            return (command_t *) commands[i];
        }
    }

    return (command_t *) commands[0];
}
