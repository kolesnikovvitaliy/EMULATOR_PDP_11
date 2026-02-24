#include <stdio.h>
#include <stdlib.h>

#include "memory/buf_word/mem_word_p.h"
#include "memory/type_func_p.h"


//--------------------------------------------------------------
void __byte_write_w(void*, address_byte_t, byte_t);
byte_t __byte_read_w(void* , address_byte_t);
word_t __word_read_w(void*, address_word_t);
void __word_write_w(void*, address_word_t, word_t);
//--------------------------------------------------------------

/////////////////////////////////////////////////////////////////
mem_word_t* mem_word_new()
{
        return (mem_word_t*)malloc(sizeof(mem_word_t));
}

void mem_word_create(mem_word_t* mem_word, size_word_buffer size)
{
        mem_word->buf_w = (word_t*)calloc(size, sizeof(word_t));
        mem_word->size_w = size;

        mem_word->read_byte = __byte_read_w;
        mem_word->write_byte = __byte_write_w;
        mem_word->read_word = __word_read_w;
        mem_word->write_word = __word_write_w;
}

void mem_word_destroy(mem_word_t* mem_word)
{
        free(mem_word->buf_w);
}
/////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------
void __word_write_w(void* mem_word,
                address_word_t addr,
                word_t data)
{
        mem_word_t *ptr = (mem_word_t*)mem_word;

        *(ptr->buf_w + addr) = data & 0xFF;
        *(ptr->buf_w + addr + 1) =  (data >> 8) & 0xFF;
}

//------------------------------------------------------------------
word_t
__word_read_w(void* mem_word, address_word_t addr)
{
        mem_word_t *ptr = (mem_word_t*)mem_word;
        return ((*(ptr->buf_w + addr) & 0xFF) | *(ptr->buf_w + addr+1) << 8);
}
//------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------
void __byte_write_w(void* mem_word,
               address_byte_t addr,
               byte_t data)
{
        mem_word_t *ptr = (mem_word_t*)mem_word;

        *(ptr->buf_w + addr) = data & 0xFF;


}

//------------------------------------------------------------------
byte_t
__byte_read_w(void* mem_word, address_byte_t addr)
{
        mem_word_t *ptr = (mem_word_t*)mem_word;

        return *(ptr->buf_w + addr) & 0xFF;
}
//------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////
