#ifndef COMMANDS_H
#    define COMMANDS_H
#    include "types/types.h"

struct pdp_11_t;

void __command_reg_dump(struct pdp_11_t *pdp);

void command_do_halt(struct pdp_11_t *, address_word_t, word_t, byte_t);
void command_do_add(struct pdp_11_t *, address_word_t, word_t, byte_t);
void command_do_mov(struct pdp_11_t *, address_word_t, word_t, byte_t);
void command_do_inc(struct pdp_11_t *, address_word_t, word_t, byte_t);
void command_do_unknown(struct pdp_11_t *, address_word_t, word_t, byte_t);

#endif
#pragma once
