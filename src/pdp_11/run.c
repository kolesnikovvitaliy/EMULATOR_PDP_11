
#include <stdio.h>
#include <stdlib.h>
#include "pdp_11/pdp_11.h"

#include "tests/test.h"



int run(struct pdp_11_t* pdp, int argc, char **argv)
{
        pdp_create(pdp);
        ////////////////////////////////////////////////////////////////////////
        // run PDP_EMULATOR
        all_tests(pdp);
        ////////////////////////////////////////////////////////////////////////
        pdp_destroy(pdp);
        return 0;
}
