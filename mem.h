#include <stdio.h>

#define MEMSIZE         (64*1024)

typedef unsigned char byte_t;
typedef unsigned short int word_t;
typedef word_t address_t;

//byte_t mem[MEMSIZE];
word_t mem[MEMSIZE];

void b_write(address_t adr, byte_t val); //пишем  значение (byte_t)val по адресу adr;
byte_t b_read(address_t adr);            // читаем байт по adr и возвращаем его;
void w_write(address_t adr, word_t val); // пишем значение (слово) val по адрессу adr;
word_t w_read(address_t adr);            // читаем слово по адрессу adr и возвращаем его;
