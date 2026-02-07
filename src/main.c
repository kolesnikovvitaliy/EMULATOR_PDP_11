#include <stdio.h>
#include "pdp_11/pdp_11.h"

#include "tests/test.h"



int main(int argc, char **argv)
{
        struct pdp_11_t* pdp = pdp_new();
        pdp_create(pdp);
        test_mem(pdp);

        pdp_destroy(pdp);

        return 0;
}
