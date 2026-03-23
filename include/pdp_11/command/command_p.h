#ifndef COMMAND_P_H
#define COMMAND_P_H
#include "types/types.h"

typedef struct {
    word_t mask;
    word_t opcode;
    byte_t * name;
    void (*do_command)(void);
} command_t;

#endif
#pragma once
