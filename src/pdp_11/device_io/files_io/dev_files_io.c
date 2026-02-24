#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "types/types.h"
#include "utils/utils.h"
#include "pdp_11/device_io/files_io/dev_files_io_p.h"
#include "pdp_11/pdp_11.h"
//#include "pdp_11/pdp_11_p.h"

void load_data_file(struct pdp_11_t* pdp, byte_t* filename)
{
        enum {res_input_data = 2};

        file_t * fp;
        word_t addr, count_str, data, res_input;

        addr = count_str = data = res_input= 0x0;
        fp  = pdp_file_open((byte_t*)filename,(byte_t*)"rb");

        res_input = fscanf(fp, "%hx%hx", &addr, &count_str);
        if (!(res_input == res_input_data)) {
                pdp_file_close(fp);
                fprintf(stderr,"Error Readeng Files\r\n");
                abort();
        }
        do {
                for(byte_t ind = 0x0; ind < count_str; ind++) {
                        fscanf(fp, "%hx", &data);
                        b_write(pdp, (addr+ind), data);
                }
                res_input = fscanf(fp, "%hx%hx", &addr, &count_str);
        }while(res_input == res_input_data);
        pdp_file_close(fp);
}

