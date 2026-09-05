//##########################################################################
// MUL
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_mul(struct pdp_11_t *pdp,
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

    word_16_t src     = (word_16_t) opcode.dd.value;
    word_16_t reg_val = (word_16_t) pdp_reg_get_var(pdp, opcode.r_reg);

    word_32_t res = (word_32_t) src * (word_32_t) reg_val;

    word_t high = (word_t)((res >> 16) & 0xFFFF);
    word_t low  = (word_t)(res & 0xFFFF);

    if ((opcode.r_reg % 2) == 0) {
        pdp_reg_set_var(pdp, opcode.r_reg, high);
        pdp_reg_set_var(pdp, (opcode.r_reg | 1), low);
    } else {
        pdp_reg_set_var(pdp, opcode.r_reg, low);
    }

    SET_PSW_BIT(psw, C, ((res < -32768 || res > 32767) ? 1 : 0));

    SET_PSW_BIT(psw, V, 0);

    SET_PSW_BIT(psw, N, ((res >> 31) & 1));
    SET_PSW_BIT(psw, Z, (res == 0));

    PRINT_RESULT("R%d", opcode.r_reg);
}
//#########################################################################
