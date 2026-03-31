#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "pdp_11/pdp_11.h"
#include "utils/logger/logger.h"
#include "pdp_11/run.h"
#include "tests/test_pdp/test_pdp.h"



int main(int argc, char **argv)
{
        if (argc > 0) {
                for (int i = 0; i < argc; i++) {
                        if (!strcmp("-d", argv[i])) {
                                test_pdp(argc, argv); // ЗАПУСК ТЕСТОВ;
                                return 1;
                        }
                }
        }

        struct pdp_11_t* pdp = pdp_new(); // Выделение памяти под обЬетк PDP_11;
        assert(pdp); // проверка
        DEBUG("PDP_11 ИНИЦИАЛИЗИРОВАННА", "");

        run(pdp, argc, argv); // Запуск ЭМУЛЯТОРА;

        free(pdp); // Высвобождение памяти;

        return 0;
}
