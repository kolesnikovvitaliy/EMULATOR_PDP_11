#ifndef MEM_BYTE_H
#define MEM_BYTE_H
#include "types/types.h"

struct mem_byte_t;

struct mem_byte_t* mem_byte_new();

void mem_byte_create(struct mem_byte_t*, const size_byte_buffer);
void mem_byte_destroy(struct mem_byte_t*);

#endif
#pragma once

