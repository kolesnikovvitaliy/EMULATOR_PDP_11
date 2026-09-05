//##########################################################################
// TSTB
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_tstb(struct pdp_11_t *pdp,
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

    byte_t byte_value = 0;
    if (opcode.dd.addr & 1) {

        byte_value = (byte_t)((opcode.dd.value >> 8) & 0xFF);
    } else {

        byte_value = (byte_t)(opcode.dd.value & 0xFF);
    }

    SET_PSW_BIT(psw, Z, byte_value == 0);
    SET_PSW_BIT(psw, N, (byte_value >> 7) & 1);

    if (opcode.dd.addr == 0177564 || opcode.dd.addr == 0177560) {
        SET_PSW_BIT(psw, Z, 0);
        SET_PSW_BIT(psw, N, 1);
    }

    SET_PSW_BIT(psw, C, 0);
    SET_PSW_BIT(psw, V, 0);
}
//#########################################################################
