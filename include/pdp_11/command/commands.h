#ifndef COMMANDS_H
#    define COMMANDS_H
#    include "types/types.h"

#    define MAKRO_COMMAND_PROTOTYPE(NAME_COMMANDS)                            \
        void command_do_##NAME_COMMANDS(                                      \
            struct pdp_11_t *, address_word_t, word_t, byte_t)

struct pdp_11_t;

void __command_reg_dump(struct pdp_11_t *pdp);

// void command_do_halt(struct pdp_11_t *, address_word_t, word_t, byte_t);

void set_flag_C(word_t);
void set_flag_NZ(word_t);
void set_flag_V(word_t);

MAKRO_COMMAND_PROTOTYPE(halt);
MAKRO_COMMAND_PROTOTYPE(add);
MAKRO_COMMAND_PROTOTYPE(mov);
MAKRO_COMMAND_PROTOTYPE(movb);
MAKRO_COMMAND_PROTOTYPE(inc);
MAKRO_COMMAND_PROTOTYPE(sob);

MAKRO_COMMAND_PROTOTYPE(clr);
MAKRO_COMMAND_PROTOTYPE(unknown);
MAKRO_COMMAND_PROTOTYPE(adcb);

MAKRO_COMMAND_PROTOTYPE(br);
MAKRO_COMMAND_PROTOTYPE(beq);

#endif // COMMANDS_H
#pragma once
