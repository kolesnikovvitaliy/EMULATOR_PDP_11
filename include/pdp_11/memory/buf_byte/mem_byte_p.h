#ifndef MEM_BYTE_P_H
#    define MEM_BYTE_P_H

#    include "pdp_11/memory/type_func_p.h"
#    include "types/types.h"

typedef struct {
    byte_t *         buf_b;
    size_byte_buffer size_b;

    read_byte_t  read_byte;
    write_byte_t write_byte;
    read_word_t  read_word;
    write_word_t write_word;
} mem_byte_t;

#endif
#pragma once
