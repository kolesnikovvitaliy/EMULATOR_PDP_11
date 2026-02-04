#ifndef MEM_WORD_H
#define MEM_WORD_H

#include "types/types.h"

struct mem_word_t;

struct mem_word_t* mem_word_new();

void mem_word_create(struct mem_word_t*, size_word_buffer);
void mem_word_destroy(struct mem_word_t*);

void byte_write(struct mem_word_t*, address_word_t, byte_t); //пишем  значение (byte_t)data по адресу adr;
byte_t byte_read(struct mem_word_t*, address_word_t);            // читаем байт по adr и возвращаем его;

void word_write(struct mem_word_t*, address_word_t, word_t); //пишем  значение (word_t)data по адресу adr;
word_t word_read(struct mem_word_t*, address_word_t);            // читаем слово по adr и возвращаем его;

#endif
