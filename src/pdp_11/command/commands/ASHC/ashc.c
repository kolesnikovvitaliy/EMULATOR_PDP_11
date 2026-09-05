//##########################################################################
// ASHC
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_ashc(struct pdp_11_t *pdp,
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

    word_32_t r1 = opcode.r_reg;
    word_32_t r2 = (r1 % 2 != 0) ? r1 : (r1 + 1);

    word_t reg1_val = (word_t)(pdp_reg_get_var(pdp, r1) & 0xFFFF);
    word_t reg2_val = (word_t)(pdp_reg_get_var(pdp, r2) & 0xFFFF);

    uword_32_t res32 = ((uword_32_t) reg1_val << 16) | (uword_32_t) reg2_val;
    uword_32_t orig_res32 = res32;

    short shift_count = (byte_t)(opcode.dd.value & 63);
    if (shift_count & 32)
        shift_count |= ~63;

    word_t    c            = 0;
    word_32_t signed_res32 = (word_32_t) res32;

    if (shift_count > 0) {
        if (shift_count >= 32) {
            c     = (word_t)(res32 & 1);
            res32 = 0;
        } else {
            c     = (word_t)((res32 >> (32 - shift_count)) & 1);
            res32 = res32 << shift_count;
        }
    } else if (shift_count < 0) {
        word_32_t count = -shift_count;
        if (count >= 32) {
            c     = (word_t)((signed_res32 >> 31) & 1);
            res32 = (signed_res32 < 0) ? 0xFFFFFFFF : 0;
        } else {
            c     = (word_t)((signed_res32 >> (count - 1)) & 1);
            res32 = (uword_32_t)(signed_res32 >> count);
        }
    }

    word_t res_r1 = (word_t)((res32 >> 16) & 0xFFFF);
    word_t res_r2 = (word_t)(res32 & 0xFFFF);

    if (opcode.r_reg % 2 != 0) {
        pdp_reg_set_var(pdp, r1, (word_32_t) res_r2);
    } else {
        pdp_reg_set_var(pdp, r1, (word_32_t) res_r1);
        pdp_reg_set_var(pdp, r2, (word_32_t) res_r2);
    }

    word_t v = 0;
    if (shift_count > 0) {
        v = ((orig_res32 >> 31) & 1) ^ ((res32 >> 31) & 1);
    }
    SET_PSW_BIT(psw, V, v);

    if (opcode.r_reg % 2 != 0) {
        SET_PSW_BIT(psw, C, 0);
        SET_PSW_BIT(psw, N, (word_t)((res_r2 >> 15) & 1));
        SET_PSW_BIT(psw, Z, (word_t)(res_r2 == 0));
    } else {
        if (shift_count != 0) {
            SET_PSW_BIT(psw, C, (word_t)(c & 1));
        } else {
            SET_PSW_BIT(psw, C, 0);
        }
        SET_PSW_BIT(psw, N, (word_t)((res32 >> 31) & 1));
        SET_PSW_BIT(psw, Z, (word_t)(res32 == 0));
    }

    PRINT_RESULT("R%d", opcode.r_reg);
}
// ##########################################################################
