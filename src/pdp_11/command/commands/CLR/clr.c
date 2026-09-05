// //#######################################################################
// CLR
//##########################################################################
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/makro_commands.h"
#include "pdp_11/pdp_11.h"
#include "types/types.h"

extern word_t psw; // Переменная флагов состояния (NZVC)

extern byte_t set_has_b; // Проверка, команда типа byte или word

//##########################################################################
void
command_do_clr(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;

    OP_CODE_T_INIT
    set_has_b = 0;
    if (!pdp) {
        return;
    }
    opcode = __get_mr(pdp, word_command, params);

    w_write(pdp, opcode.dd.addr, (word_t)(opcode.ss.value + opcode.dd.value));
    SET_PSW_BIT(psw, N, (word_t) ZERO);
    SET_PSW_BIT(psw, Z, (word_t) ONE);
    SET_PSW_BIT(psw, V, (word_t) ZERO);
    SET_PSW_BIT(psw, C, (word_t) ZERO);
}
//##########################################################################
