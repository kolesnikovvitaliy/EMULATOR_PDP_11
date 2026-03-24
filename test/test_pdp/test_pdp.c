#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "types/types.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/command/command.h"
#include "pdp_11/register/register_p.h"
#include "tests/test.h"
#include "tests/test_pdp/test_pdp.h"
#include "utils/utils.h"
#include "utils/logger/logger.h"

//-------------------------------------------------------------------
extern byte_t g_default_memory;
void memory_type(byte_t type_memory)
{
        g_default_memory = type_memory;
}
//////////////////////////////////////////////////////////////////////
void test_pdp_memory(byte_t type_memory, int argc, char **argv)
{

        memory_type(type_memory);
        DEBUG( "DEFAULT MEMORY FOR CREATE PDP_11 \
                        \t%d\r\n", g_default_memory);
        struct pdp_11_t* pdp = pdp_new();
        pdp_create(pdp);
        assert(pdp);

        INFO("\n\nTEST REGISTER\n", "");
        *(((pdp_11_t*)pdp)->R0) = 0xfe;
        INFO("\n\n*(pdp->R0) = 0xfe;\n", "");
        pdp_11_t* ptr_pdp = (pdp_11_t*)pdp;
        register_t* ptr_regist = (register_t*)ptr_pdp->regist;
        assert(*(ptr_pdp->R0) == ptr_regist->R0);
        INFO("\n\npdp->regist->R0 = %X;\n\n", ptr_regist->R0);
        INFO("\n\n(*(pdp->R0) == pdp->regist->R0)\n", "SUCCES");


        //pdp_load_data(pdp,(byte_t*)"./data/test.txt");
        // byte_t* filename = pdp_parse_filename(argc, argv);
        // pdp_load_data(pdp, (byte_t*)filename);
        // pdp_mem_dump(pdp, 0x40, 0x20);
        // pdp_mem_dump(pdp, 0x200, 0x26);

        all_tests(pdp, argc, argv);
        INFO("\n\nTEST REGISTER - 2\n", "");
        *(((pdp_11_t*)pdp)->R0) = 0xdeef;
        INFO("\n\n*(pdp->R0) = 0xdeef;\n", "");
        assert(*(ptr_pdp->R0) == ptr_regist->R0);
        INFO("\n\npdp->regist->R0 = 0x%X;\n\n", ptr_regist->R0);
        INFO("\n\n(*(pdp->R0) == pdp->regist->R0)\n", "SUCCES");


        word_t *pc = ((pdp_11_t*)pdp)->R7;
        *pc = 01000;


        word_t w;     // текущее слово, которое содержит команду
        // главный цикл выполнения программы
        int i = 0;
        while(i++ <= 10) {
                // читаем текущее слово
                w = w_read(pdp, *pc);
                // печатаем адрес и слово по этому адресу, как в листинге
                TRACE("%06o %06o: ", *pc, w);
                // pc сразу же указывает на следующее неразобранное слово
                *pc += 2;
        }
        if (w == 0) {
                TRACE("\nHALT\n", "");
                do_halt();
        }
        pdp_destroy(pdp);
        free(pdp);
        memory_type((byte_t)0);

}
/////////////////////////////////////////////////////////////////////
int test_pdp(int argc, char **argv)
{
        DEBUG("\nСтартовый тест для проверки правильности\n \
                        создания и использования функций", "\r\n\n");
        ////////////////////////////////////////////
        DEBUG( "\nMEMORY BYTE CREATED !!!","\r\n\n");
        if (1 == argc) {
                test_pdp_memory(type_memory_byte, argc, argv);
                DEBUG( "\nMEMORY BYTE DESTROY !!! ","\r\n\n");
                return 0;
        }

        test_pdp_memory(type_memory_byte, argc, argv);

        DEBUG( "\nMEMORY BYTE DESTROY !!! ","\r\n\n");
        ////////////////////////////////////////////

        ////////////////////////////////////////////
        DEBUG( "\nMEMORY WORD CREATED !!! ","\r\n\n");

        test_pdp_memory(type_memory_word, argc, argv);

        DEBUG( "\nMEMORY WORD DESTROY !!! ","\r\n\n");
        ////////////////////////////////////////////

        return 1;

}
