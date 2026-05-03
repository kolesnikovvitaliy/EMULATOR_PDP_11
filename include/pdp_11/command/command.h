#ifndef COMMAND_H
#    define COMMAND_H
#    include "types/types.h"
// struct command_t;
struct pdp_11_t;

struct command_t *command_new();

void command_create(struct command_t **);
void command_destroy(struct command_t **);

void print_command(address_word_t, word_t, byte_t *);

#endif
#pragma once
