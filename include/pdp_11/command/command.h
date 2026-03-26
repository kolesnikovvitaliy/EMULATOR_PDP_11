#ifndef COMMAND_H
#define COMMAND_H


//struct command_t;
struct pdp_11_t;

struct command_t* command_new();

void command_create(struct command_t**);
void command_destroy(struct command_t**);

void print_command(address_word_t, word_t, byte_t*);

//void command_do_halt(struct pdp_11_t, address_word_t, word_t);
//void command_do_add(struct pdp_11_t, address_word_t, word_t);
//void command_do_mov(struct pdp_11_t, address_word_t, word_t);
//void command_do_nothing(struct pdp_11_t, address_word_t, word_t);

#endif
#pragma once
