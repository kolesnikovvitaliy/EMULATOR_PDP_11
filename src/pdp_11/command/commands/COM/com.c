//##########################################################################
// COM, COMB
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_com(struct pdp_11_t *pdp,
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

    word_t dst = opcode.dd.value;
    word_t res = (word_t)(~dst & 0xFFFF);

    w_write(pdp, opcode.dd.addr, res);
    pdp_reg_set_var(pdp, opcode.dd.addr, res);

    set_flag_V(0);
    set_flag_C(1);
    set_flag_NZ(res);
}
//--------------------------------------------------------------------------
void
command_do_comb(struct pdp_11_t *pdp,
                address_word_t   addr,
                word_t           word_command,
                byte_t           params)
{
    (void) addr;
    OP_CODE_T_INIT
    if (pdp) {
        opcode = __get_mr(pdp, word_command, params);
    }

    byte_t dst = (byte_t) opcode.dd.value;
    byte_t res = (byte_t)(~dst & 0xFF);

    b_write(pdp, opcode.dd.addr, res);
    pdp_reg_set_var(pdp, opcode.dd.addr, res);

    set_flag_V(0);
    set_flag_C(1);
    set_flag_NZ(res);
}
//##########################################################################
