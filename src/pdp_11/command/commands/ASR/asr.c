//##########################################################################
// ASR, ASRB
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_asr(struct pdp_11_t *pdp,
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

    word_t c = dst & 1;

    word_t res = (word_t)((word_16_t) dst >> 1);

    w_write(pdp, opcode.dd.addr, res);
    pdp_reg_set_var(pdp, opcode.dd.addr, res);

    word_t n = (res >> 15) & 1; // Старший знаковый бит байта
    word_t v = n ^ c;
    word_t z = (res == 0);

    SET_PSW_BIT(psw, C, (word_t) c);
    SET_PSW_BIT(psw, V, (word_t) v);
    SET_PSW_BIT(psw, N, (word_t) n);
    SET_PSW_BIT(psw, Z, (word_t) z);
}
//----------------------------------------------------------------------------
void
command_do_asrb(struct pdp_11_t *pdp,
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

    word_t c = dst & 1;

    byte_t res = (byte_t)((byte8_t) dst >> 1);
    b_write(pdp, opcode.dd.addr, res);
    pdp_reg_set_var(pdp, opcode.dd.addr, res);

    word_t n = (res >> 7) & 1;
    word_t v = n ^ c;
    word_t z = (res == 0);

    SET_PSW_BIT(psw, C, (word_t) c);
    SET_PSW_BIT(psw, V, (word_t) v);
    SET_PSW_BIT(psw, N, (word_t) n);
    SET_PSW_BIT(psw, Z, (word_t) z);
}
//##########################################################################
