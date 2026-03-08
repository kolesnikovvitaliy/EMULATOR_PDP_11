#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pdp_11/pdp_11.h"
// Загрузка данных из терминала
void load_data_term(struct pdp_11_t* pdp)
{
        enum {res_input_data = 2}; // Введены два параметра addr,
                                   // и количество байт

        word_t addr, count_str, data, res_input;

        addr = count_str = data = res_input= 0x00;
        res_input = fscanf(stdin, "%hx%hx", &addr, &count_str);
        printf("RES INPUT DATA = %d\r\nADDR = %hx\r\nCOUNT_STR = %hx\r\n",
                        res_input, addr, count_str);
        if (!(res_input == res_input_data)) {
                fprintf(stderr,"Error Readeng Files TERM\r\n");
                assert((res_input == res_input_data));
                abort();
        }
        do {
                for(byte_t ind = 0x0; ind < count_str; ind++) {
                        fscanf(stdin, "%hx", &data);
                        b_write(pdp, (addr+ind), data);
                }
                res_input = fscanf(stdin, "%hx%hx", &addr, &count_str);
        }while(res_input == res_input_data);
        fprintf(stdout, "READ TERMINAL EXIT\r\n");
        return;
}
