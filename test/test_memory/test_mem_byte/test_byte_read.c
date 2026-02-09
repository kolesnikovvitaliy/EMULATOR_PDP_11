#include <stdio.h>
#include "types/types.h"
#include "tests/test_memory/test_memory_byte/test_memory_byte.h"
// Start created TESTS
// Список тестов
//-------------------------------------------------------------------
/* Записать и проверить байт по адресу */
bool_t test_byte_read(struct pdp_11_t* pdp, address_byte_t addr, byte_t data)
{

        b_write(pdp, addr, data);

        printf("\nRESULT MEM_B = %x\n", b_read(pdp, addr));

        return (b_read(pdp, addr) == data);
}

//-------------------------------------------------------------------

