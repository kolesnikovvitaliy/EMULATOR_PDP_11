#ifndef MEM_WORD_P_H
#    define MEM_WORD_P_H

#    include "pdp_11/memory/type_func_p.h"
#    include "types/types.h"

typedef struct {
    word_t *         buf_w;
    size_word_buffer size_w;

    read_byte_t  read_byte;
    write_byte_t write_byte;
    read_word_t  read_word;
    write_word_t write_word;
} mem_word_t;

#endif
#pragma once
