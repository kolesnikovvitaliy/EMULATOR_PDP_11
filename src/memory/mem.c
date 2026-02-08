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
        memory->default_memory = default_memory;

        if (memory->default_memory) {
                fprintf(stdout, "MEMORY_BYTE\n");
                memory->mem_byte = (mem_byte_t*)mem_byte_new();
                mem_byte_create((struct mem_byte_t*)memory->mem_byte,
                        size_buffer_default);
                return;
        }
        fprintf(stdout, "MEMORY_WORD\n");
        memory->mem_word = (mem_word_t*)mem_word_new();
        mem_word_create((struct mem_word_t*)memory->mem_word,
                        size_buffer_default);

}

void mem_destroy(mem_t* memory)
{
        if (memory->default_memory) {
                 mem_byte_destroy((struct mem_byte_t*)memory->mem_byte);
                 free(memory->mem_byte);
                 fprintf(stdout, "MEMORY_BYTE_DELETED\n");
                 return;
        }
        mem_word_destroy((struct mem_word_t*)memory->mem_word);
        free(memory->mem_word);
        fprintf(stdout, "MEMORY_WORD_DELETED\n");
}

void byte_write(mem_t* memory,
                address_byte_t addr, byte_t data)
{
        if (memory->default_memory) {
                memory->mem_byte->write_byte(
                                memory->mem_byte,addr, data);
                return;
        }
        memory->mem_word->write_word(memory->mem_word,addr, data);
}


byte_t byte_read(mem_t* memory, address_byte_t addr)
{
        if (memory->default_memory) {
                 return (byte_t)memory->mem_byte->read_byte(memory->mem_byte, addr);
        }
        return (word_t)memory->mem_word->read_word(
                        memory->mem_word, addr);
}
//#######################################################
void word_write(mem_t* memory,
                address_word_t addr, word_t data)
{
        if (memory->default_memory) {
                memory->mem_byte->write_byte(memory->mem_byte,
                                addr, data);
                return;
        }
        memory->mem_word->write_word(memory->mem_word,addr, data);
}


word_t word_read(mem_t* memory, address_word_t addr)
{
        if (memory->default_memory) {
                 return (byte_t)memory->mem_byte->read_byte(memory->mem_byte,addr);
        }
        return (word_t)memory->mem_word->read_word(memory->mem_word,addr);
}

