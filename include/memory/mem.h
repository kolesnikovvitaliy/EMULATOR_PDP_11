#ifndef MEM_H
#define MEM_H

#include "types/types.h"


struct mem_t;

struct mem_t* mem_new();

void mem_create(struct mem_t*);
void mem_destroy(struct mem_t*);

void byte_write(struct mem_t*, address_byte_t, byte_t); //пишем  значение (byte_t)data по адресу adr;
byte_t byte_read(struct mem_t*, address_byte_t);            // читаем байт по adr и возвращаем его;

void word_write(struct mem_t*, address_word_t, word_t); // пишем значение (слово) val по адрессу adr;
word_t word_read(struct mem_t*, address_word_t);            // читаем слово по адрессу adr и возвращаем его;

#endif
