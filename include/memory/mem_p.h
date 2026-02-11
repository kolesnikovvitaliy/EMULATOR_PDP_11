#ifndef MEM_P_H
#define MEM_P_H
#include "types/types.h"


#include "buf_byte/mem_byte_p.h"
#include "buf_word/mem_word_p.h"

typedef struct {
        mem_byte_t* mem_byte;
        mem_word_t* mem_word;
        byte_t default_memory; // 1 = mem_byte, 0 = mem_word;
} mem_t;


#endif

