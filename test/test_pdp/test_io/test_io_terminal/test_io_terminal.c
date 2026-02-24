#include <stdio.h>
#include <stdlib.h>
#include "types/types.h"
#include "pdp_11/pdp_11.h"
int test_io_terminal(struct pdp_11_t* pdp)
{

    fprintf(stdout, "TEST LOSD DATA FROM TERMINAL\n");
    //pdp_load_data((struct pdp_11_t*)pdp, NULL);
    return 0;
}
