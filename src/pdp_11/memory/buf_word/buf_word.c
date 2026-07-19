#include "pdp_11/memory/buf_word/mem_word_p.h"
#include "pdp_11/memory/type_func_p.h"
#include "utils/logger/logger.h"

#include <stdio.h>
#include <stdlib.h>

//--------------------------------------------------------------
void   __byte_write_w(void *, address_byte_t, byte_t);
byte_t __byte_read_w(void *, address_byte_t);
word_t __word_read_w(void *, address_word_t);
void   __word_write_w(void *, address_word_t, word_t);
//--------------------------------------------------------------

/////////////////////////////////////////////////////////////////
mem_word_t *
mem_word_new(void)
{
    // Выделение памяти для класса памяти типа WORD;
    return (mem_word_t *) malloc(sizeof(mem_word_t));
}

void
mem_word_create(mem_word_t *mem_word, size_word_buffer size)
{
    // Выделение памяти для работы PDP_11 в режие типа WORD;
    mem_word->buf_w  = (word_t *) calloc(size / 2, sizeof(word_t));
    mem_word->size_w = size / 2;

    // Полморфные функции работы с памятью типа WORD или BYTE;
    mem_word->read_byte  = __byte_read_w;
    mem_word->write_byte = __byte_write_w;
    mem_word->read_word  = __word_read_w;
    mem_word->write_word = __word_write_w;
}

void
mem_word_destroy(mem_word_t *mem_word)
{
    // Полморфные функции работы с памятью типа WORD или BYTE;
    free(mem_word->buf_w);
}
/////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------
void
__word_write_w(void *mem_word, address_word_t addr, word_t data)
{
    // Записываем слово по адресу;
    mem_word_t *ptr = (mem_word_t *) mem_word;

    *(ptr->buf_w + addr)     = data & 0xFF;
    *(ptr->buf_w + addr + 1) = (data >> 8) & 0xFF;
}

//------------------------------------------------------------------
word_t
__word_read_w(void *mem_word, address_word_t addr)
{
    // Читаем слово по адресу;
    mem_word_t *ptr = (mem_word_t *) mem_word;
    return (word_t)((*(ptr->buf_w + addr) & 0xFF)
                    | *(ptr->buf_w + addr + 1) << 8);
}
//------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------
void
__byte_write_w(void *mem_word, address_byte_t addr, byte_t data)
{
    // Записываем байт в слово по адресу;
    mem_word_t *ptr = (mem_word_t *) mem_word;

    *(ptr->buf_w + addr) = data & 0xFF;
}

//------------------------------------------------------------------
byte_t
__byte_read_w(void *mem_word, address_byte_t addr)
{
    // Читаем байт из слова по адресу;
    mem_word_t *ptr = (mem_word_t *) mem_word;

    // return (byte_t) * (ptr->buf_w + addr) & 0xFF;
    return (byte_t) * (ptr->buf_w + addr);
}
//------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////
