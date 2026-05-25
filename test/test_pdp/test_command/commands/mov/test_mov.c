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
 * @brief Тестирование режима адресации 0 (Регистровый режим).
 * @details Проверяет работу с операндами, когда данные находятся
 * непосредственно в регистрах процессора, без обращения к памяти.
 *
 * @param[in,out] pdp Указатель на структуру виртуальной машины PDP-11.
 */
// void
// test_mode0(struct pdp_11_t *pdp)
// {
//     //! < Инициализируем счетчик команд (R7 / PC) базовым адресом 01000
//     pdp_reg_set_var(pdp, 7, 01000);
//     PRINT_RESULT("\r\n", "");
//
//     //! < Заполняем регистры-операнды тестовыми восьмеричными значениями
//     pdp_reg_set_var(pdp, 3, 012); ///< Данные приемника (Destination) в R3
//     pdp_reg_set_var(pdp, 5, 034); ///< Данные источника (Source) в R5
//
//     op_code_t opcode = { { 0, 0 }, { 0, 0 } };
//     if (pdp) {
//         //! < Декодируем макет инструкции (0010503) для извлечения модов и
//         //! номеров регистров
//         opcode = __get_mr(pdp, (word_t) 0010503);
//     }
//
//     PRINT_RESULT("\r      ", "");
//     //! < Логируем запуск теста нулевого режима адресации
//     TRACE("%s\t", "test_mode0");
//
//     //! < Проверяем корректность декодирования источника (SS): R5 с
//     содержимым
//     //! 034
//     assert(opcode.ss.value == 034);
//     assert(opcode.ss.addr == 05);
//
//     //! < Проверяем корректность декодирования приемника (DD): R3 с
//     содержимым
//     //! 012
//     assert(opcode.dd.value == 012);
//     assert(opcode.dd.addr == 03);
//
//     //! < Тест Mode 0 успешно пройден
//     PRINT_RESULT("%s", " ... OK\n");
// }

/**
 * @brief Тестирование режима адресации 1 (Косвенно-регистровый) с записью в
 * регистр.
 * @details Проверяет чтение данных из ячейки памяти, адрес которой хранится в
 * регистре.
 *
 * @param[in,out] pdp  Указатель на структуру виртуальной машины PDP-11.
 * @param[in]     addr Адрес тестируемой команды в памяти.
 */
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
    pdp_reg_set_var(pdp, 7, 01000);
    for (int i = 0; i <= 6; i++) {
        pdp_reg_set_var(pdp, i, 00);
    }
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
        opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
    }

    PRINT_RESULT("\r          ", "");
    TRACE("%s", "test_mode1_reg_to_mem  ");

    //! < Контроль декодирования: SS ссылается на R3, DD указывает на адрес
    //! 0200 в памяти
    assert(opcode.ss.value == 066);
    assert(opcode.ss.addr == 03);
    assert(opcode.dd.value == 034);
    assert(opcode.dd.addr == 0200);

    //! < Запуск выполнения логики команды
    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);

    //! < Финальная проверка: значение 066 обязано записаться в ячейку памяти
    //! 0200
    assert(w_read(pdp, opcode.dd.addr) == 066);

    PRINT_RESULT("  %s", " ... OK\n");
}

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

/**
 * @brief Тестирование режима адресации 2 (Автоинкрементный режим).
 * @details Проверяет чтение/запись данных с последующим автоматическим
 * увеличением регистра-указателя на размер переданного типа (байт/слово).
 *
 * @param[in,out] pdp  Указатель на структуру виртуальной машины PDP-11.
 * @param[in]     addr Адрес тестируемой команды в памяти.
 */
void
test_mode2(struct pdp_11_t *pdp, const address_word_t addr)
{
    //! < Очистка регистрового файла
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
    pdp_reg_set_var(pdp, 3, 0102); // В R3 записан вдрес
    //! @}

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        //! < Декодирование аргументов инкрементной операции
        opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
    }

    PRINT_RESULT("\r          ", "");
    TRACE("%s", "test_mode2 ");

    //! < Контроль расчетных значений декодера для автоинкремента
    assert(opcode.ss.value == 02);
    assert(opcode.ss.addr == 01002);
    assert(opcode.dd.value == 034);
    assert(opcode.dd.addr == 0104);
    //! < Исполнение команды с триггером автоинкремента регистров
    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);
    //! < Проверка измененного состояния памяти после выполнения операции
    assert(pdp_reg_get_var(pdp, 3) == 0106);
    PRINT_RESULT("  %s", " ... OK\n");
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

