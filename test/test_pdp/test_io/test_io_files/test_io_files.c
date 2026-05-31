#include "pdp_11/pdp_11.h"
#include "types/types.h"

#include <stdio.h>
#include <stdlib.h>
int
test_io_files(struct pdp_11_t *pdp, byte_t *filename)
{
    TRACE("TEST LOAD DATA FROM FILES", "");
    pdp_load_data((struct pdp_11_t *) pdp, (byte_t *) filename);
    return 1;
}
