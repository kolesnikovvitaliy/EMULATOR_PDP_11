#ifndef TEST_MOV_H
#    define TEST_MOV_H
#    include "types/types.h"
struct pdp_11_t;
int  test_mov(struct pdp_11_t *, const address_word_t);
void test_mode1_toreg(struct pdp_11_t *, const address_word_t);
void test_mode1_reg_to_mem(struct pdp_11_t *, const address_word_t);
void test_mode1_mem_to_mem(struct pdp_11_t *, const address_word_t);
void test_mode2(struct pdp_11_t *, const address_word_t);
#endif
#pragma once
