#include <stdio.h>
#include <stdlib.h>

#include "memory/mem_p.h"
#include "types/types.h"
#include "memory/buf_byte/mem_byte.h"
#include "memory/buf_word/mem_word.h"

mem_t* mem_new()
{
        return (mem_t*)malloc(sizeof(mem_t));
}

void mem_create(mem_t* memory)
{
        memory->mem_byte = (mem_byte_t*)mem_byte_new();
        mem_byte_create((struct mem_byte_t*)memory->mem_byte, size_buffer_default);

        memory->mem_word = (mem_word_t*)mem_word_new();
        mem_word_create((struct mem_word_t*)memory->mem_word, size_buffer_default);

}

void mem_destroy(mem_t* memory)
{
        mem_byte_destroy((struct mem_byte_t*)memory->mem_byte);
        mem_word_destroy((struct mem_word_t*)memory->mem_word);
}

void byte_write(mem_t* memory,
                address_byte_t addr, byte_t data)
{
        memory->mem_byte->write_byte(memory->mem_byte,addr, data);
}


byte_t byte_read(mem_t* memory, address_byte_t addr)
{
        return (byte_t)memory->mem_byte->read_byte(memory->mem_byte, addr);
}
//#######################################################
void word_write(mem_t* memory,
                address_word_t addr, word_t data)
{
        memory->mem_word->write_word(memory->mem_byte,addr, data);
}


word_t word_read(mem_t* memory, address_word_t addr)
{
        return (word_t)memory->mem_word->read_word(memory->mem_byte,addr);
}
