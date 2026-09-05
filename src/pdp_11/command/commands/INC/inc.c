// //#######################################################################
// INC
//##########################################################################
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/makro_commands.h"
#include "pdp_11/pdp_11.h"
#include "types/types.h"

extern word_t psw; // Переменная флагов состояния (NZVC)

extern byte_t set_has_b; // Проверка, команда типа byte или word

//##########################################################################
void
command_do_inc(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;
    if (!pdp) {
        return;
    }

    OP_CODE_T_INIT
    set_has_b = 0;
    opcode    = __get_mr(pdp, word_command, params);

    word_t dst = opcode.dd.value;
    word_t res = (word_t)((dst + 1) & 0xFFFF);

    if (opcode.dd.addr < 8) {
        pdp_reg_set_var(pdp, opcode.dd.addr, res);
    } else {
        w_write(pdp, opcode.dd.addr, res);
    }

    word_t n = (res >> 15) & 1;
    word_t z = (res == 0);

    word_t v = (dst == 077777);

    SET_PSW_BIT(psw, N, n);
    SET_PSW_BIT(psw, Z, z);
    SET_PSW_BIT(psw, V, v);
}
//##########################################################################
