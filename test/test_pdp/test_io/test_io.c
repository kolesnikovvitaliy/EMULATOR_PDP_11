#include "tests/test_pdp/test_io/test_io.h"

#include "pdp_11/pdp_11.h"
#include "types/types.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
//-------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
int
test_io(struct pdp_11_t *pdp, int argc, char **argv)
{
    assert(pdp);
    byte_t *filename = pdp_parse_filename(argc, argv);
    TRACE("TEST PDP LOAD DATA", "");
    pdp_load_data(pdp, (byte_t *) filename);
    PRINT_RESULT("%s", "   ...OK\n");
    // pdp_mem_dump(pdp, 0x40, 0x20);
    // pdp_mem_dump(pdp, 0x200, 0x26);
    return 0;
}
//////////////////////////////////////////////////////////////////////
