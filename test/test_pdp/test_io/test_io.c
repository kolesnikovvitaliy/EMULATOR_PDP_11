#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pdp_11/pdp_11.h"
#include "tests/test_pdp/test_io/test_io.h"
#include "types/types.h"
//-------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
 int test_io(struct pdp_11_t* pdp, int argc, char** argv)
{
        assert(pdp);
        if (argc <= 1) {
                test_io_terminal((struct pdp_11_t*)pdp);
                return 0;
        }
        test_io_files((struct pdp_11_t*)pdp, (byte_t*)argv[2]);
        return 1;


}
//////////////////////////////////////////////////////////////////////
