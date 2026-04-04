#ifndef DEVICE_IO_H_P
#    define DEVICE_IO_H_P
#    include "types/types.h"
struct pdp_11_t;

typedef void (*load_data_t)(struct pdp_11_t *, byte_t *);
// typedef void (*load_term_t)(struct pdp_11_t*);

typedef struct {
    byte_t      default_device;
    load_data_t load_data;
    // load_files_t load_data_files;
    // load_term_t load_data_terminal;
} dev_io_t;

#endif
#pragma once
