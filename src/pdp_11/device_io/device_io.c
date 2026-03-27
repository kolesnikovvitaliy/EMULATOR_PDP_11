#include <stdio.h>
#include <stdlib.h>

#include "pdp_11/device_io/device_io_p.h"
#include "pdp_11/device_io/terminal_io/dev_terminal_io_p.h"
#include "pdp_11/device_io/files_io/dev_files_io_p.h"
#include "pdp_11/pdp_11.h"

byte_t g_default_device;

dev_io_t* dev_io_new()
{
        return (dev_io_t*)malloc(sizeof(dev_io_t)); // Выделение паамяти для обЪекта dev_io_t
}

void dev_io_create(dev_io_t* device_io)
{
        device_io->default_device = g_default_device;
}

void dev_io_destroy(dev_io_t* device_io)
{
        device_io->default_device = 0;
        device_io->load_data = load_data_term;
        return;

}

void dev_io_load_data(struct pdp_11_t* pdp,dev_io_t* device_io, byte_t *filename)
{
        if (filename) {
                device_io->load_data = load_data_file;
        } else {
                device_io->load_data = load_data_term;
        }
        device_io->load_data((struct pdp_11_t*)pdp, (byte_t*)filename);

}
void dev_io_mem_dump(struct pdp_11_t* pdp, address_word_t addr, word_t size)
{
    for(word_t ind= 0x0000; ind < size; ind++) {
                word_t ch = w_read(pdp, addr + ind);
                fprintf(stdout,
                        "%06o: %06o %04hx\r\n",
                        (addr | ind++), ch,  ch);
        }
    fprintf(stdout, "\r\n\n");
}

