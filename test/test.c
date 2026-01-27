#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "mem.h"

typedef int bool_t;

bool_t test_b_write(address_t*, byte_t*);

//////////////////////////////////////////////////////////////////////
void test_mem()
{
        address_t a;
        byte_t b0, b1, bres;
        word_t w, wres;

        a = 0;
        b0 = 0x12;

        assert(test_b_write(&a, &b0));
}
//////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------
/* Записать и проверить байт по адресу */
bool_t test_b_write(address_t *addr, byte_t *ch){
        b_write(*addr, *ch);

        printf("\nRESULT MEM_B = %x\n", mem_b[(int)*addr]);

        return (bool_t)(mem_b[(int)*addr] == *ch);
}

//-------------------------------------------------------------------
