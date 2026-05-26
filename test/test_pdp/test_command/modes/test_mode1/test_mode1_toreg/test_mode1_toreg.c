#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/pdp_11.h"
#include "tests/test_pdp/test_command/test_command.h"
#include "types/types.h"
#include "utils/logger/logger.h"

#include <assert.h>
#include <string.h>

void
test_mode1_toreg(struct pdp_11_t *pdp, const address_word_t addr)
{
    //! < Сбрасываем процессор в исходное состояние: PC = 01000, регистры R0-R6
    //! очищены
    pdp_reg_set_var(pdp, 7, 01000);
    for (int i = 0; i <= 6; i++) {
        pdp_reg_set_var(pdp, i, 00);
    }
    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;

    //! < Извлекаем внутренний указатель на исполняемую команду из таблицы
    //! команд PDP-11
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    //! @name Настройка окружения (Setup)
    //! @{
    pdp_reg_set_var(
        pdp, 02, 012); ///< В R2 помещаем адрес/значение приемника (DD)
    pdp_reg_set_var(pdp, 05, 0200); ///< В R5 помещаем адрес источника (0200)

    //! < Записываем в ячейку памяти 0200 эталонное значение 034
    w_write(pdp, 0200, 034);
    //! @}

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        //! < Читаем инструкцию из памяти по заданному адресу и парсим операнды
        opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
    }

    PRINT_RESULT("\r          ", "");
    TRACE("%s", "test_mode1_toreg  ");

    //! < Валидация декодера: проверяем, что данные считались из памяти по
    //! адресу из R5
    assert(opcode.ss.value == 034);
    assert(opcode.ss.addr == 0200);
    assert(opcode.dd.value == 012);
    assert(opcode.dd.addr == 02);

    //! < Эмулируем непосредственное выполнение команды процессором
    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);

    //! < Проверяем результат: значение 034 должно перекочевать в регистр R2
    //! (DD)
    assert(pdp_reg_get_var(pdp, 2) == 034);
    //! < Убеждаемся, что регистр-указатель R5 не изменил своего значения
    assert(pdp_reg_get_var(pdp, 5) == 0200);

    PRINT_RESULT("  %s", " ... OK\n");
}
