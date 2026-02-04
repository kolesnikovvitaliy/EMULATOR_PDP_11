#ifndef MEM_WORD_P_H
#define MEM_WORD_P_H

#include "types/types.h"

typedef struct {
        word_t* buf_w;
        size_word_buffer size_w;

        read_byte_funk_t read_byte_funk;
        write_byte_funk_t write_word_funk;
        read_word_funk_t read_word_funk;
        write_word_funk_t write_word_funk;
} mem_word_t;

#endif

