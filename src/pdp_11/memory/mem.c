#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "memory/mem_p.h"
#include "types/types.h"
#include "memory/buf_byte/mem_byte.h"
#include "memory/buf_word/mem_word.h"
byte_t g_default_memory;
mem_t* mem_new()
{
        return (mem_t*)malloc(sizeof(mem_t));
}

void mem_create(mem_t* memory)
{
        memory->default_memory = g_default_memory;

        if (!(memory->default_memory)) {
                memory->mem_byte = (mem_byte_t*)mem_byte_new();
                mem_byte_create((struct mem_byte_t*)memory->mem_byte,
                        __get_size_buffer());
                return;
        }
        memory->mem_word = (mem_word_t*)mem_word_new();
        mem_word_create((struct mem_word_t*)memory->mem_word,
                        __get_size_buffer());

}

void mem_destroy(mem_t* memory)
{
        if (!(memory->default_memory)) {
                 mem_byte_destroy((struct mem_byte_t*)memory->mem_byte);
                 free(memory->mem_byte);
                 return;
        }
        mem_word_destroy((struct mem_word_t*)memory->mem_word);
        free(memory->mem_word);
}
//----------------------------------------------------------------------

static inline bool_t __is_valid_addr(mem_t* memory,
                address_word_t addr,
                 word_t len)
{

        if (!(memory->default_memory)) {
                if ((addr + len) > memory->mem_byte->size_b) {
                        abort();
                }
                return 1;
        }

        if ((addr + len) > memory->mem_word->size_w) {
                abort();
        }
        return 1;
}

static inline address_word_t __corect_addr_for_write_word(
                address_word_t addr)
{
        if ((addr & 1)) {
                addr ^= 1;
                return addr;

        }
        return addr;
}
//----------------------------------------------------------------------
void byte_write(mem_t* memory,
                address_byte_t addr, byte_t data)
{
        assert(__is_valid_addr(memory, addr, 1));
        if (!(memory->default_memory)) {
                memory->mem_byte->write_byte(
                                memory->mem_byte,addr, data);
                return;
        }

        //addr = __corect_addr_for_write_word(addr);
        assert(__is_valid_addr(memory, addr, 1));

        memory->mem_word->write_byte(memory->mem_word,addr, data);

}


byte_t byte_read(mem_t* memory, address_byte_t addr)
{
        assert(__is_valid_addr(memory, addr, 1));
        if (!(memory->default_memory)) {
                 return (byte_t)memory->mem_byte->read_byte(
                                 memory->mem_byte, addr);
        }

        //addr = __corect_addr_for_write_word(addr);
        assert(__is_valid_addr(memory, addr, 1));

        return (byte_t)memory->mem_word->read_byte(
                        memory->mem_word, addr);
}
//#######################################################
void word_write(mem_t* memory,
                address_word_t addr, word_t data)
{
        addr = __corect_addr_for_write_word(addr);
        assert(__is_valid_addr(memory, addr, 2));

        if (!(memory->default_memory)) {
                memory->mem_byte->write_word(memory->mem_byte,
                                addr, data);
                return;
        }

        memory->mem_word->write_word(memory->mem_word,addr, data);
}


word_t word_read(mem_t* memory, address_word_t addr)
{
        assert(__is_valid_addr(memory, addr, 2));
        if (!(memory->default_memory)) {
                 return (word_t)memory->mem_byte->read_word(
                                 memory->mem_byte,addr);
        }
        if (addr & 1) {
                addr--;
        }

        addr = __corect_addr_for_write_word(addr);
        assert(__is_valid_addr(memory, addr, 2));
        return (word_t)memory->mem_word->read_word(
                        memory->mem_word,addr);
}

