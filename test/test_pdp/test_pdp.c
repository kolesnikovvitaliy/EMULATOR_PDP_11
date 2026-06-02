/**
 * @file test_pdp.c
 * @brief Запуск модульных тестов эмулятора.
 */
#include "tests/test_pdp/test_pdp.h"

#include "pdp_11/command/command.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "tests/test.h"
#include "tests/test_pdp/test_command/test_command.h"
#include "types/types.h"
#include "utils/logger/logger.h"
#include "utils/utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------------------------------------------------------------
extern byte_t g_default_memory;
void
memory_type(byte_t type_memory)
{
    g_default_memory = type_memory;
}
//////////////////////////////////////////////////////////////////////
void
test_pdp_memory(byte_t type_memory, int argc, char **argv)
{

    memory_type(type_memory);


    struct pdp_11_t *pdp = pdp_new();
    pdp_create(pdp);
    assert(pdp);
    PRINT_RESULT("%s", " ... OK\n");
    all_tests(pdp, argc, argv);

    pdp_destroy(pdp);
    free(pdp);
    memory_type((byte_t) 0);
}
//////////////////////////////////////////////////////////////////////
int
test_pdp(int argc, char **argv)
{
    /*
     * DEBUG("\nСтартовый тест для проверки правильности\n \
     *                   создания и использования функций",
     *     "\r\n\n");
     */
    ////////////////////////////////////////////
    WARNING("ALLOCAT MEMORY FOR CLASS MEM_BYTE_T < BYTE >", "");
    INFO("ALLOCAT MEMORY  TYPE ==BYTE==", "");
    // Если путь к файлу отсутствует запускается BYTE_TEST
    if (3 >= argc) {
        test_pdp_memory(type_memory_byte, argc, argv);
        WARNING("MEMORY TYPE ==BYTE== DESTROY !!! \n", "");
        return 0;
    }
    // Если есть путь к файлу запускаются BYTE_TEST и WORD_TEST

    test_pdp_memory(type_memory_byte, argc, argv);

    WARNING("MEMORY TYPE ==BYTE== DESTROY !!! \n", "");
    ////////////////////////////////////////////

    ////////////////////////////////////////////
    WARNING("ALLOCAT MEMORY FOR CLASS MEM_WORD_T < WORD >", "");
    INFO("ALLOCAT MEMORY  TYPE ==WORD==  !!!", "");

    test_pdp_memory(type_memory_word, argc, argv);

    WARNING("MEMORY ==WORD== DESTROY !!! \n", "");
    ////////////////////////////////////////////
    INFO("ALL TESTS PDP_11 PASSED SUCCESSFULLY\n", "");
    return 1;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
