//##########################################################################
// ADS
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_adc(struct pdp_11_t *pdp,
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

    word_t dst   = opcode.dd.value;
    word_t carry = get_flag(C);
    word_t res   = (word_t)(dst + carry);

    w_write(pdp, opcode.dd.addr, res);
    pdp_reg_set_var(pdp, opcode.dd.addr, res);

    word_t v = (dst == 0077777 && carry == 1) ? 1 : 0;
    word_t c = (dst == 0177777 && carry == 1) ? 1 : 0;

    set_flag_V(v);
    set_flag_C(c);
    set_flag_NZ(res);
}
//#########################################################################
