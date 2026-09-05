// ##########################################################################
// ASH
// ##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_ash(struct pdp_11_t *pdp,
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

    word_t reg_val     = pdp_reg_get_var(pdp, opcode.r_reg);
    short  shift_count = (byte_t)(opcode.dd.value & 63);

    if (shift_count & 32)
        shift_count |= ~63;

    word_t res = reg_val;
    word_t c   = 0;

    if (shift_count > 0) {
        if (shift_count >= 16) {
            c   = reg_val & 1;
            res = 0;
        } else {
            c   = (word_t)((res >> (16 - shift_count)) & 1);
            res = (word_t)((res << shift_count) & 0xFFFF);
        }
    } else if (shift_count < 0) {
        int count = -shift_count;
        if (count > 16)
            count = 16;
        c = (word_t)((res >> (count - 1)) & 1);

        res = (word_t)(((short) res >> count) & 0xFFFF);
    }

    pdp_reg_set_var(pdp, opcode.r_reg, (int) res);

    word_t v = ((reg_val >> 15) & 1) ^ ((res >> 15) & 1);

    set_flag_V(v);

    if (shift_count != 0) {
        SET_PSW_BIT(psw, C, (word_t)(c & 1));
    } else {
        set_flag_C(0);
    }

    set_flag_NZ(res);
    PRINT_RESULT("R%d", opcode.r_reg);
}
// ##########################################################################
