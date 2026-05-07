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
    TRACE("%s\t", "test_parse_mov");
    assert(strcmp((char *) ptr_command->name, "mov") == 0);
    PRINT_RESULT("%s", " ... OK");
}

void
test_mode0(struct pdp_11_t *pdp)
{
    TRACE("%s\t", "test_mode0");
    *(((pdp_11_t *) pdp)->R3) = 012;
    *(((pdp_11_t *) pdp)->R5) = 034;
    //     reg[3] = 12;    // dd
    //     reg[5] = 34;    // ss
    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    // pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    if (pdp)
        opcode = __get_mr(pdp, (word_t) 0010503);
    assert(opcode.ss.value == 034);
    assert(opcode.ss.addr == 05);
    assert(opcode.dd.value == 012);
    assert(opcode.dd.addr == 03);
    PRINT_RESULT("%s", " ... OK\n");
}

int
test_mov(struct pdp_11_t *pdp, const address_word_t addr)
{
    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;
    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);
    test_parse_mov(run_command);
    test_mode0(pdp);
    // assert(strcmp((char *) run_command->name, "mov") == 0);

    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);
    TRACE("%s\t", "test_move R5 R3");
    assert(*(((pdp_11_t *) pdp)->R3) == 034);
    assert(*(((pdp_11_t *) pdp)->R5) == 034);
    PRINT_RESULT("%s", " ... OK\n");
    return 0;
}
