#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "types/types.h"
#include "pdp_11/pdp_11.h"
#include "tests/test.h"
#include "tests/test_pdp/test_pdp.h"
#include "utils/utils.h"
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
        fprintf(stdout, "DEFAULT MEMORY FOR CREATE PDP_11 \
                        \t%d\r\n", g_default_memory);
        struct pdp_11_t* pdp = pdp_new();
        pdp_create(pdp);
        assert(pdp);

        //pdp_load_data(pdp,(byte_t*)"./data/test.txt");
        // byte_t* filename = pdp_parse_filename(argc, argv);
        // pdp_load_data(pdp, (byte_t*)filename);
        // pdp_mem_dump(pdp, 0x40, 0x20);
        // pdp_mem_dump(pdp, 0x200, 0x26);

        all_tests(pdp, argc, argv);
        pdp_destroy(pdp);
        free(pdp);
        memory_type((byte_t)0);

}
/////////////////////////////////////////////////////////////////////
int test_pdp(int argc, char **argv)
{
        fprintf(stdout,"\nСтартовый тест для проверки правильности\n \
                        создания и использования функций\r\n\n");
        ////////////////////////////////////////////
        fprintf(stdout, "MEMORY BYTE CREATED !!! \r\n\n");
        if (1 == argc) {
                test_pdp_memory(type_memory_byte, argc, argv);
                fprintf(stdout, "\nMEMORY BYTE DESTROY !!! \r\n\n");
                return 0;
        }

        test_pdp_memory(type_memory_byte, argc, argv);

        fprintf(stdout, "\nMEMORY BYTE DESTROY !!! \r\n\n");
        ////////////////////////////////////////////

        ////////////////////////////////////////////
        fprintf(stdout, "MEMORY WORD CREATED !!! \r\n\n");

        test_pdp_memory(type_memory_word, argc, argv);

        fprintf(stdout, "\nMEMORY WORD DESTROY !!! \r\n\n");
        ////////////////////////////////////////////

        return 1;

}
