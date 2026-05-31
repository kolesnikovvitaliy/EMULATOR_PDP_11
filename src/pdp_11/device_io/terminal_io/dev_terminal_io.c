#include "pdp_11/pdp_11.h"
#include "utils/logger/logger.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
// Загрузка данных из терминала
void
load_data_term(struct pdp_11_t *pdp)
{
    enum {
        res_input_data = 2
    }; // Введены два параметра addr,
       // и количество байт

    word_t addr, count_str, data, res_input;

    addr = count_str = data = res_input = 0x00;
    res_input = (word_t) fscanf(stdin, "%hx%hx", &addr, &count_str);
    // printf("RES INPUT DATA = %d\r\nADDR = %hx\r\nCOUNT_STR = %hx\r\n",
    //                res_input, addr, count_str);
    DEBUG("\nRES INPUT DATA = %d\r\nADDR = %hx\r\nCOUNT_STR = %hx\r\n",
          res_input,
          addr,
          count_str);

    if (!(res_input == res_input_data)) {
        ERROR("\nError Readeng With TERMINAL\r\n", "");
        assert((res_input == res_input_data));
        abort();
    }
    do {
        for (byte_t ind = 0x0; ind < count_str; ind++) {
            int res_fscanf __attribute__((unused));
            res_fscanf = fscanf(stdin, "%hx", &data);
            b_write(pdp, (address_word_t)(addr | ind), (byte_t) data);
        }
        res_input = (word_t) fscanf(stdin, "%hx%hx", &addr, &count_str);
    } while (res_input == res_input_data);
    // fprintf(stdout, "\nREAD TERMINAL EXIT\r\n\n");
    INFO("READ TERMINAL EXIT", "");
    return;
}
