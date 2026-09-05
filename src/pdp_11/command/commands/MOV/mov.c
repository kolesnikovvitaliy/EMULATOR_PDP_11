//##########################################################################
// MOV, MOVB
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################

void
command_do_mov(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{

    (void) addr;

    if (!pdp) {
        return;
    }
    OP_CODE_T_INIT
    set_has_b  = 0;
    opcode     = __get_mr(pdp, word_command, params);
    word_t val = (word_t)(opcode.ss.value & 0xFFFF);

    if (opcode.dd.addr <= 7) {
        pdp_reg_set_var(pdp, opcode.dd.addr, (word_t) opcode.ss.value);
    } else {
        w_write(pdp, opcode.dd.addr, (word_t) opcode.ss.value);
    }
    SET_PSW_BIT(psw, V, 0);
    SET_PSW_BIT(psw, Z, val == 0);
    SET_PSW_BIT(psw, N, (val >> 15) & 1);
}
//---------------------------------------------------------------------------------
void
command_do_movb(struct pdp_11_t *pdp,
                address_word_t   addr,
                word_t           word_command,
                byte_t           params)
{

    (void) addr;

    if (!pdp) {
        return;
    }
    OP_CODE_T_INIT
    opcode = __get_mr(pdp, word_command, params);

    if ((word_t) opcode.dd.addr == 0177566) {
        putchar(opcode.ss.value);
    }
    byte_t byte_val = (byte_t)(opcode.ss.value & 0xFF);
    if (opcode.dd.addr <= 7) {
        word_16_t sign_extended
            = (word_16_t)((byte_val & 0x80) ? (0xFF00 | byte_val) : byte_val);
        pdp_reg_set_var(pdp, opcode.dd.addr, (word_t) sign_extended);
    } else {
        b_write(pdp, opcode.dd.addr, (byte_t)((word_t) byte_val));
    }
    SET_PSW_BIT(psw, V, 0);
    SET_PSW_BIT(psw, Z, byte_val == 0);
    SET_PSW_BIT(psw, N, (byte_val >> 7) & 1);
}
//#########################################################################
