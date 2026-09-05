//##########################################################################
// ADD
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_add(struct pdp_11_t *pdp,
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
    word_t src = opcode.ss.value;
    word_t dst = opcode.dd.value;
    word_t res = (word_t)(src + dst);

    w_write(pdp, opcode.dd.addr, (word_t)(res));
    pdp_reg_set_var(pdp, opcode.dd.addr, (word_t)(res));

    word_t v = (word_t)(((src ^ res) & (dst ^ res)) >> 15);

    set_flag_V((word_t) v);
    set_flag_C(res);
    set_flag_NZ(res);
}

//#########################################################################
