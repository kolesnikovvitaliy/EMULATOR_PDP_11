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
test_1(struct pdp_11_t *pdp)
{
    //! < Очистка регистрового файла
    address_word_t addr = 01000;

    pdp_reg_clear(pdp);
    pdp_reg_set_var(pdp, 7, addr);

    w_write(pdp, addr, (word_t) 0014301); // 0014341

    pdp_reg_set_var(pdp, 3, 01004);
    w_write(pdp, 01002, (word_t) 05);

    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);
    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        //! < Декодирование аргументов инкрементной операции
        opcode
            = __get_mr(pdp, (word_t) w_read(pdp, addr), run_command->params);
    }
    //! < Привязка указателя на команду
    // run_command = __ptr_command(
    //     (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    PRINT_RESULT("\r                            ", "");
    PRINT_RESULT("\x1b[F", "");
    TRACE("%s", "test_mode4 ");

    //! < Контроль расчетных значений декодера для автоинкремента
    //! < Исполнение команды с триггером автоинкремента регистров

    // PRINT_RESULT("\nopcode.ss.value%o\n", opcode.ss.value);
    // PRINT_RESULT("\nopcode.ss.addr%o\n", opcode.ss.addr);
    // PRINT_RESULT("\nopcode.dd.value%o\n", opcode.dd.value);
    // PRINT_RESULT("\nopcode.dd.addr%o\n", opcode.dd.addr);
    // pdp_reg_set_var(pdp, 7, (word_t)(pdp_reg_get_var(pdp, 7) - 2));
    run_command->do_commands_command(
        pdp, addr, w_read(pdp, addr), run_command->params);
    assert(opcode.ss.value == 05);
    assert(opcode.ss.addr == 01002);
    assert(opcode.dd.value == 00);
    assert(opcode.dd.addr == 01);
    // //! < Проверка измененного состояния памяти после выполнения операции

    assert(pdp_reg_get_var(pdp, 3) - 2 == opcode.ss.addr);
    //__command_reg_dump(pdp);

    PRINT_RESULT("  %s", " ... OK\n");
}
void
test_mode4(struct pdp_11_t *pdp)
{
    test_1(pdp);
    return;
}
