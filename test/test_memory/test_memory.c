#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tests/test_memory/test_memory_byte/test_memory_byte.h"
//-------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void test_memory(struct pdp_11_t* pdp)
{
        address_byte_t adr;
        byte_t b0, b1, bres;
        word_t w, wrest;

        adr = 0xFFFE;
        b0 = 0x12;
        //return (b_read(pdp, addr) == data);
        test_byte_write(pdp, adr, b0);
        assert(test_byte_read(pdp, adr) && "ERROR IS INCORRECT");
}
//////////////////////////////////////////////////////////////////////
