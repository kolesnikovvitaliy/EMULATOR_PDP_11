#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pdp_11/pdp_11.h"
#include "run/run.h"


int main(int argc, char **argv)
{
        struct pdp_11_t* pdp = pdp_new();
        assert(pdp);

        run(pdp, argc, argv);

        free(pdp);

        return 0;
}
