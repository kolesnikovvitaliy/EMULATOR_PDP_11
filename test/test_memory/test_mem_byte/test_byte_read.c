#include <stdio.h>
#include "tests/test_memory/test_memory_byte/test_memory_byte.h"


//-------------------------------------------------------------------
/* Прочитать байт по адресу */
bool_t test_byte_read(struct pdp_11_t* pdp, address_byte_t addr)
{
        printf("\nRESULT MEM_B = %x\n", b_read(pdp, addr));
        return (b_read(pdp, addr));
}

//-------------------------------------------------------------------

