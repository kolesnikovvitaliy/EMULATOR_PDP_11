//##########################################################################
// DIV
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_div(struct pdp_11_t *pdp,
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

    word_16_t src = (word_16_t) opcode.dd.value;
    if (src == 0) {
        set_flag_V(1);
        set_flag_C(1); // Деление на ноль
        return;
    }

    uword_16_t reg_num = (uword_16_t)(opcode.r_reg & ~1);

    uword_16_t high_word = (uword_16_t) pdp_reg_get_var(pdp, reg_num);
    uword_16_t low_word  = (uword_16_t) pdp_reg_get_var(pdp, reg_num | 1);

    word_32_t dividend
        = (word_32_t)(((uword_32_t) high_word << 16) | low_word);

    word_32_t quot = dividend / src;
    word_32_t rem  = dividend % src;

    if (quot < -32768 || quot > 32767) {
        set_flag_V(1);
        SET_PSW_BIT(psw, N, (word_t)(quot < 0 ? 1 : 0));
        return;
    }

    pdp_reg_set_var(pdp, reg_num, (word_t)(quot & 0xFFFF)); // Частное в R2
    pdp_reg_set_var(
        pdp, (reg_num | 1), (word_t)(rem & 0xFFFF)); // Остаток в R3

    SET_PSW_BIT(psw, V, 0);
    SET_PSW_BIT(psw, C, 0);
    set_flag_NZ((word_t)(quot & 0xFFFF));

    PRINT_RESULT("R%d", opcode.r_reg);
}
//##########################################################################
