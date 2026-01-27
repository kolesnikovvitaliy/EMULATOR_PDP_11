#include "mem.h"

void b_write(address_t addr, byte_t ch)
{
        mem_b[addr] = ch;
}