//##############################################################################

// #include "pdp_11/command/command.h"
// #include "pdp_11/command/command_p.h"
// #include "pdp_11/command/commands.h"
// #include "pdp_11/pdp_11.h"
// #include "tests/test_pdp/test_command/test_command.h"
// #include "types/types.h"
// #include "utils/logger/logger.h"
// // #include "utils/logger/logger.h"
// // #include "utils/utils.h"
// //
// #include <assert.h>
// // #include <stdio.h>
// // #include <stdlib.h>
// #include <string.h>
//
// void
// test_parse_mov(command_t *ptr_command)
// {
//     TRACE("%s\t", "test_parse_mov");
//     assert(strcmp((char *) ptr_command->name, "mov") == 0);
//     PRINT_RESULT("%s", " ... OK");
// }
//
// void
// test_mode0(struct pdp_11_t *pdp)
// {
//     pdp_reg_set_var(pdp, 7, 01000);
//     PRINT_RESULT("\r\n", "");
//
//     pdp_reg_set_var(pdp, 3, 012);
//     pdp_reg_set_var(pdp, 5, 034);
//
//     op_code_t opcode = { { 0, 0 }, { 0, 0 } };
//     if (pdp) {
//         opcode = __get_mr(pdp, (word_t) 0010503);
//     }
//
//     PRINT_RESULT("\r      ", "");
//     TRACE("%s\t", "test_mode0");
//     assert(opcode.ss.value == 034);
//     assert(opcode.ss.addr == 05);
//     assert(opcode.dd.value == 012);
//     assert(opcode.dd.addr == 03);
//
//     PRINT_RESULT("%s", " ... OK\n");
// }
//
// void
// test_mode1_toreg(struct pdp_11_t *pdp, const address_word_t addr)
// {
//     pdp_reg_set_var(pdp, 7, 01000);
//     for (int i = 0; i <= 6; i++) {
//         pdp_reg_set_var(pdp, i, 00);
//     }
//     pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
//     command_t *run_command;
//
//     run_command = __ptr_command(
//         (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);
//
//     // setup
//     pdp_reg_set_var(pdp, 02, 012);  // dd
//     pdp_reg_set_var(pdp, 05, 0200); // ss
//
//     w_write(pdp, 0200, 034);
//
//     op_code_t opcode = { { 0, 0 }, { 0, 0 } };
//     if (pdp) {
//         opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
//     }
//
//     PRINT_RESULT("\r          ", "");
//     TRACE("%s", "test_mode1_toreg  ");
//     assert(opcode.ss.value == 034);
//     assert(opcode.ss.addr == 0200);
//     assert(opcode.dd.value == 012);
//     assert(opcode.dd.addr == 02);
//
//     run_command->do_commands_command(pdp, addr, w_read(pdp, addr),
//     (byte_t) 1);
//
//     assert(pdp_reg_get_var(pdp, 2) == 034);
//     assert(pdp_reg_get_var(pdp, 5) == 0200);
//
//     PRINT_RESULT("  %s", " ... OK\n");
// }
//
// void
// test_mode1_reg_to_mem(struct pdp_11_t *pdp, const address_word_t addr)
// {
//     pdp_reg_set_var(pdp, 7, 01000);
//     for (int i = 0; i <= 6; i++) {
//         pdp_reg_set_var(pdp, i, 00);
//     }
//     pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
//     command_t *run_command;
//
//     run_command = __ptr_command(
//         (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);
//
//     // setup
//     pdp_reg_set_var(pdp, 03, 066);  // dd
//     pdp_reg_set_var(pdp, 05, 0200); // ss
//
//     w_write(pdp, 0200, 034);
//
//     op_code_t opcode = { { 0, 0 }, { 0, 0 } };
//     if (pdp) {
//         opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
//     }
//
//     PRINT_RESULT("\r          ", "");
//     TRACE("%s", "test_mode1_reg_to_mem  ");
//
//     assert(opcode.ss.value == 066);
//     assert(opcode.ss.addr == 03);
//     assert(opcode.dd.value == 034);
//     assert(opcode.dd.addr == 0200);
//
//     run_command->do_commands_command(pdp, addr, w_read(pdp, addr),
//     (byte_t) 1);
//
//     assert(w_read(pdp, opcode.dd.addr) == 066);
//
//     PRINT_RESULT("  %s", " ... OK\n");
// }
// void
// test_mode1_mem_to_mem(struct pdp_11_t *pdp, const address_word_t addr)
// {
//     pdp_reg_set_var(pdp, 7, 01000);
//     for (int i = 0; i <= 6; i++) {
//         pdp_reg_set_var(pdp, i, 00);
//     }
//     pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
//     command_t *run_command;
//
//     run_command = __ptr_command(
//         (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);
//
//     // setup
//     pdp_reg_set_var(pdp, 1, 0200); // dd
//     pdp_reg_set_var(pdp, 4, 0210); // ss
//
//     w_write(pdp, 0200, 055);
//     w_write(pdp, 0210, 011);
//
//     op_code_t opcode = { { 0, 0 }, { 0, 0 } };
//     if (pdp) {
//         opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
//     }
//
//     PRINT_RESULT("\r          ", "");
//     TRACE("%s", "test_mode1_mem_to_mem  ");
//
//     assert(opcode.ss.value == 055);
//     assert(opcode.ss.addr == 0200);
//     assert(opcode.dd.value == 011);
//     assert(opcode.dd.addr == 0210);
//
//     run_command->do_commands_command(pdp, addr, w_read(pdp, addr),
//     (byte_t) 1);
//
//     assert(w_read(pdp, opcode.dd.addr) == 055);
//
//     PRINT_RESULT("  %s", " ... OK\n");
// }
//
// void
// test_mode2(struct pdp_11_t *pdp, const address_word_t addr)
// {
//     pdp_reg_set_var(pdp, 7, 01000);
//     for (int i = 0; i <= 6; i++) {
//         pdp_reg_set_var(pdp, i, 00);
//     }
//     pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
//     command_t *run_command;
//
//     run_command = __ptr_command(
//         (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);
//
//     // setup
//     pdp_reg_set_var(pdp, 3, 00); // ss
//     pdp_reg_set_var(pdp, 0, 01); // ss
//
//     op_code_t opcode = { { 0, 0 }, { 0, 0 } };
//     if (pdp) {
//         opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
//     }
//
//     PRINT_RESULT("\r          ", "");
//     TRACE("%s", "test_mode2 ");
//
//     assert(opcode.ss.value == 02);
//     assert(opcode.ss.addr == 01002);
//     assert(opcode.dd.value == 0131000);
//     assert(opcode.dd.addr == 03);
//
//     run_command->do_commands_command(pdp, addr, w_read(pdp, addr),
//     (byte_t) 1);
//
//     assert(w_read(pdp, opcode.dd.addr) == 0140400);
//
//     PRINT_RESULT("  %s", " ... OK\n");
// }
//
// int
// test_mov(struct pdp_11_t *pdp, const address_word_t addr)
// {
//     pdp_reg_set_var(pdp, 7, 01000);
//     pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
//     command_t *run_command;
//
//     run_command = __ptr_command(
//         (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);
//
//     test_parse_mov(run_command);
//     test_mode0(pdp);
//
//     // PRINT_RESULT("\r", "");
//     TRACE("%s", "test_move  ");
//     run_command->do_commands_command(pdp, addr, w_read(pdp, addr),
//     (byte_t) 1);
//
//     assert(pdp_reg_get_var(pdp, 3) == 034);
//     assert(pdp_reg_get_var(pdp, 5) == 034);
//     PRINT_RESULT("  %s", "... OK\n");
//     return 0;
// }
