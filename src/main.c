/**
 * @file main.c
 * @brief Ядро эмулятора архитектуры PDP-11.
 *
 * @details Данный файл является "точкой сборки" всей системы. Он управляет
 * жизненным циклом эмулятора: от парсинга аргументов командной строки
 * до корректного освобождения ресурсов после завершения работы.
 *
 * @author <kolesnikovvitaliy@mail.ru Vitaliy Kolesnikov>
 * @date 2026
 */

#include "pdp_11/pdp_11.h"
#include "pdp_11/run.h"
#include "tests/test_pdp/test_pdp.h"
#include "utils/logger/logger.h"

#include <assert.h>
#ifdef _WIN32
#    include <windows.h>
#else
#    include <locale.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Селектор режима отладки.
 *
 * @details Анализирует массив аргументов командной строки на наличие флага
 * `-d`. При обнаружении флага прерывает стандартный поток выполнения и
 * запускает модуль модульного тестирования.
 *
 * @param[in] argc Количество аргументов (передается из main).
 * @param[in] argv Вектор строк аргументов.
 *
 * @retval 1 Обнаружен флаг отладки, тесты выполнены.
 * @retval 0 Флаг отсутствует, требуется обычный запуск.
 *
 * @note Вызов этой функции должен происходить до инициализации основных
 * структур данных.
 */
int start_test_if_mode_debug(int argc, char **argv);

/**
 * @brief Главная точка входа в приложение (Main Entry Point).
 *
 * @details Алгоритм работы:
 * 1. Проверка необходимости запуска тестов.
 * 2. Аллокация и "чистая" инициализация контекста PDP-11.
 * 3. Передача управления диспетчеру выполнения (функция @ref run).
 * 4. Деаллокация памяти.
 *
 * @param[in] argc Количество аргументов командной строки.
 * @param[in] argv Массив строк аргументов.
 *
 * @return Статус завершения процесса.
 * @retval 0 Успешное завершение.
 * @retval !=0 Критическая ошибка при работе эмулятора.
 */
int
main(int argc, char **argv)
{
#ifdef _WIN32
    SetConsoleOutputCP(65001); // Для Windows
#else
    setlocale(LC_ALL, "Russian");
#endif
        // Проверка режима отладки перед стартом основной логики
    int flag_tests = 0;
    /*ЗАПУСК ПРОВЕРКИ РАБОТЫ ЭМУЛЯТОРА В РЕЖИМЕ ПАМЯТИ =BYTE= и =WORD=*/
    flag_tests = start_test_if_mode_debug(argc, argv);
    /**
     * @note Инициализация через pdp_new гарантирует нулевое состояние
     * регистров и памяти, что предотвращает "undefined behavior" и ошибки
     * Valgrind.
     */
    struct pdp_11_t *pdp = pdp_new();
    assert(pdp != NULL);

    TRACE("PDP_11 ИНИЦИАЛИЗИРОВАНА\n", "");

    /* Запуск исполнительного цикла */
    run(pdp, argc, argv, flag_tests);

    /* Очистка контекста перед выходом */

    free(pdp);

    return 0;
}

/**
 * @brief Переключатель режимов: Тестирование vs Эмуляция.
 *
 * @details Проходит по массиву аргументов командной строки в поиске
 * управляющего флага `-d`. Если флаг обнаружен, функция немедленно инициирует
 * выполнение набора тестов через вызов @ref test_pdp.
 *
 * @param[in] argc Количество аргументов, переданных программе.
 * @param[in] argv Массив строк (аргументов), где выполняется поиск флага.
 *
 * @retval 1 Обнаружен режим отладки, тесты запущены.
 * @retval 0 Режим отладки не активен, работа продолжается в штатном режиме.
 *
 * @note Поиск чувствителен к регистру; флаг должен быть в нижнем регистре
 * ("-d").
 * @attention Функция не проверяет остальные аргументы после нахождения первого
 * вхождения "-d".
 *
 * @todo Добавить поддержку расширенных флагов отладки (например, "-d:verbose")
 * для выбора конкретных групп тестов.
 */
int
start_test_if_mode_debug(int argc, char **argv)
{
    if (argv == NULL || argc < 1) {
        return 0;
    }
    char log_flag = 0;
    for (int i = 0; i < argc; i++) {
        if (strcmp("-disable_logs", argv[i]) == 0) {
            log_flag = 1;
            continue;
        }
        if (strcmp("-d", argv[i]) == 0) {
            WARNING("ЗАПУСК ПРОВЕРКИ КОНФИГУРАЦИИ", "");
            log_level_t log_level = set_log_level(NONE);
            test_pdp(argc, argv);

            set_log_level(log_level);
            WARNING("ПРОВЕРКА КОНФИГУРАЦИИ ЗАВЕРШИЛАСЬ УСПЕШНО\n", "");
            continue;
        }
    }
    if (log_flag) {
        set_log_level(NONE);
    }

    return 0;
}
