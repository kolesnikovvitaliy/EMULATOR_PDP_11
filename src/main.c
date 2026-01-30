#include <stdio.h>
#include "pdp_11/pdp_11.h"
#include "memory/mem.h"
#include "tests/test.h"

int main(int argc, char **argv)
{
        pdp_11_t* pdp = pdp_create();

        test_mem(*pdp);

        pdp_destroy(*pdp);

        return 0;
}
