#ifndef PDP_P_H
#define PDP_P_H
#include "types/types.h"
typedef struct {
       struct mem_t* memory;
       struct dev_io_t* device_io;
       struct register_t* regist;
       word_t *R0;
       word_t *R1;
       word_t *R2;
       word_t *R3;
       word_t *R4;
       word_t *R5;
       word_t *R6;
       word_t *R7;
} pdp_11_t;

#endif
#pragma once

