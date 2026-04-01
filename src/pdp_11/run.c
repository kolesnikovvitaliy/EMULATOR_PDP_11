
#include <stdio.h>
#include <stdlib.h>
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "types/types.h"
#include "tests/test.h"


int run(struct pdp_11_t* pdp, int argc, char **argv)
{
        pdp_create(pdp); // Инициализация обЬекта эмулятор PDP_11;
        //////////////////////////////////////////////////////////////////////

        // читать данные из терминала или файла
        byte_t* filename = pdp_parse_filename(argc, argv);
        // чтение потока данных
        pdp_load_data(pdp, (byte_t*)filename);


        pdp_mem_dump(pdp, 0x40, 0x20); // тестовый вывод 32 байта данных с адресса 0x40;
        pdp_mem_dump(pdp, 0x200, 0x26); // тестовый вывод 38 байт данных с адресса 0x200;


        // Прогармма стартует из адресса 01000
        address_word_t addr = 01000;

        // Инициализация регистра "PC"
        pdp_11_t* ptr_pdp = (pdp_11_t*)pdp;
        word_t *ptr_pc = ptr_pdp->PC;

        // Установка значения регистра "PC" в адресс 1000
        *ptr_pc = addr;

        // ГЛАВНЫЙ ЦИКЛ ИСПОЛНЕНИЯ ПРОГРАММЫ
        while(1) {
                // Последовательное выполнеие инструкций
               ptr_pc = do_command(pdp, ptr_pdp->command, *ptr_pc);
        }


        ////////////////////////////////////////////////////////////////////////
        pdp_destroy(pdp); // Уничтожение обЬекта PDP_11;
        return 0;
}
