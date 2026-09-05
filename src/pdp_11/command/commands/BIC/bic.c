//##########################################################################
// BIC, BICB
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_bic(struct pdp_11_t *pdp,
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
    word_16_t s = (word_16_t) opcode.ss.value;
    word_16_t d = (word_16_t) opcode.dd.value;

    word_16_t res = (word_16_t)(d & (~s) & 0xFFFF);
    if (opcode.dd.addr < 8) {
        pdp_reg_set_var(pdp, opcode.dd.addr, (word_t) res);
    } else {
        w_write(pdp, opcode.dd.addr, (word_t) res);
    }

    set_flag_NZ((word_t) res);
    SET_PSW_BIT(psw, V, 0);
}
//-------------------------------------------------------------------------
void
command_do_bicb(struct pdp_11_t *pdp,
                address_word_t   addr,
                word_t           word_command,
                byte_t           params)
{
    (void) addr;
    OP_CODE_T_INIT
    if (pdp) {
        opcode = __get_mr(pdp, word_command, params);
    }
    byte_t s = (byte_t)(opcode.ss.value & 0xFF);
    byte_t d = (byte_t)(opcode.dd.value & 0xFF);

    byte_t res = (word_t)(d & (~s) & 0xFF);
    if (opcode.dd.addr < 8) {
        pdp_reg_set_var(pdp, opcode.dd.addr, (word_t) res);
    } else {
        b_write(pdp, opcode.dd.addr, (byte_t) res);
    }

    set_flag_NZ((word_t) res);
    SET_PSW_BIT(psw, V, 0);
}
//##########################################################################
