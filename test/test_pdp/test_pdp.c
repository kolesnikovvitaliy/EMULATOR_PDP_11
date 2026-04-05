#include "pdp_11/command/command.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "types/types.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "pdp_11/command/command_p.h"
#include "pdp_11/register/register_p.h"
#include "tests/test.h"
#include "tests/test_pdp/test_pdp.h"
#include "utils/logger/logger.h"
#include "utils/utils.h"

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

    DEBUG("DEFAULT MEMORY FOR CREATE PDP_11 \
                        \t%d\r\n",
          g_default_memory);

    struct pdp_11_t *pdp = pdp_new();
    pdp_create(pdp);
    assert(pdp);

    all_tests(pdp, argc, argv);

    pdp_destroy(pdp);
    free(pdp);
    memory_type((byte_t) 0);
}
/////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------
int
test_commands()
{
    /*TODO Прочитать из фйла программу и выполнить*/
    struct pdp_11_t *pdp = pdp_new();
    pdp_create(pdp);
    assert(pdp);

    pdp_11_t *     ptr_pdp = (pdp_11_t *) pdp;
    address_word_t addr    = 01000;
    word_t *       ptr_pc  = ptr_pdp->PC;
    *ptr_pc                = addr;

    // word_t w;     // текущее слово, которое содержит команду
    // главный цикл выполне:w
    // ния программы
    int i = 0;
    while (i <= 10) {
        // читаем текущее слово
        ptr_pc = do_command(pdp, ptr_pdp->command, *ptr_pc);
        i++;
    }
    return 0;
}
//--------------------------------------------------------------------
int
test_pdp(int argc, char **argv)
{
    DEBUG("\nСтартовый тест для проверки правильности\n \
                        создания и использования функций",
          "\r\n\n");
    ////////////////////////////////////////////
    WARNING("\nALLOCAT MEMORY FOR CLASS MEM_BYTE_T < BYTE >\n", "");
    TRACE("\nMEMORY BYTE CREATED !!!", "\r\n\n");
    // Если путь к файлу отсутствует запускается BYTE_TEST
    if (3 >= argc) {
        test_pdp_memory(type_memory_byte, argc, argv);
        TRACE("\nMEMORY BYTE DESTROY !!! ", "\r\n\n");
        return 0;
    }
    // Если есть путь к файлу запускаются BYTE_TEST и WORD_TEST

    test_pdp_memory(type_memory_byte, argc, argv);

    TRACE("\nMEMORY BYTE DESTROY !!! ", "\r\n\n");
    ////////////////////////////////////////////

    ////////////////////////////////////////////
    WARNING("\nALLOCAT MEMORY FOR CLASS MEM_WORD_T < WORD >\n", "");
    TRACE("\nMEMORY WORD CREATED !!! ", "\r\n\n");

    test_pdp_memory(type_memory_word, argc, argv);

    TRACE("\nMEMORY WORD DESTROY !!! ", "\r\n\n");
    test_commands();
    ////////////////////////////////////////////
    INFO("ALL TESTS PDP_11 PASSED SUCCESSFULLY", "");
    return 1;
}
