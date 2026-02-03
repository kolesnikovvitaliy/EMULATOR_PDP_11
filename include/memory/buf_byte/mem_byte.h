#ifndef MEM_BYTE_H
#define MEM_BYTE_H

#include "types/types.h"

struct mem_byte_t;

struct mem_byte_t* mem_byte_new();

void mem_byte_create(struct mem_byte_t*, size_byte_buffer);
void mem_byte_destroy(struct mem_byte_t*);


void byte_write(struct mem_byte_t*, address_byte_t, byte_t); //пишем  значение (byte_t)data по адресу adr;
byte_t byte_read(struct mem_byte_t*, address_byte_t);            // читаем байт по adr и возвращаем его;

#endif

