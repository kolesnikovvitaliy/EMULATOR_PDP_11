#include "pdp_11/memory/buf_byte/mem_byte_p.h"
#include "pdp_11/memory/type_func_p.h"

#include <stdio.h>
#include <stdlib.h>

//--------------------------------------------------------------
void   __byte_write_b(void *, address_byte_t, byte_t);
byte_t __byte_read_b(void *, address_byte_t);
word_t __word_read_b(void *, address_word_t);
void   __word_write_b(void *, address_word_t, word_t);
//--------------------------------------------------------------

/////////////////////////////////////////////////////////////////////

mem_byte_t *
mem_byte_new(void)
{
    return (mem_byte_t *) malloc(sizeof(mem_byte_t));
}

void
mem_byte_create(mem_byte_t *mem_byte, const size_byte_buffer size)
{
    mem_byte->buf_b  = (byte_t *) calloc(size, sizeof(byte_t));
    mem_byte->size_b = size;

    mem_byte->read_byte  = __byte_read_b;
    mem_byte->write_byte = __byte_write_b;
    mem_byte->read_word  = __word_read_b;
    mem_byte->write_word = __word_write_b;
}

void
mem_byte_destroy(mem_byte_t *mem_byte)
{
    free(mem_byte->buf_b);
}

/////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------
// Записывает слово по адресу
void
__word_write_b(void *mem_byte, address_word_t addr, word_t data)
{
    mem_byte_t *ptr = (mem_byte_t *) mem_byte;

    *(ptr->buf_b + addr)     = (byte_t)(data & 0xFF);
    *(ptr->buf_b + addr + 1) = (byte_t)((data >> 8) & 0xFF);
}

//-----------------------------------------------------------------------
// Читает слово по адресу
word_t
__word_read_b(void *mem_byte, address_word_t addr)
{
    mem_byte_t *ptr = (mem_byte_t *) mem_byte;

    return (word_t)((*(ptr->buf_b + addr) & 0xFF)
                    | (*(ptr->buf_b + addr + 1) << 8));
}
//-----------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------
void
__byte_write_b(void *mem_byte, address_byte_t addr, byte_t data)
{
    mem_byte_t *ptr = (mem_byte_t *) mem_byte;
    //*(ptr->buf_b + addr) = data & 0xFF;
    *(ptr->buf_b + addr) = data;
}

//-----------------------------------------------------------------------
byte_t
__byte_read_b(void *mem_byte, address_byte_t addr)
{
    mem_byte_t *ptr = (mem_byte_t *) mem_byte;
    // return (byte_t) * (ptr->buf_b + addr) & 0xFF;
    return (byte_t) * (ptr->buf_b + addr);
}
//-----------------------------------------------------------------------
