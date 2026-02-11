#include <stdio.h>
#include <assert.h>
#include "tests/test_memory/test_memory_byte/test_memory_byte.h"

//--------------------------------------------------------------------
/* Записать байт по адресу */
/* Прочитать байт по адресу t*/

void test_write_read_byte(struct pdp_11_t* pdp)
{
        address_byte_t adr;
        byte_t b0, b1, bres;
        word_t w, wrest;


        //пишем байт, читаем байт

        fprintf(stderr,"Пишем и читаем байт по четному адресу\r\n");
        adr = 0;
        b0 = 0x12;
        b_write(pdp, adr, b0);
        bres = b_read(pdp, adr);
        fprintf(stderr,"a = %08o\t b0 = %hhx\t bres = %hhx\r\n", adr, b0, bres);
        assert(bres == b0);

}

//--------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void test_byte_buffer(struct pdp_11_t* pdp)
{

        test_write_read_byte(pdp);
}
//////////////////////////////////////////////////////////////////////
