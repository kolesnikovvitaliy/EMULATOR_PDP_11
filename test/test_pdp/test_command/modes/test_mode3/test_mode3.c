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
test_mode3(struct pdp_11_t *pdp)
{
    //! < Очистка регистрового файла
    address_word_t addr = 01000;

    pdp_reg_clear(pdp);
    pdp_reg_set_var(pdp, 7, addr);

    w_write(pdp, 01000, (word_t) 0013703);
    w_write(pdp, 01002, (word_t) 0202);
    w_write(pdp, 0202, (word_t) 017);

    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        //! < Декодирование аргументов инкрементной операции
        opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
    }
    //! < Привязка указателя на команду
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    PRINT_RESULT("\r                            ", "");
    PRINT_RESULT("\x1b[F", "");
    TRACE("%s", "test_mode3 ");

    //! < Контроль расчетных значений декодера для автоинкремента
    //! < Исполнение команды с триггером автоинкремента регистров

    assert(opcode.ss.value == 017);
    assert(opcode.ss.addr == 0202);
    assert(opcode.dd.value == 00);
    assert(opcode.dd.addr == 03);

    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);

    //! < Проверка измененного состояния памяти после выполнения операции
    assert(pdp_reg_get_var(pdp, opcode.dd.addr) == opcode.ss.value);
    //__command_reg_dump(pdp);

    PRINT_RESULT("  %s", " ... OK\n");
}
