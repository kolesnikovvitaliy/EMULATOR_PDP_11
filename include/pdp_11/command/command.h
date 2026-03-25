#ifndef COMMAND_H
#define COMMAND_H

struct command_t;


struct command_t* command_new();

void command_create(struct command_t*);
void command_destroy(struct command_t*);

void command_do_halt();
void command_do_add();
void command_do_mov();
void command_do_nothing();
#endif
#pragma once
