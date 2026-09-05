//##########################################################################
// ASL, ASLB
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_asl(struct pdp_11_t *pdp,
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

    word_t dst = (word_t)(opcode.dd.value);
    word_t c   = (word_t)((dst >> 15) & 1);

    word_t res = (word_t)((dst << 1) & 0xFFFF);

    w_write(pdp, opcode.dd.addr, res);
    pdp_reg_set_var(pdp, opcode.dd.addr, res);

    word_t n = (word_t)((res >> 15) & 1);
    word_t v = n ^ c;

    set_flag_C(c);
    set_flag_V(v);
    set_flag_NZ(res);
}

// ----------------------------------------------------------------------------
void
command_do_aslb(struct pdp_11_t *pdp,
                address_word_t   addr,
                word_t           word_command,
                byte_t           params)
{
    (void) addr;
    OP_CODE_T_INIT
    if (pdp) {
        opcode = __get_mr(pdp, word_command, params);
    }

    byte_t dst = (byte_t)(opcode.dd.value & 0xFF);
    word_t c   = (word_t)((dst >> 7) & 1);

    byte_t res = (byte_t)((dst << 1) & 0xFF);

    b_write(pdp, opcode.dd.addr, res);
    pdp_reg_set_var(pdp, opcode.dd.addr, res);

    word_t n = (word_t)((res >> 7) & 1);
    word_t v = n ^ c;

    set_flag_C(c);
    set_flag_V(v);
    set_flag_NZ(res);
}
//##########################################################################
