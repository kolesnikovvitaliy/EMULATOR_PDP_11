#include <stdio.h>
#include <assert.h>
#include "utils/logger/logger.h"
#include "tests/test_pdp/test_memory/test_memory_word/test_memory_word.h"

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

        DEBUG("\nПишем и читаем байт по четному адресу\r\n", "");

        b_write(pdp, adr, b0);
        bres = b_read(pdp, adr);

        DEBUG("\na = %06o\t b0 = %hhx\t bres = %hhx\r\n", adr, b0, bres);
        assert(bres == b0);
}

void test_rw_word_w(struct pdp_11_t* pdp)
{
        // пишем слово, читаем слово.

        address_word_t adr;
        word_t w, wres;

        adr = 8;
        w = 0x3456;

        DEBUG("\nПишем и читаем слово\r\n", "");

        w_write(pdp, adr, w);
        wres = w_read(pdp, adr);
        if (adr & 1) (adr--);

        DEBUG("\na = %06x\t w = %04x\t wres = %04x\n", adr, w, wres);
        assert(w == wres);

}

void test_w2b_rword_w(struct pdp_11_t* pdp)
{
        // пишем 2 байта, читаем 1 слов;
    address_word_t a;
    byte_t b0, b1;
    word_t w, wres;
    DEBUG("\nПишем 2 байта, читаем слово\n", "");
    a = 4;        // другой адрес
    w = 0xa1b2;
    // little-endian, младшие разряды по меньшему адресу
    b0 = 0xb2;
    b1 = 0xa1;
    b_write(pdp, a, b0);
    b_write(pdp, a+1, b1);
    wres = w_read(pdp, a);
    // тут полезно написать отладочную печать a, w, wres
    DEBUG("\na=%06o\t b1=%02hhx\t b0=%02hhx\t wres=%04x\n", a, b1, b0, wres);
    assert(w == wres);
}

void test_wword_r2b_w(struct pdp_11_t* pdp)
{

    address_word_t a;
    byte_t b0, b1, res_b0, res_b1;
    word_t w;
    DEBUG("\nПишем слово читаем 2 байтита\n", "");
    a = 4;        // другой адрес
    w = 0xa1b2;
    // little-endian, младшие разряды по меньшему адресу
    res_b0 = 0xb2;
    res_b1 = 0xa1;
    w_write(pdp, a, w);
    b0 = b_read(pdp, a);
    b1 = b_read(pdp, a+1);
    // тут полезно написать отладочную печать a, w, wres
    DEBUG("\na=%06o\t b1=%02hhx\t b0=%02hhx\t w=%04x\n", a, b1, b0, w);
    assert(b1 == res_b1);
    assert(b0 == res_b0);
}

//--------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void test_word_buffer(struct pdp_11_t* pdp)
{
        DEBUG("\r\n\t\t TEST_WORD_BUFFER\r\n\n", "");

        test_rw_word_w(pdp);
        test_rw_word_w(pdp);
        test_w2b_rword_w(pdp);
        test_wword_r2b_w(pdp);
}
//////////////////////////////////////////////////////////////////////
