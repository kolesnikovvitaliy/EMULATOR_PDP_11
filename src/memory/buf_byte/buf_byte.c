#include <stdio.h>
#include <stdlib.h>

#include "memory/buf_byte/mem_byte_p.h"
#include "memory/type_func_p.h"

///////////////////////////////////////////////////////////////////
void __byte_write(void* mem_byte,
               address_byte_t addr,
               byte_t data)
{
        mem_byte_t *ptr = (mem_byte_t*)mem_byte;
        *(ptr->buf_b + addr) = data;
}

byte_t
__byte_read(void* mem_byte, address_byte_t addr)
{
        mem_byte_t *ptr = (mem_byte_t*)mem_byte;
        return *(ptr->buf_b + addr);
}

///////////////////////////////////////////////////////////////////
mem_byte_t* mem_byte_new()
{
       return (mem_byte_t*)malloc(sizeof(mem_byte_t));
}
void mem_byte_create(mem_byte_t* mem_byte, size_byte_buffer size)
{
        mem_byte->buf_b = (byte_t*)calloc(size, sizeof(byte_t));
        mem_byte->size_b = size;

        mem_byte->read_byte = __byte_read;
        mem_byte->write_byte = __byte_write;
}

void mem_byte_destroy(mem_byte_t* mem_byte)
{
        free(mem_byte->buf_b);
}

//void byte_write(mem_byte_t* mem_byte,
//               address_byte_t addr,
//               byte_t data)
//{
//      *(mem_byte->buf_b + addr) = data;
//}
//
//byte_t
//byte_read(mem_byte_t* mem_byte, address_byte_t addr)
//{
//       return (byte_t)*(mem_byte->buf_b + addr);
//}
