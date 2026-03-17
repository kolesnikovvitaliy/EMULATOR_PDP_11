#ifndef PDP_11_H
#define PDP_11_H

#include "types/types.h"
#include "utils/logger/logger.h"
struct pdp_11_t;

struct pdp_11_t* pdp_new();

void pdp_create(struct pdp_11_t*);
void pdp_destroy(struct pdp_11_t*);


void b_write(struct pdp_11_t* , const address_byte_t, byte_t);//пишем  значение (byte_t)data по адресу adr;
byte_t b_read(struct pdp_11_t* , const address_byte_t);// читаем байт по adr и возвращаем его;

void w_write(struct pdp_11_t* , const address_word_t, word_t);// пишем значение (слово) val по адрессу adr;
word_t w_read(struct pdp_11_t* , const address_word_t);// читаем слово по адрессу adr и возвращаем его;

void pdp_load_data(struct pdp_11_t* pdp, byte_t* filename);
void pdp_mem_dump(struct pdp_11_t* pdp, address_word_t addr, word_t size);
byte_t* pdp_parse_filename(int argc, char **argv);

#endif
#pragma once
