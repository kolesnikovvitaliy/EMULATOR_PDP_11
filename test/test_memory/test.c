#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tests/test.h"
#include "types/types.h"
#include "tests/test_memory_byte/test_memory_byte.h"

-------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void test_memory(struct pdp_11_t* pdp)
{
        address_byte_t adr;
        byte_t b0, b1, bres;
        word_t w, wrest;

        adr = 0;
        b0 = 0x12;

        assert(test_byte_write(pdp, adr, b0) && "ERROR IS INCORRECT");
}
//////////////////////////////////////////////////////////////////////
