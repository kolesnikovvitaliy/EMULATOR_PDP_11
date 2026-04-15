#ifndef COMMAND_P_H
#    define COMMAND_P_H
#    include "types/types.h"

#    define NO_PARAMS 0
#    define HAS_DD    1
#    define HAS_SS    2

struct pdp_11_t;
typedef struct {
    word_t  mask;
    word_t  opcode;
    byte_t *name;
    void (*do_commands_command)(struct pdp_11_t *, address_word_t, word_t);
    byte_t params;
} command_t;

void command_do_halt(struct pdp_11_t *, address_word_t, word_t);
void command_do_add(struct pdp_11_t *, address_word_t, word_t);
void command_do_mov(struct pdp_11_t *, address_word_t, word_t);
void command_do_inc(struct pdp_11_t *, address_word_t, word_t);
void command_do_unknown(struct pdp_11_t *, address_word_t, word_t);

extern byte_t commands_list;
#endif
#pragma once
