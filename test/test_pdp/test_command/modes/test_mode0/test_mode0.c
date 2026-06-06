#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/pdp_11.h"
#include "tests/test_pdp/test_command/test_command.h"
#include "types/types.h"
#include "utils/logger/logger.h"

#include <assert.h>
#include <string.h>

/**
 * @brief Тестирование режима адресации 0 (Регистровый режим).
 * @details Проверяет работу с операндами, когда данные находятся
 * непосредственно в регистрах процессора, без обращения к памяти.
 *
 * @param[in,out] pdp Указатель на структуру виртуальной машины PDP-11.
 */

void
test_mode0(struct pdp_11_t *pdp)
{
    address_word_t addr = 01000;

    pdp_reg_clear(pdp);
    pdp_reg_set_var(pdp, 7, 01000);

    w_write(pdp, addr, (word_t) 0010503);

    //! < Инициализируем счетчик команд (R7 / PC) базовым адресом 01000

    PRINT_RESULT("\r\n", "");

    //! < Заполняем регистры-операнды тестовыми восьмеричными значениями
    pdp_reg_set_var(pdp, 3, 012); ///< Данные приемника (Destination) в R3
    pdp_reg_set_var(pdp, 5, 034); ///< Данные источника (Source) в R5

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        //! < Декодируем макет инструкции (0010503) для извлечения модов и
        //! номеров регистров
        opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
    }

    PRINT_RESULT("\r      ", "");
    PRINT_RESULT("\x1b[F", "");
    //! < Логируем запуск теста нулевого режима адресации
    TRACE("%s\t", "test_mode0");

    //! < Проверяем корректность декодирования источника (SS): R5 с содержимым
    //! 034
    assert(opcode.ss.value == 034);
    assert(opcode.ss.addr == 05);

    //! < Проверяем корректность декодирования приемника (DD): R3 с содержимым
    //! 012
    assert(opcode.dd.value == 012);
    assert(opcode.dd.addr == 03);

    //! < Тест Mode 0 успешно пройден
    PRINT_RESULT("%s", " ... OK\n\r");
}
