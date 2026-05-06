#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/pdp_11.h"
#include "tests/test_pdp/test_command/test_command.h"
#include "types/types.h"
#include "utils/logger/logger.h"
// #include "utils/logger/logger.h"
// #include "utils/utils.h"
//
#include <assert.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <string.h>

void
test_parse_mov(command_t *ptr_command)
{
    TRACE("%s\n", "test_parse_mov");
    assert(strcmp((char *) ptr_command->name, "mov") == 0);
    TRACE("%s\n", " ... OK");
}

// void test_mode0(struct pdp_11_t *pdp)
// {
//     TRACE("%s\n", "test_mode0");
//     reg[3] = 12;    // dd
//     reg[5] = 34;    // ss
//     Command cmd = parse_cmd(0010503);
//     assert(ss.val == 34);
//     assert(ss.adr == 5);
//     assert(dd.val == 12);
//     assert(dd.adr == 3);
//     trace(TRACE, " ... OK\n");
// }

int
test_mov(struct pdp_11_t *pdp, const address_word_t addr)
{
    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);
    test_parse_mov(run_command);
    // test_mode0(pdp)
    // assert(strcmp((char *) run_command->name, "mov") == 0);

    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);
    return 0;
}
