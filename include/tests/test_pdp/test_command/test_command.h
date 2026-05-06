#ifndef TEST_COMMAND
#    define TEST_COMMAND
#    include "pdp_11/command/command_p.h"
#    include "pdp_11/pdp_11_p.h"
#    include "tests/test_pdp/test_command/halt/test_halt.h"
#    include "tests/test_pdp/test_command/mov/test_mov.h"
struct pdp_11_t;
int        test_command(struct pdp_11_t *);
command_t *__ptr_command(pdp_11_t *, command_t **, const address_word_t);
#endif
#pragma once
