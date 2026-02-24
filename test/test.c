#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tests/test_pdp/test_memory/test_memory.h"
#include "tests/test_pdp/test_io/test_io.h"


//-------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void all_tests(struct pdp_11_t* pdp, int argc, char** argv)
{
        test_memory(pdp, argc, argv);
        test_io(pdp, argc, argv);
}
//////////////////////////////////////////////////////////////////////
