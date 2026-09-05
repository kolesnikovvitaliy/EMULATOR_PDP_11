//##########################################################################
// RTS
//##########################################################################
#include "pdp_11/command/includ_for_commands.h"
//#########################################################################
void
command_do_rts(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;
    (void) params;
    (void) word_command;
    (void) pdp;

    if (!pdp) {
        return;
    }

    text_t name_registers[3] = { '\0' };

    word_t target_reg = (word_t)(word_command & 07);
    word_t target_sp  = pdp_reg_get_var(pdp, 6);

    pdp_reg_set_var(pdp, 7, (word_t)(pdp_reg_get_var(pdp, target_reg) - 2));
    if (target_reg != 7) {
        pdp_reg_set_var(
            pdp, target_reg, (word_t)(w_read(pdp, (word_t)(target_sp + 2))));

    } else {
        pdp_reg_set_var(pdp,
                        target_reg,
                        (word_t)(w_read(pdp, (word_t)(target_sp + 2)) - 2));
    }

    pdp_reg_set_var(pdp, 6, (word_t)(target_sp + 2));

    if (target_reg == 6) {
        snprintf(name_registers, sizeof(name_registers), "ps");
    }
    if (target_reg == 7) {
        snprintf(name_registers, sizeof(name_registers), "pc");
    }
    if (target_reg < 6) {
        snprintf(name_registers, sizeof(name_registers), "r%d", target_reg);
    }

    PRINT_RESULT(" %s ", name_registers);
}
//##########################################################################
