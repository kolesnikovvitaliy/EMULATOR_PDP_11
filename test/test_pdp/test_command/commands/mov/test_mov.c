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
    pdp_reg_set_var(pdp, 7, 01000);
    PRINT_RESULT("\r\n", "");

    pdp_reg_set_var(pdp, 3, 012);
    pdp_reg_set_var(pdp, 5, 034);

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        opcode = __get_mr(pdp, (word_t) 0010503);
    }

    PRINT_RESULT("\r      ", "");
    TRACE("%s\t", "test_mode0");
    assert(opcode.ss.value == 034);
    assert(opcode.ss.addr == 05);
    assert(opcode.dd.value == 012);
    assert(opcode.dd.addr == 03);

    PRINT_RESULT("%s", " ... OK\n");
}

void
test_mode1_toreg(struct pdp_11_t *pdp, const address_word_t addr)
{
    pdp_reg_set_var(pdp, 7, 01000);
    for (int i = 0; i <= 6; i++) {
        pdp_reg_set_var(pdp, i, 00);
    }
    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;

    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    // setup
    pdp_reg_set_var(pdp, 2, 012);  // dd
    pdp_reg_set_var(pdp, 5, 0200); // ss

    w_write(pdp, 0200, 034);

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
    }

    PRINT_RESULT("\r          ", "");
    TRACE("%s", "test_mode1_toreg  ");
    assert(opcode.ss.value == 034);
    assert(opcode.ss.addr == 0200);
    assert(opcode.dd.value == 012);
    assert(opcode.dd.addr == 02);

    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);

    assert(pdp_reg_get_var(pdp, 2) == 034);
    assert(pdp_reg_get_var(pdp, 5) == 0200);

    PRINT_RESULT("  %s", " ... OK\n");
}

void
test_mode1_reg_to_mem(struct pdp_11_t *pdp, const address_word_t addr)
{
    pdp_reg_set_var(pdp, 7, 01000);
    for (int i = 0; i <= 6; i++) {
        pdp_reg_set_var(pdp, i, 0);
    }
    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;

    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    // setup
    pdp_reg_set_var(pdp, 3, 066);  // dd
    pdp_reg_set_var(pdp, 5, 0200); // ss
    w_write(pdp, 0200, 034);

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
    }

    PRINT_RESULT("\r          ", "");
    TRACE("%s", "test_mode1_reg_to_mem  ");

    assert(opcode.ss.value == 066);
    assert(opcode.ss.addr == 03);
    assert(opcode.dd.value == 034);
    assert(opcode.dd.addr == 0200);

    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);

    assert(w_read(pdp, opcode.dd.addr) == 066);

    PRINT_RESULT("  %s", " ... OK\n");
}
void
test_mode1_mem_to_mem(struct pdp_11_t *pdp, const address_word_t addr)
{
    pdp_reg_set_var(pdp, 7, 01000);
    for (int i = 0; i <= 6; i++) {
        pdp_reg_set_var(pdp, i, 00);
    }
    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;

    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    // setup
    pdp_reg_set_var(pdp, 1, 0200); // dd
    pdp_reg_set_var(pdp, 4, 0210); // ss

    w_write(pdp, 0200, 055);
    w_write(pdp, 0210, 011);

    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        opcode = __get_mr(pdp, (word_t) w_read(pdp, addr));
    }

    PRINT_RESULT("\r          ", "");
    TRACE("%s", "test_mode1_mem_to_mem  ");

    assert(opcode.ss.value == 055);
    assert(opcode.ss.addr == 0200);
    assert(opcode.dd.value == 011);
    assert(opcode.dd.addr == 0210);

    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);

    assert(w_read(pdp, opcode.dd.addr) == 055);

    PRINT_RESULT("  %s", " ... OK\n");
}

int
test_mov(struct pdp_11_t *pdp, const address_word_t addr)
{
    pdp_reg_set_var(pdp, 7, 01000);
    pdp_11_t * ptr_pdp = (pdp_11_t *) pdp;
    command_t *run_command;

    run_command = __ptr_command(
        (pdp_11_t *) pdp, (command_t **) ptr_pdp->command, addr);

    test_parse_mov(run_command);
    test_mode0(pdp);

    // PRINT_RESULT("\r", "");
    TRACE("%s", "test_move  ");
    run_command->do_commands_command(pdp, addr, w_read(pdp, addr), (byte_t) 1);

    assert(pdp_reg_get_var(pdp, 3) == 034);
    assert(pdp_reg_get_var(pdp, 5) == 034);
    PRINT_RESULT("  %s", "... OK\n");
    return 0;
}
