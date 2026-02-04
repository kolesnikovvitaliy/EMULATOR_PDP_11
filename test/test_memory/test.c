#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "memory/mem_p.h"
#include "pdp_11/pdp_11_p.h"
#include "tests/test.h"
typedef int bool_t;


//-------------------------------------------------------------------
/* Записать и проверить байт по адресу */
bool_t test_b_write(pdp_11_t* pdp, address_byte_t* addr, byte_t* data)
{

        b_write(pdp, addr, data);

        printf("\nRESULT MEM_B = %x\n", b_read(pdp, addr));

        return 0;
}

//-------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void test_mem(pdp_11_t* pdp)
{
        address_byte_t* adr;
        byte_t b0, b1, bres;
        word_t *w, *wres;

        adr = 0;
        b0 = 0x12;
        assert(test_b_write(pdp, adr, &b0));
}
//////////////////////////////////////////////////////////////////////

