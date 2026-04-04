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

    INFO("\n\nTEST REGISTER\n", "");
    *(((pdp_11_t *) pdp)->R0) = 01000;
    INFO("\n\n*(pdp->R0) = 1000;\n", "");
    pdp_11_t *ptr_pdp    = (pdp_11_t *) pdp;
    reg_t *   ptr_regist = (reg_t *) ptr_pdp->regist;
    assert(*(ptr_pdp->R0) == ptr_regist->R0);
    INFO("\n\npdp->regist->R0 = %o;\n\n", ptr_regist->R0);
    INFO("\n\n(*(pdp->R0) == pdp->regist->R0)\n", "SUCCES");

    // pdp_load_data(pdp,(byte_t*)"./data/test.txt");
    // byte_t* filename = pdp_parse_filename(argc, argv);
    // pdp_load_data(pdp, (byte_t*)filename);
    // pdp_mem_dump(pdp, 0x40, 0x20);
    // pdp_mem_dump(pdp, 0x200, 0x26);

    all_tests(pdp, argc, argv);
    INFO("\n\nTEST REGISTER - 2\n", "");
    *(((pdp_11_t *) pdp)->R0) = 01000;
    INFO("\n\n*(pdp->R0) = 1000;\n", "");
    assert(*(ptr_pdp->R0) == ptr_regist->R0);
    INFO("\n\npdp->regist->R0 = %o;\n\n", ptr_regist->R0);
    INFO("\n\n(*(pdp->R0) == pdp->regist->R0)\n", "SUCCES");

    address_word_t addr   = 01000;
    word_t *       ptr_pc = ptr_pdp->PC;
    *ptr_pc               = addr;

    // word_t w;     // текущее слово, которое содержит команду
    // главный цикл выполне:w
    // ния программы
    int i = 0;
    while (i <= 10) {
        // читаем текущее слово
        ptr_pc = do_command(pdp, ptr_pdp->command, *ptr_pc);
        i++;

        // w = w_read(pdp, *pc);
        // печатаем адрес и слово по этому адресу, как в листинге
        // TRACE("%06o %06o: ", *ptr_pc, w);
        // pc сразу же указывает на следующее неразобранное слово
        // *pc += 2;
    }
    INFO("\nTEST_OUTPUT \n\n%d\t%d\n\n", ptr_pc, *ptr_pc);
    pdp_destroy(pdp);
    free(pdp);
    memory_type((byte_t) 0);
}
/////////////////////////////////////////////////////////////////////
int
test_pdp(int argc, char **argv)
{
    DEBUG("\nСтартовый тест для проверки правильности\n \
                        создания и использования функций",
          "\r\n\n");
    ////////////////////////////////////////////
    DEBUG("\nMEMORY BYTE CREATED !!!", "\r\n\n");
    // Если путь к файлу отсутствует запускается BYTE_TEST
    if (2 == argc) {
        test_pdp_memory(type_memory_byte, argc, argv);
        DEBUG("\nMEMORY BYTE DESTROY !!! ", "\r\n\n");
        return 0;
    }
    // Если есть путь к файлу запускаются BYTE_TEST и WORD_TEST

    test_pdp_memory(type_memory_byte, argc, argv);

    DEBUG("\nMEMORY BYTE DESTROY !!! ", "\r\n\n");
    ////////////////////////////////////////////

    ////////////////////////////////////////////
    DEBUG("\nMEMORY WORD CREATED !!! ", "\r\n\n");

    test_pdp_memory(type_memory_word, argc, argv);

    DEBUG("\nMEMORY WORD DESTROY !!! ", "\r\n\n");
    ////////////////////////////////////////////

    return 1;
}
