#ifndef DEVICE_IO_H
#    define DEVICE_IO_H

struct pdp_11_t;

struct dev_io_t;

struct dev_io_t *dev_io_new(void);

void dev_io_create(struct dev_io_t *);
void dev_io_destroy(struct dev_io_t *);

void dev_io_load_data(struct pdp_11_t *, struct dev_io_t *, byte_t *);

void dev_io_mem_dump(struct pdp_11_t *, address_word_t, word_t);

#endif
#pragma once
