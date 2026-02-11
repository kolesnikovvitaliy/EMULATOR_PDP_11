#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tests/test_memory/test_memory_byte/test_memory_byte.h"
//-------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void test_memory(struct pdp_11_t* pdp)
{
        assert(pdp);

        test_byte_buffer(pdp);

}
//////////////////////////////////////////////////////////////////////
