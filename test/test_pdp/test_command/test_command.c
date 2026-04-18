#include "tests/test_pdp/test_command/test_command.h"

#include "pdp_11/command/command_p.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "tests/test.h"
#include "tests/test_pdp/test_pdp.h"
#include "utils/logger/logger.h"
#include "utils/utils.h"

#include <assert.h>
#include <stdio.h>

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
    /* TODO Прочитать из файла программу и выполнить */

    assert(pdp);

    pdp_11_t *     ptr_pdp = (pdp_11_t *) pdp;
    address_word_t addr    = 01000;

    word_t *ptr_pc = ptr_pdp->PC;
    *ptr_pc        = addr;

    /* TESTS COMMAND */
    TRACE("\n\nTHE COMMAND TEST STARTED\n", "");

    WARNING("TEST MOV\n", "");
    // Тест команды mov;
    *ptr_pc = addr;
    w_write(pdp, addr, (word_t) 0010503);
    test_mov(pdp, ptr_pc);

    WARNING("TEST HALT\n", "");
    // Запись команды HALT (000000) по адресу addr и запуск теста
    *ptr_pc = addr;
    w_write(pdp, addr, (word_t) 0000001);
    test_halt(pdp, ptr_pc);

    // ptr_pc = do_command(pdp, ptr_pdp->command, *ptr_pc);

    return 0;
}
