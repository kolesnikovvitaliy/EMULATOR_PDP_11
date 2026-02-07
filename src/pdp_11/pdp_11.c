#include <stdio.h>
#include <stdlib.h>

#include "pdp_11/pdp_11_p.h"
#include "memory/mem.h"


pdp_11_t* pdp_new()
{
        return (pdp_11_t*)malloc(sizeof(pdp_11_t));
}

void pdp_create(pdp_11_t* pdp)
{
        pdp->memory = (struct mem_t*)mem_new();
        mem_create(pdp->memory);
}

void pdp_destroy(pdp_11_t* pdp)
{
        mem_destroy((struct mem_t*)pdp->memory);
        free(pdp->memory);
}

void b_write(pdp_11_t* pdp, address_byte_t addr, byte_t data)
{
        byte_write((struct mem_t*)pdp->memory, addr, data);
}

byte_t b_read(pdp_11_t* pdp, address_byte_t addr)
{
        return (byte_t)byte_read((struct mem_t*)pdp->memory, addr);
}

//------------------------------------------------------------------;
void w_write(pdp_11_t* pdp,
                address_word_t addr, word_t data)
{
        word_write((struct mem_t*)pdp->memory, addr, data);
}

word_t w_read(pdp_11_t* pdp, address_word_t addr)
{
        return (word_t)word_read((struct mem_t*)pdp->memory, addr);
}

