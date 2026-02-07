#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pdp_11/pdp_11.h"
#include "tests/test.h"
#include "types/types.h"


//-------------------------------------------------------------------
/* Записать и проверить байт по адресу */
bool_t test_b_write(struct pdp_11_t* pdp, address_byte_t addr, byte_t data)
{

        b_write(pdp, addr, data);

        printf("\nRESULT MEM_B = %x\n", b_read(pdp, addr));

        return (b_read(pdp, addr) == data);
}

//-------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void test_mem(struct pdp_11_t* pdp)
{
        address_byte_t adr;
        byte_t b0, b1, bres;
        word_t w, wres;

        adr = 0;
        b0 = 0x12;

        assert(test_b_write(pdp, adr, b0) && "ERROR IS INCORRECT");
}
//////////////////////////////////////////////////////////////////////

