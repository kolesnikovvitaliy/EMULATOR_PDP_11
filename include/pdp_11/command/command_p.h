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
    void (*do_commands_command)(struct pdp_11_t *,
                                address_word_t,
                                word_t,
                                byte_t);
    byte_t params;
} command_t;

typedef struct {
    word_t value;
    word_t addr;
} arg_t;

typedef struct {
    arg_t ss;
    arg_t dd;
} op_code_t;

op_code_t __get_mr(struct pdp_11_t *pdp, word_t word_command);
arg_t     __get_args(struct pdp_11_t *pdp, word_t word_command);
void      __print_command(address_word_t addr,
                          word_t         word_command,
                          byte_t *       name_command);
#endif
#pragma once
