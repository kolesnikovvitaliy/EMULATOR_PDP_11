#include "tests/test_pdp/test_memory/test_memory_word/test_memory_word.h"
#include "utils/logger/logger.h"

#include <assert.h>
#include <stdio.h>

//--------------------------------------------------------------------
/* Записать байт по адресу */
/* Прочитать байт по адресу t*/

void
test_rw_byte_w(struct pdp_11_t *pdp)
{
    //пишем байт, читаем байт

    address_word_t adr;
    word_t         b0, bres;

    adr = 0;
    b0  = 0x12;

    TRACE_LOG("Пишем и читаем байт по четному адресу", "");

    b_write(pdp, adr, (byte_t) b0);
    bres = (word_t) b_read(pdp, adr);
    assert(bres == b0);

    PRINT_RESULT("%s", " ... OK");
}

void
test_rw_word_w(struct pdp_11_t *pdp)
{
    // пишем слово, читаем слово.

    address_word_t adr;
    word_t         w, wres;

    adr = 8;
    w   = 0x3456;

    TRACE_LOG("Пишем и читаем слово", "");

    w_write(pdp, adr, w);
    wres = w_read(pdp, adr);
    if (adr & 1)
        (adr--);

    // TRACE(" a = %06x\t w = %04x\t wres = %04x\n", adr, w, wres);
    assert(w == wres);
    PRINT_RESULT("%s", " ... OK");
}

void
test_w2b_rword_w(struct pdp_11_t *pdp)
{
    // пишем 2 байта, читаем 1 слов;
    address_word_t a;
    byte_t         b0, b1;
    word_t         w, wres;
    TRACE_LOG("Пишем 2 байта, читаем слово", "");
    a = 4; // другой адрес
    w = 0xa1b2;
    // little-endian, младшие разряды по меньшему адресу
    b0 = (byte_t) 0xb2;
    b1 = (byte_t) 0xa1;
    b_write(pdp, a, b0);
    b_write(pdp, (address_byte_t)(a + 1), b1);
    wres = w_read(pdp, a);
    // тут полезно написать отладочную печать a, w, wres
    // TRACE(" a=%06o\t b1=%02hhx\t b0=%02hhx\t wres=%04x\n", a, b1, b0, wres);
    assert(w == wres);
    PRINT_RESULT("%s", " ... OK");
}

void
test_wword_r2b_w(struct pdp_11_t *pdp)
{

    address_word_t a;
    byte_t         b0, b1, res_b0, res_b1;
    word_t         w;
    TRACE_LOG("Пишем слово читаем 2 байтита", "");
    a = 4; // другой адрес
    w = 0xa1b2;
    // little-endian, младшие разряды по меньшему адресу
    res_b0 = (byte_t) 0xb2;
    res_b1 = (byte_t) 0xa1;
    w_write(pdp, a, w);
    b0 = b_read(pdp, a);
    b1 = b_read(pdp, (address_byte_t)(a + 1));
    // тут полезно написать отладочную печать a, w, wres
    // TRACE(" a=%06o\t b1=%02hhx\t b0=%02hhx\t w=%04x\n", a, b1, b0, w);
    assert(b1 == res_b1);
    assert(b0 == res_b0);
    PRINT_RESULT("%s", " ... OK");
}

//--------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void
test_word_buffer(struct pdp_11_t *pdp)
{
    INFO_LOG("TEST MEMORY TYPE ==WORD== START", "");

    test_rw_byte_w(pdp);
    test_rw_word_w(pdp);
    test_w2b_rword_w(pdp);
    test_wword_r2b_w(pdp);
    INFO_LOG("TEST MEMORY TYPE ==WORD== WAS PASSED SUCCESSFULLY\n", "");
}
//////////////////////////////////////////////////////////////////////
