#include <stdio.h>
#include <assert.h>
#include "tests/test_memory/test_memory_word/test_memory_word.h"

//--------------------------------------------------------------------
/* Записать байт по адресу */
/* Прочитать байт по адресу t*/

void test_rw_byte_w(struct pdp_11_t* pdp)
{
        //пишем байт, читаем байт

        address_word_t adr;
        word_t b0, bres;

        adr = 0;
        b0 = 0x12;

        fprintf(stderr,"Пишем и читаем байт по четному адресу\r\n");

        b_write(pdp, adr, b0);
        bres = b_read(pdp, adr);

        fprintf(stderr,"a = %06o\t b0 = %hhx\t bres = %hhx\r\n", adr, b0, bres);
        assert(bres == b0);
}

void test_rw_word_w(struct pdp_11_t* pdp)
{
        // пишем слово, читаем слово.

        address_word_t adr;
        word_t w, wres;

        adr = 8;
        w = 0x3456;

        fprintf(stderr, "Пишем и читаем слово\r\n");

        w_write(pdp, adr, w);
        wres = w_read(pdp, adr);
        if (adr & 1) (adr--);

        fprintf(stderr, "a = %06x\t w = %04x\t wres = %04x\n", adr, w, wres);
        assert(w == wres);

}
//--------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void test_word_buffer(struct pdp_11_t* pdp)
{

        test_rw_word_w(pdp);
        test_rw_word_w(pdp);
}
//////////////////////////////////////////////////////////////////////
