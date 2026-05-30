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
test_mode2(struct pdp_11_t *pdp)
{
    //! < Очистка регистрового файла
    address_word_t addr = 01000;

    w_write(pdp, addr, (word_t) 0012723);

    w_write(pdp, 0102, (word_t) 017);
    w_write(pdp, 0104, (word_t) 066);
    w_write(pdp, 01002, (word_t) 055);
    w_write(pdp, 01004, (word_t) 077);

    pdp_reg_set_var(pdp, 7, 01000);

    for (int i = 0; i <= 6; i++) {
        pdp_reg_set_var(pdp, i, 00);
    }
    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;

    //! < Привязка указателя на команду
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    //! @name Конфигурация режима автоинкремента
    //! @{
    pdp_reg_set_var(pdp, 3, 0100); // В R3 записан вдрес
    //! @}

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        //! < Декодирование аргументов инкрементной операции
        opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
    }

    PRINT_RESULT("\r                            ", "");
    TRACE("%s", "test_mode2 ");

    //! < Контроль расчетных значений декодера для автоинкремента

    assert(opcode.ss.value == 055);
    assert(opcode.ss.addr == 01002);
    assert(opcode.dd.value == 017);
    assert(opcode.dd.addr == 0102);
    assert(w_read(pdp, 0104) == 066);
    //! < Исполнение команды с триггером автоинкремента регистров
    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);

    //! < Проверка измененного состояния памяти после выполнения операции
    assert(pdp_reg_get_var(pdp, 3) == 0104);
    assert(w_read(pdp, pdp_reg_get_var(pdp, 3)) == 077);

    PRINT_RESULT("  %s", " ... OK\n");
}
