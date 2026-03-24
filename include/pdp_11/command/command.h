#ifndef COMMAND_H
#define COMMAND_H

struct command_t;


struct command_t* command_new();

void command_create(struct command_t*);
void command_destroy(struct command_t*);

void do_halt();
#endif
#pragma once
