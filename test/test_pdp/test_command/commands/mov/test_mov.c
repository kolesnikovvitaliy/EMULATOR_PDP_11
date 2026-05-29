#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/pdp_11.h"
#include "tests/test_pdp/test_command/test_command.h"
#include "types/types.h"
#include "utils/logger/logger.h"
// #include "utils/logger/logger.h"
// #include "utils/utils.h"
//
#include <assert.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <string.h>

/**
 * @brief Тестирование корректности парсинга имени команды "mov".
 * @details Функция проверяет, что переданная команда была распознана парсером
 * именно как инструкция пересылки данных (MOV).
 *
 * @param[in] ptr_command Указатель на структуру тестируемой команды.
 */
void
test_parse_mov(command_t *ptr_command)
{
    //! < Выводим в лог имя текущего теста для трассировки
    TRACE("%s\t", "test_parse_mov");

    //! < Критическая проверка: имя команды обязано строго соответствовать
    //! "mov"
    assert(strcmp((char *) ptr_command->name, "mov") == 0);

    //! < Сигнализируем об успешном завершении проверки парсера
    PRINT_RESULT("%s", " ... OK");
}

/**
 * @brief Главная управляющая функция (главный тест) для инструкции MOV.
 * @details Агрегирует выполнение всех подтестов для различных режимов
 * адресации инструкции пересылки данных (MOV) и проверяет базовую логику.
 *
 * @param[in,out] pdp  Указатель на структуру виртуальной машины PDP-11.
 * @param[in]     addr Базовый адрес команды MOV в эмулируемой памяти.
 * @return int Возвращает 0 при успешном завершении всех тестов.
 */
int
test_mov(struct pdp_11_t *pdp, const address_word_t addr)
{
    //! < Первоначальная установка регистра PC на тестовый адрес
    pdp_reg_set_var(pdp, 7, 01000);
    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;

    //! < Получаем контекст команды из пула команд эмулятора
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    //! < Запуск подтеста 1: Проверка корректности парсинга строки "mov"
    test_parse_mov(run_command);

    //! < Запуск подтеста 2: Проверка работы команды в режиме 0
    //! (регистр-регистр)
    // test_mode0(pdp);

    TRACE("%s", "test_move  ");
    //! < Выполнение основной тестируемой операции MOV
    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);

    //! @name Финальная верификация результатов работы MOV
    //! @{
    assert(pdp_reg_get_var(pdp, 3)
           == 034); ///< Значение приемника (R3) должно стать 034
    assert(pdp_reg_get_var(pdp, 5)
           == 034); ///< Значение источника (R5) должно остаться 034
    //! @}

    PRINT_RESULT("  %s", "... OK\n");
    return 0;
}
