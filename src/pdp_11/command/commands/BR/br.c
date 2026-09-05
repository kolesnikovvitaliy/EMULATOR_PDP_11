//##########################################################################
// BR
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_br(struct pdp_11_t *pdp,
              address_word_t   addr,
              word_t           word_command,
              byte_t           params)
{
    (void) addr;

    if (!pdp) {
        return;
    }
    OP_CODE_T_INIT
    set_has_b = 0;
    opcode    = __get_mr(pdp, word_command, params);

    word_t PC = pdp_reg_get_var(pdp, 7);
    pdp_reg_set_var(pdp, 7, (word_t)(PC + opcode.offset_xx * 2));
    PRINT_RESULT("%06o", (word_t)(pdp_reg_get_var(pdp, 7) + 2));

    return;
}
//##########################################################################
