#include "tests/test_pdp/test_command/test_command.h"

#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "tests/test.h"
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
    TRACE("\nTHE COMMAND TEST STARTED\n", "");
    //////////////////////////////////////////////////////////////////////////;
    // Тест команды mov;
    w_write(pdp, addr, (word_t) 0010503);
    test_mov(pdp, addr);
    /////////////////////////////////////////////////////////////////////////;
    // Тест команды hall
    w_write(pdp, addr, (word_t) 0000001);
    test_halt(pdp, ptr_pc);
    //----------------------------------------------------------------------;

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
            // PRINT_RESULT(
            //    "%06o %06o : %s ", addr, word_command, commands[i]->name);
            return (command_t *) commands[i];
        }
    }

    return (command_t *) commands[0];
}
