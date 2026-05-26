#include "tests/test_pdp/test_command/modes/test_mode1/test_mode1.h"

#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/pdp_11.h"
#include "tests/test_pdp/test_command/test_command.h"
#include "types/types.h"
#include "utils/logger/logger.h"

#include <assert.h>
#include <string.h>

void
test_mode1(struct pdp_11_t *pdp)
{
    address_word_t addr = 01000;

    w_write(pdp, addr, (word_t) 0011502);
    test_mode1_toreg(pdp, addr);

    w_write(pdp, addr, (word_t) 0010315);
    test_mode1_reg_to_mem(pdp, addr);

    w_write(pdp, addr, (word_t) 0011114);
    test_mode1_mem_to_mem(pdp, addr);
}
