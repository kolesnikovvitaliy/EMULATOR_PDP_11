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
 * @brief Тестирование режима адресации 1 при копировании из регистра в память.
 * @details Данные из физического регистра процессора пересылаются по адресу в
 * памяти, который хранится в другом регистре.
 *
 * @param[in,out] pdp  Указатель на структуру виртуальной машины PDP-11.
 * @param[in]     addr Адрес тестируемой команды в памяти.
 */
void
test_mode1_reg_to_mem(struct pdp_11_t *pdp, const address_word_t addr)
{
    //! < Первичная инициализация системных регистров общего назначения
    pdp_reg_clear(pdp);
    pdp_reg_set_var(pdp, 7, 01000);

    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;

    //! < Поиск и привязка контекста исполняемой команды
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    //! @name Подготовка тестовых данных
    //! @{
    pdp_reg_set_var(
        pdp, 03, 066); ///< Источник (SS): регистр R3 содержит значение 066
    pdp_reg_set_var(
        pdp,
        05,
        0200); ///< Приемник (DD): регистр R5 хранит целевой адрес 0200

    //! < Инициализируем память по адресу назначения мусорным значением
    w_write(pdp, 0200, 034);
    //! @}

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        //! < Извлечение битовой маски операции
        opcode
            = __get_mr(pdp, (word_t) w_read(pdp, addr), run_command->params);
    }
    // PRINT_RESULT("\x1b[F", "");
    PRINT_RESULT("\r          ", "");
    PRINT_RESULT("\x1b[F", "");
    TRACE("%s", "test_mode1_reg_to_mem  ");

    //! < Контроль декодирования: SS ссылается на R3, DD указывает на адрес
    //! 0200 в памяти
    assert(opcode.ss.value == 066);
    assert(opcode.ss.addr == 03);
    assert(opcode.dd.value == 034);
    assert(opcode.dd.addr == 0200);

    //! < Запуск выполнения логики команды
    run_command->do_commands_command(
        pdp, addr, w_read(pdp, addr), run_command->params);

    //! < Финальная проверка: значение 066 обязано записаться в ячейку памяти
    //! 0200
    assert(w_read(pdp, opcode.dd.addr) == 066);

    PRINT_RESULT("  %s", " ... OK\n");
}
