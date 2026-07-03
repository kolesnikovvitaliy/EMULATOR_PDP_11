#include "pdp_11/command/commands_list.h"

#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "utils/logger/logger.h"

#include <stdio.h>
#include <stdlib.h>

command_t template_commands[] = {
    { 0000000, 0111111, (byte_t *) "unknown", command_do_unknown, NO_PARAMS },
    { 0177777, 0000000, (byte_t *) "halt", command_do_halt, NO_PARAMS },
    { 0170000, 0010000, (byte_t *) "mov", command_do_mov, HAS_SS | HAS_DD },
    { 0170000, 0060000, (byte_t *) "add", command_do_add, HAS_SS | HAS_DD },
    { 0177700, 0005200, (byte_t *) "inc", command_do_inc, HAS_DD },
    { 0177000, 0110000, (byte_t *) "movb", command_do_movb, HAS_SS | HAS_DD },
    { 0177000, 0077000, (byte_t *) "sob", command_do_sob, HAS_R | HAS_NN },
    { 0177700, 0005000, (byte_t *) "clr", command_do_clr, HAS_DD },

    // добавить остальные команды
};

/* размер списка комманд */

byte_t commands_list = sizeof(template_commands) / sizeof(command_t);
