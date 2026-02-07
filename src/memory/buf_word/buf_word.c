#include <stdio.h>
#include <stdlib.h>
#include "memory/buf_word/mem_word_p.h"
#include "memory/type_func_p.h"

mem_word_t* mem_word_new()
{
        return (mem_word_t*)malloc(sizeof(mem_word_t));
}

void mem_word_create(mem_word_t* mem_word, size_word_buffer size)
{
        mem_word->buf_w = (word_t*)calloc(size, sizeof(word_t));
        mem_word->size_w = size;
}

void mem_word_destroy(mem_word_t* mem_word)
{
        free(mem_word->buf_w);
}

// void word_write(mem_word_t* mem_word,
//                 address_word_t addr,
//                 word_t data)
// {
//        *(mem_word->buf_w + (word_t)addr) = data;
// }
//
// word_t
// word_read(mem_word_t* mem_word, const address_word_t *addr)
// {
//         return (word_t)*(mem_word->buf_w + *(word_t*)addr);
// }
