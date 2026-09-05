//##########################################################################
// COMMAND HALT
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_halt(struct pdp_11_t *pdp,
                address_word_t   addr,
                word_t           word_command,
                byte_t           params)
{
    (void) addr;
    (void) params;
    (void) word_command;
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    word_t *  ptr_pc  = ptr_pdp->PC;
    *ptr_pc           = (word_t)(*ptr_pc + 2);
    tick++;
    PRINT_RESULT("\n\n---------------- halted ---------------", "");
    __command_reg_dump(pdp);

    if (!current_log_level) {
        fprintf(stdout, "\n");
    }
    PRINT_RESULT("\n\nTHE END!!!\n", "");
    pdp_destroy(pdp);
    free(pdp);
    exit(0);
}
//#########################################################################
