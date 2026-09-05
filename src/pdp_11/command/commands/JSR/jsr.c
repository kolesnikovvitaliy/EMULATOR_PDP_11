//##########################################################################
// JSR
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_jsr(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;

    if (!pdp) {
        return;
    };

    OP_CODE_T_INIT
    set_has_b = 0;
    opcode    = __get_mr(pdp, word_command, params);

    word_t target_pc = pdp_reg_get_var(pdp, 7);

    word_t target_sp = pdp_reg_get_var(pdp, 6);

    if (opcode.r_reg != 7) {
        w_write(pdp, target_sp, pdp_reg_get_var(pdp, opcode.r_reg));
        pdp_reg_set_var(
            pdp, opcode.r_reg, (word_t)(pdp_reg_get_var(pdp, 7) + 2));
    } else {
        w_write(pdp, target_sp, (word_t)(target_pc + 2));
        pdp_reg_set_var(pdp, 7, (word_t)((opcode.dd.addr) + 2));
    }

    pdp_reg_set_var(pdp, 7, (word_t)((opcode.dd.addr) - 2));

    pdp_reg_set_var(pdp, 6, (word_t)(target_sp - 2));
}
//##########################################################################
