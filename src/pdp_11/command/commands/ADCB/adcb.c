//##########################################################################
// ADCB
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_adcb(struct pdp_11_t *pdp,
                address_word_t   addr,
                word_t           word_command,
                byte_t           params)
{
    (void) addr;

    OP_CODE_T_INIT
    if (!pdp) {
        return;
    }
    opcode = __get_mr(pdp, word_command, params);

    w_write(pdp, opcode.dd.addr, opcode.ss.value);
}
//##########################################################################
