//##########################################################################
// JMP
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_jmp(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;
    OP_CODE_T_INIT
    set_has_b = 0;
    if (pdp) {
        opcode = __get_mr(pdp, word_command, params);
    }

    pdp_reg_set_var(pdp, 7, (word_t)(opcode.dd.addr - 2));
}
//##########################################################################
