
#include <stdio.h>
#include <stdlib.h>
#include "pdp_11/pdp_11.h"
#include "types/types.h"
#include "tests/test.h"


int run(struct pdp_11_t* pdp, int argc, char **argv)
{
        pdp_create(pdp); // Активация эмулятора
        ////////////////////////////////////////////////////////////////////////
        // run all_test_pdp_11;
        //all_tests(pdp, argc, argv);

        // Перед запуском тестов закомментировать;
        byte_t* filename = pdp_parse_filename(argc, argv); // читать данные из терминала или файла
        pdp_load_data(pdp, (byte_t*)filename); // чтение потока данных
        pdp_mem_dump(pdp, 0x40, 0x20);
        pdp_mem_dump(pdp, 0x200, 0x26);

        ////////////////////////////////////////////////////////////////////////
        pdp_destroy(pdp);
        return 0;
}
