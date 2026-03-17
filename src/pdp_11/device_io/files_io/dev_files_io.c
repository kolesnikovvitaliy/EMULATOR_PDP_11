#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "types/types.h"
#include "utils/utils.h"
#include "utils/logger/logger.h"
#include "pdp_11/pdp_11.h"
// Загрузка данных из файла
void load_data_file(struct pdp_11_t* pdp, byte_t* filename)
{
        enum {res_input_data = 2}; // Введены два параметра addr,
                                   // и количество байт

        file_t * fp;
        word_t addr, count_str, data, res_input;

        addr = count_str = data = res_input = 0x00;
        fp  = pdp_file_open(filename,(byte_t*)"rb");
        assert(fp);

        res_input = fscanf(fp, "%hx%hx", &addr, &count_str);
        assert(res_input);
        if (!(res_input == res_input_data)) {
                pdp_file_close(fp);
                //fprintf(stderr,"Error Readeng Files file\r\n");
                ERROR("Error Readeng File [%s]\r\n", filename);
                assert(res_input == res_input_data);
                abort();
        }
        do {
                for(byte_t ind = 0x0; ind < count_str; ind++) {
                        fscanf(fp, "%hx", &data);
                        b_write(pdp, (addr+ind), data);
                }
                res_input = fscanf(fp, "%hx%hx", &addr, &count_str);
        }while(res_input == res_input_data);
        //fprintf(stdout, "\nREAD FILE EXIT\n\n");
        INFO("\nREAD FILE EXIT\n\n", "");
        pdp_file_close(fp);
        return;
}

