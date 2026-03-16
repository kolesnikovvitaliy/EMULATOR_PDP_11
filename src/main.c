#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pdp_11/pdp_11.h"
#include "utils/logger/logger.h"
#include "pdp_11/run.h"
#include "tests/test_pdp/test_pdp.h"


int main(int argc, char **argv)
{
        //test_pdp(argc, argv); // ЗАПУСК ТЕСТОВ;

        // Перед запуском тестов закомментировать;
        struct pdp_11_t* pdp = pdp_new(); // Выделение памяти под обЬетк PDP_11;
        assert(pdp); // проверка
        INFO("PDP_11 ИНИЦИАЛИЗИРОВАННА", "");

        run(pdp, argc, argv); // Запуск ЭМУЛЯТОРА;

        free(pdp); // Высвобождение памяти;

        return 0;
}
