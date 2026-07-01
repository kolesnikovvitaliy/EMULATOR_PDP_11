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

    w_write(pdp, addr, (word_t) 0014301)

    pdp_reg_set_var(pdp, 3, 01004);
    w_write(pdp, 01002, (word_t) 05);

    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;
    //! < Привязка указателя на команду
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    PRINT_RESULT("\r                            ", "");
    PRINT_RESULT("\x1b[F", "");
    TRACE("%s", "test_mode4 ");

    //! < Контроль расчетных значений декодера для автодекремента
    //! < Исполнение команды с триггером автодекремента регистров

    run_command->do_commands_command(
        pdp, addr, w_read(pdp, addr), run_command->params);

    //! < Проверка измененного состояния памяти после выполнения операции
    assert(pdp_reg_get_var(pdp, 1) == 05);
    assert(pdp_reg_get_var(pdp, 3) == 01002);

    //__command_reg_dump(pdp);

    PRINT_RESULT("  %s", " ... OK\n");
}
void
test_mode4(struct pdp_11_t *pdp)
{
    test_1(pdp);
    return;
}
