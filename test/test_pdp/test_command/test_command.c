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
int
test_command(struct pdp_11_t *pdp)
{
    /* TODO Прочитать из файла программу и выполнить */

    assert(pdp);

    pdp_11_t *     ptr_pdp = (pdp_11_t *) pdp;
    address_word_t addr    = 01000;

    word_t *ptr_pc = ptr_pdp->PC;
    *ptr_pc        = addr;
    // command_t *run_command;
    /* TESTS COMMAND */
    TRACE("\nTHE COMMAND TEST STARTED\n", "");

    //----------------------------------------------------------------------
    //////////////////////////////////////////////////////////////////////////
    WARNING("TEST MOV\n", "");
    // Тест команды mov;
    //*ptr_pc = addr;
    w_write(pdp, addr, (word_t) 0010503);
    /*run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    assert(strcmp((char *) run_command->name, "mov") == 0);

    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);
    *///
    test_mov(pdp, addr);
    ////////////////////////////////////////////////////////////////////////////
    //---------------------------------------------------------------------------

    WARNING("TEST HALT\n", "");
    // Запись команды HALT (000000) по адресу addr и запуск теста
    *ptr_pc = addr;
    w_write(pdp, addr, (word_t) 0000001);
    test_halt(pdp, ptr_pc);

    // ptr_pc = do_command(pdp, ptr_pdp->command, *ptr_pc);

    return 0;
}
