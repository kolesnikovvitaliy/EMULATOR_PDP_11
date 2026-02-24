#ifndef MEM_P_H
#define MEM_P_H
#include "types/types.h"
#include "pdp_11/pdp_11.h"

typedef void (*load_files_t)(struct pdp_11_t*, byte_t*);
typedef void (*load_term_t)(struct pdp_11_t*);

typedef struct {
        byte_t default_device;
        load_files_t load_data_files;
        load_term_t load_data_terminal;
} dev_io_t;


#endif
#pragma once

