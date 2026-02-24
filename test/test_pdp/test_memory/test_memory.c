#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tests/test_pdp/test_memory/test_memory_byte/test_memory_byte.h"
#include "tests/test_pdp/test_memory/test_memory_word/test_memory_word.h"
//-------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
void test_memory(struct pdp_11_t* pdp)
{
        assert(pdp);


        test_byte_buffer(pdp);
        test_word_buffer(pdp);


}
//////////////////////////////////////////////////////////////////////
