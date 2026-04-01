/**
 * @file main.c
 * @brief Точка входа в эмулятор PDP-11.
 *
 * Файл содержит основную логику инициализации системы, обработку
 * флагов отладки и запуск цикла выполнения команд.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "pdp_11/pdp_11.h"
#include "utils/logger/logger.h"
#include "pdp_11/run.h"
#include "tests/test_pdp/test_pdp.h"

/**
 * @brief Проверяет наличие флага отладки и запускает тесты.
 *
 * Если в аргументах командной строки передан флаг "-d",
 * функция запускает набор встроенных тестов эмулятора.
 *
 * @param[in] argc Количество аргументов командной строки.
 * @param[in] argv Массив строк аргументов.
 * @return int Возвращает 1, если тесты были запущены, иначе 0.
 */
int start_test_if_mode_debug(int argc, char **argv);

/**
 * @brief Главная функция программы.
 *
 * Выполняет инициализацию структуры эмулятора, проверку режима
 * работы и передает управление основному циклу эмуляции.
 *
 * @param[in] argc Количество аргументов.
 * @param[in] argv Массив аргументов.
 * @return int Код завершения программы (0 - успех).
 */
int main(int argc, char **argv) {
    // Если указан префикс "-d", автоматически запускаются тесты
    start_test_if_mode_debug(argc, argv);

    /*
     * Инициализация объекта PDP-11.
     * чтобы избежать ошибок "uninitialised value" в Valgrind.
     */
    struct pdp_11_t *pdp = pdp_new();
    assert(pdp != NULL);

    DEBUG("PDP_11 ИНИЦИАЛИЗИРОВАНА", "");

    // Основной цикл работы эмулятора
    run(pdp, argc, argv);

    // Освобождение ресурсов
    free(pdp);

    return 0;
}

int start_test_if_mode_debug(int argc, char **argv) {
    if (argv == NULL || argc < 1) {
        return 0;
    }

    for (int i = 0; i < argc; i++) {
        if (strcmp("-d", argv[i]) == 0) {
            DEBUG("ЗАПУСК ТЕСТОВ", "");
            test_pdp(argc, argv);
            return 1;
        }
    }

    return 0;
}
