#include <stdio.h>
#include <stdlib.h>
#include "pdp_11/pdp_11.h"
#include "types/types.h"
int test_io_files(struct pdp_11_t* pdp, byte_t* filename)
{
    fprintf(stdout,"TEST LOAD DATA FROM FILES\n");
    pdp_load_data((struct pdp_11_t*)pdp, (byte_t*)filename);
    return 1;
}
