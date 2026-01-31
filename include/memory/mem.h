#ifndef MEM_H
#define MEM_H

/*#define MEMSIZE         (64*1024)*/

struct byte_t;
struct word_t;
struct address_byte_t;
struct address_word_t;

struct mem_t;

struct mem_t* mem_new();

void mem_create(struct mem_t*, size_t*);
void mem_destroy(struct mem_t*);

void b_write(struct mem_t*, const address_byte_t*, const struct byte_t*); //пишем  значение (byte_t)data по адресу adr;
struct byte_t b_read(struct mem_t*, const address_byte_t*);            // читаем байт по adr и возвращаем его;

void w_write(struct mem_t*, const address_word_t*, const struct word_t*); // пишем значение (слово) val по адрессу adr;
struct word_t w_read(struct mem_t*, const address_word_t*);            // читаем слово по адрессу adr и возвращаем его;

#endif
