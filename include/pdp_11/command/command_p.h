#ifndef COMMAND_P_H
#    define COMMAND_P_H
#    include "types/types.h"
struct pdp_11_t;
typedef struct {
    word_t  mask;
    word_t  opcode;
    byte_t *name;
    void (*do_commands_command)(struct pdp_11_t *, address_word_t, word_t);
} command_t;

void command_do_halt(struct pdp_11_t *, address_word_t, word_t);
void command_do_add(struct pdp_11_t *, address_word_t, word_t);
void command_do_mov(struct pdp_11_t *, address_word_t, word_t);
void command_do_inc(struct pdp_11_t *, address_word_t, word_t);
void command_do_unknown(struct pdp_11_t *, address_word_t, word_t);

extern byte_t commands_list;
#endif
#pragma once
