#include <stdio.h>
#include "pdp_11/pdp_11.h"
#include "memory/mem.h"
#include "tests/test.h"

size_t memory_byte_size = 65536;
size_t memory_word_size = 65536;

int main(int argc, char **argv)
{
        struct pdp_11_t* pdp = pdp_new();
        pdp_create(pdp, memory_word_size, memory_word_size);
        test_mem(pdp);

        pdp_destroy(pdp);

        return 0;
}
