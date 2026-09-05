//##########################################################################
// ROR, RORB
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_ror(struct pdp_11_t *pdp,
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

    uword_16_t dst   = opcode.dd.value;
    word_t     old_c = get_flag(C);

    uword_16_t res = (uword_16_t)((dst >> 1) | ((old_c & 1) << 15));

    if (opcode.dd.addr < 8) {
        pdp_reg_set_var(pdp, opcode.dd.addr, res);
    } else {
        w_write(pdp, opcode.dd.addr, res);
    }

    word_t c = dst & 1;
    word_t n = (res >> 15) & 1;
    word_t v = n ^ c;

    SET_PSW_BIT(psw, C, c);
    SET_PSW_BIT(psw, V, v);
    SET_PSW_BIT(psw, N, n);
    SET_PSW_BIT(psw, Z, (res == 0));
}
//-------------------------------------------------------------------------
void
command_do_rorb(struct pdp_11_t *pdp,
                address_word_t   addr,
                word_t           word_command,
                byte_t           params)
{
    (void) addr;
    OP_CODE_T_INIT
    if (pdp) {
        opcode = __get_mr(pdp, word_command, params);
    }

    byte_t dst   = (byte_t) opcode.dd.value;
    word_t old_c = get_flag(C);

    byte_t res = (byte_t)((dst >> 1) | ((old_c & 1) << 7));

    if (opcode.dd.addr < 8) {
        pdp_reg_set_var(pdp, opcode.dd.addr, (word_t) res);
    } else {
        b_write(pdp, opcode.dd.addr, (byte_t) res);
    }

    word_t c = dst & 1;
    word_t n = (res >> 7) & 1;
    word_t v = n ^ c;

    SET_PSW_BIT(psw, C, c);
    SET_PSW_BIT(psw, V, v);
    SET_PSW_BIT(psw, N, n);
    SET_PSW_BIT(psw, Z, (res == 0));
}
//##########################################################################
