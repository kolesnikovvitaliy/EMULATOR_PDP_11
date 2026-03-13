#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pdp_11/pdp_11.h"
#include "pdp_11/run.h"
#include "tests/test_pdp/test_pdp.h"


int main(int argc, char **argv)
{
        //test_pdp(argc, argv);

        // Перед запуском тестов закоментировать;
        struct pdp_11_t* pdp = pdp_new(); // Создание эмулятора
        assert(pdp);

        run(pdp, argc, argv); // Запуск

        free(pdp);

        return 0;
}
