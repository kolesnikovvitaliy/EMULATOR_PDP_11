//##########################################################################
// TST
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_tst(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;
    OP_CODE_T_INIT
    set_has_b = 0;
    if (!pdp) {
        return;
    }
    opcode = __get_mr(pdp, word_command, params);

    SET_PSW_BIT(psw, Z, (opcode.dd.value & 0xFFFF) == 0);
    SET_PSW_BIT(psw, N, (opcode.dd.value >> 15) & 1);

    if (opcode.dd.addr == 0177564) {
        SET_PSW_BIT(psw, N, 1);
    }

    SET_PSW_BIT(psw, V, 0);
    SET_PSW_BIT(psw, C, 0);
}
//#########################################################################
