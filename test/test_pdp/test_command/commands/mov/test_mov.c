#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/pdp_11.h"
#include "tests/test_pdp/test_command/test_command.h"
#include "types/types.h"
// #include "utils/logger/logger.h"
// #include "utils/utils.h"
//
// #include <assert.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

int
test_mov(struct pdp_11_t *pdp, word_t *ptr_pc)
{
    command_do_mov(pdp, *ptr_pc, w_read(pdp, *ptr_pc), (byte_t) 1);
    return 0;
}
