//##########################################################################
// SOB
//##########################################################################
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/makro_commands.h"
#include "pdp_11/pdp_11.h"
#include "types/types.h"

extern word_t psw; // Переменная флагов состояния (NZVC)

extern byte_t set_has_b; // Проверка, команда типа byte или word

//#########################################################################

void
command_do_sob(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;

    OP_CODE_T_INIT
    set_has_b = 0;
    opcode    = __get_mr(pdp, word_command, params);

    word_t offset_word  = (word_t) opcode.value_nn;
    word_t num_register = (word_t) opcode.r_reg;

    word_t count_cycles = pdp_reg_get_var(pdp, num_register);

    pdp_reg_set_var(pdp, num_register, --count_cycles);
    count_cycles = pdp_reg_get_var(pdp, num_register);

    if (count_cycles != 0) {
        word_t PC = pdp_reg_get_var(pdp, 7);
        pdp_reg_set_var(pdp, 7, (word_t)((PC - (2 * offset_word))));
        PC = pdp_reg_get_var(pdp, 7);

        PRINT_RESULT("R%d %06o", num_register, (word_t)(PC + 2));
    } else {
        PRINT_RESULT(
            "R%d %06o",
            num_register,
            (word_t)((pdp_reg_get_var(pdp, 7)) - (2 * offset_word) + 2));
    }
}
//##########################################################################
