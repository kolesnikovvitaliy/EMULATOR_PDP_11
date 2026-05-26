#ifndef TESTS_MODE_1
#    define TESTS_MODE_1
#    include "types/types.h"
struct pdp_11_t;
void test_mode1_toreg(struct pdp_11_t *, const address_word_t);
void test_mode1_reg_to_mem(struct pdp_11_t *, const address_word_t);
void test_mode1_mem_to_mem(struct pdp_11_t *, const address_word_t);
#endif
#pragma once
