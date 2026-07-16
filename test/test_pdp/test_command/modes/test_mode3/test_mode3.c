#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "tests/test_pdp/test_command/test_command.h"
#include "types/types.h"
#include "utils/logger/logger.h"

#include <assert.h>
#include <string.h>
/**
 * @brief Тестирование режима адресации 2 (Автоинкрементный режим).
 * @details Проверяет чтение/запись данных с последующим автоматическим
 * увеличением регистра-указателя на размер переданного типа (байт/слово).
 *
 * @param[in,out] pdp  Указатель на структуру виртуальной машины PDP-11.
 * @param[in]     addr Адрес тестируемой команды в памяти.
 */

void
test_mode_3_valid_register(struct pdp_11_t *pdp)
{
    //! < Очистка регистрового файла
    address_word_t addr = 01000;

    pdp_reg_clear(pdp);
    pdp_reg_set_var(pdp, 7, addr);

    w_write(pdp, 01000, (word_t) /*0013703*/ 0013301); // 0013303
    pdp_reg_set_var(pdp, 3, 01002);
    // PRINT_RESULT("\npdp_reg_get_addr(pdp, 3) = %o\n",
    //              pdp_reg_get_addr(pdp, 3));
    w_write(pdp, 01002, (word_t) 0112);
    w_write(pdp, 0112, (word_t) 017);

    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);
    // OP_CODE_T_INIT
    // if (pdp) {
    //     //! < Декодирование аргументов инкрементной операции
    //     opcode
    //         = __get_mr(pdp, (word_t) w_read(pdp, addr),
    //         run_command->params);
    // }
    //! < Привязка указателя на команду
    // run_command = __ptr_command(
    //     (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    PRINT_RESULT("\r                            ", "");
    PRINT_RESULT("\x1b[F", "");
    TRACE("%s", "test_mode3 ");

    //! < Контроль расчетных значений декодера для автоинкремента
    //! < Исполнение команды с триггером автоинкремента регистров

    // assert(opcode.ss.value == 017);
    // assert(opcode.ss.addr == 0202);
    // assert(opcode.dd.value == 00);
    // assert(opcode.dd.addr == 03);
    // PRINT_RESULT("\nopcode.ss.value%o\n", opcode.ss.value);
    // PRINT_RESULT("\nopcode.ss.addr%o\n", opcode.ss.addr);
    // PRINT_RESULT("\nopcode.dd.value%o\n", opcode.dd.value);
    // PRINT_RESULT("\nopcode.dd.addr%o\n", opcode.dd.addr);
    // pdp_reg_set_var(pdp, 7, (word_t)(pdp_reg_get_var(pdp, 7) - 2));
    run_command->do_commands_command(
        pdp, addr, w_read(pdp, addr), run_command->params);

    //! < Проверка измененного состояния памяти после выполнения операции
    // PRINT_RESULT("\nopcode.ss.value%o\n", opcode.ss.value);
    // PRINT_RESULT("\nopcode.ss.addr%o\n", opcode.ss.addr);
    // PRINT_RESULT("\nopcode.dd.value%o\n", opcode.dd.value);
    // PRINT_RESULT("\nopcode.dd.addr%o\n", opcode.dd.addr);
    assert(pdp_reg_get_var(pdp, 1) == 017);
    assert(pdp_reg_get_var(pdp, 3) == 01004);
    assert(pdp_reg_get_var(pdp, 7) == 01000);
    // pdp_mem_dump(pdp, 0x40, 0x26);
    // pdp_mem_dump(pdp, 0x200, 0x26);
    // __command_reg_dump(pdp);

    PRINT_RESULT("  %s", " ... OK\n");
}

void
test_mode_3_register_ps(struct pdp_11_t *pdp)
{
    //! < Очистка регистрового файла
    address_word_t addr = 01004;

    pdp_reg_clear(pdp);
    pdp_reg_set_var(pdp, 7, addr);

    w_write(pdp, addr, (word_t) 0013701);
    w_write(pdp, 01006, (word_t) 0202);
    w_write(pdp, 0202, (word_t) 05);

    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);
    // OP_CODE_T_INIT
    // if (pdp) {
    //     //! < Декодирование аргументов инкрементной операции
    //     opcode
    //         = __get_mr(pdp, (word_t) w_read(pdp, addr),
    //         run_command->params);
    // }
    //! < Привязка указателя на команду
    // run_command = __ptr_command(
    //     (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    PRINT_RESULT("\r                            ", "");
    PRINT_RESULT("\x1b[F", "");
    TRACE("%s", "test_mode3_reg7 ");

    //! < Контроль расчетных значений декодера для автоинкремента
    //! < Исполнение команды с триггером автоинкремента регистров

    // assert(opcode.ss.value == 017);
    // assert(opcode.ss.addr == 0202);
    // assert(opcode.dd.value == 00);
    // assert(opcode.dd.addr == 03);
    // PRINT_RESULT("\nopcode.ss.value%o\n", opcode.ss.value);
    // PRINT_RESULT("\nopcode.ss.addr%o\n", opcode.ss.addr);
    // PRINT_RESULT("\nopcode.dd.value%o\n", opcode.dd.value);
    // PRINT_RESULT("\nopcode.dd.addr%o\n", opcode.dd.addr);
    // pdp_reg_set_var(pdp, 7, (word_t)(pdp_reg_get_var(pdp, 7) - 2));
    run_command->do_commands_command(
        pdp, addr, w_read(pdp, addr), run_command->params);

    //! < Проверка измененного состояния памяти после выполнения операции
    // PRINT_RESULT("\nopcode.ss.value%o\n", opcode.ss.value);
    // PRINT_RESULT("\nopcode.ss.addr%o\n", opcode.ss.addr);
    // PRINT_RESULT("\nopcode.dd.value%o\n", opcode.dd.value);
    // PRINT_RESULT("\nopcode.dd.addr%o\n", opcode.dd.addr);
    assert(pdp_reg_get_var(pdp, 1) == 05);
    assert(pdp_reg_get_var(pdp, 7) == 01006);
    // pdp_mem_dump(pdp, 0x40, 0x26);
    // pdp_mem_dump(pdp, 0x200, 0x26);
    // __command_reg_dump(pdp);

    PRINT_RESULT("  %s", " ... OK\n");
}

void
test_mode3(struct pdp_11_t *pdp)
{
    test_mode_3_valid_register(pdp);
    test_mode_3_register_ps(pdp);
    pdp_reg_clear(pdp);
}
