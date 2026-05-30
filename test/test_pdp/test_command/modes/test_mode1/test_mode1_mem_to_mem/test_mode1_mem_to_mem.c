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
 * @brief Тестирование режима адресации 1 при копировании из памяти в память.
 * @details Самый сложный случай для Mode 1: чтение по указателю из одного
 * регистра и запись по указателю в другой регистр.
 *
 * @param[in,out] pdp  Указатель на структуру виртуальной машины PDP-11.
 * @param[in]     addr Адрес тестируемой команды в памяти.
 */
void
test_mode1_mem_to_mem(struct pdp_11_t *pdp, const address_word_t addr)
{
    //! < Стандартный сброс процессора перед симуляцией
    pdp_reg_set_var(pdp, 7, 01000);
    for (int i = 0; i <= 6; i++) {
        pdp_reg_set_var(pdp, i, 00);
    }
    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;

    //! < Получение дескриптора команды
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    //! @name Настройка ячеек ОЗУ и указателей
    //! @{
    pdp_reg_set_var(pdp, 1, 0200); // R1 указывает на источник (0200)
    pdp_reg_set_var(pdp, 4, 0210); // R4 указывает на приемник (0210)

    w_write(pdp, 0200, 055); ///< Данные, которые будем копировать
    w_write(pdp, 0210, 011); ///< Старые данные в целевой ячейке
    //! @}

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        //! < Чтение и разбор опкода
        opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
    }

    PRINT_RESULT("\r          ", "");
    TRACE("%s", "test_mode1_mem_to_mem  ");

    //! < Проверяем корректность связки "Регистр -> Адрес памяти -> Значение"
    assert(opcode.ss.value == 055);
    assert(opcode.ss.addr == 0200);
    assert(opcode.dd.value == 011);
    assert(opcode.dd.addr == 0210);

    //! < Выполнение операции пересылки MOV memory -> memory
    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);

    //! < Проверяем, что значение 055 успешно скопировано в адрес назначения
    //! (0210)
    assert(w_read(pdp, opcode.dd.addr) == 055);

    PRINT_RESULT("  %s", " ... OK\n");
}
