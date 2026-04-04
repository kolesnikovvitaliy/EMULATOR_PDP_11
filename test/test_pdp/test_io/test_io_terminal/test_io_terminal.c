#include "pdp_11/pdp_11.h"
#include "types/types.h"

#include <stdio.h>
#include <stdlib.h>
int
test_io_terminal(struct pdp_11_t *pdp)
{

    DEBUG("\nTEST LOAD DATA FROM TERMINAL\n", "\n");
    pdp_load_data((struct pdp_11_t *) pdp, NULL);
    return 0;
}
