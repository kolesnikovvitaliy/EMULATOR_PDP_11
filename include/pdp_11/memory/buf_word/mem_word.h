#ifndef MEM_WORD_H
#    define MEM_WORD_H
#    include "types/types.h"
struct mem_word_t;

struct mem_word_t *mem_word_new();

void mem_word_create(struct mem_word_t *, size_word_buffer);
void mem_word_destroy(struct mem_word_t *);

#endif
#pragma once
