#include <stdio.h>
#include <assert.h>
#include "tests/test_memory/test_memory_byte/test_memory_byte.h"

//--------------------------------------------------------------------
/* Записать байт по адресу */
/* Прочитать байт по адресу t*/

void test_rw_byte_b(struct pdp_11_t* pdp)
{
        //пишем байт, читаем байт

        address_byte_t adr;
        byte_t b0, bres;

        adr = 0;
        b0 = 0x12;

        fprintf(stderr,"Пишем и читаем байт по четному адресу\r\n");

        b_write(pdp, adr, b0);
        bres = b_read(pdp, adr);

        fprintf(stderr,"a = %06o\t b0 = %hhx\t bres = %hhx\r\n", adr, b0, bres);
        assert(bres == b0);
}

void test_rw_word_b(struct pdp_11_t* pdp)
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

void test_w2b_rword_b(struct pdp_11_t* pdp)
{
        // пишем 2 байта, читаем 1 слов;
    address_word_t a;
    byte_t b0, b1, bres;
    word_t w, wres;
    fprintf(stderr, "Пишем 2 байта, читаем слово\n");
    a = 4;        // другой адрес
    w = 0xa1b2;
    // little-endian, младшие разряды по меньшему адресу
    b0 = 0xb2;
    b1 = 0xa1;
    b_write(pdp, a, b0);
    b_write(pdp, a+1, b1);
    wres = w_read(pdp, a);
    // тут полезно написать отладочную печать a, w, wres
    fprintf(stderr, "a=%06o b1=%02hhx b0=%02hhx wres=%04x\n", a, b1, b0, wres);
    assert(w == wres);
}

//--------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void test_byte_buffer(struct pdp_11_t* pdp)
{

        test_rw_byte_b(pdp);
        test_rw_word_b(pdp);
        test_w2b_rword_b(pdp);
}
//////////////////////////////////////////////////////////////////////
