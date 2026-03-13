#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "pdp_11/pdp_11_p.h"
#include "pdp_11/memory/mem.h"
#include "pdp_11/device_io/device_io.h"
#include "utils/utils.h"

//////////////////////////////////////////////////////
pdp_11_t* pdp_new()
{
        return (pdp_11_t*)malloc(sizeof(pdp_11_t));
}
//------------------------------------------------------------------;
void pdp_create(pdp_11_t* pdp)
{
        pdp->memory = (struct mem_t*)mem_new(); // Создание обЬектов  памяти типа byte и word
        pdp->device_io = (struct dev_io_t*)dev_io_new(); // Создание обЬекта ввода вывода
        assert(pdp->memory);
        assert(pdp->device_io);
        mem_create(pdp->memory); // Инициализация памяти
        dev_io_create(pdp->device_io); // Инициализация ввода вывода
}
//------------------------------------------------------------------;
void pdp_destroy(pdp_11_t* pdp)
{
        mem_destroy((struct mem_t*)pdp->memory);
        free(pdp->memory);
        dev_io_destroy((struct dev_io_t*)pdp->device_io);
        free(pdp->device_io);
}
//////////////////////////////////////////////////////

//------------------------------------------------------------------;
//------------------------------------------------------------------;
void b_write(pdp_11_t* pdp, address_byte_t addr, byte_t data)
{
        assert(__is_valid_address(addr));
        byte_write((struct mem_t*)pdp->memory, addr, data);
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
byte_t b_read(pdp_11_t* pdp, address_byte_t addr)
{
        assert(__is_valid_address(addr));
        return (byte_t)byte_read((struct mem_t*)pdp->memory, addr);
}

//------------------------------------------------------------------;
//------------------------------------------------------------------;
void w_write(pdp_11_t* pdp,
                address_word_t addr, word_t data)
{
        assert(__is_valid_address(addr));
        word_write((struct mem_t*)pdp->memory, addr, data);
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
word_t w_read(pdp_11_t* pdp, address_word_t addr)
{
        assert(__is_valid_address(addr));
        return (word_t)word_read((struct mem_t*)pdp->memory, addr);
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
void pdp_load_data(pdp_11_t* pdp, byte_t* filename) {
        dev_io_load_data((struct pdp_11_t*)pdp,
                        (struct dev_io_t*)pdp->device_io,
                        (byte_t*)filename
                        );
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
void pdp_mem_dump(pdp_11_t* pdp, address_word_t addr, word_t size) {
        dev_io_mem_dump((struct pdp_11_t*)pdp, addr, size);
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
byte_t*
pdp_parse_filename(int argc, char **argv) {
        if (1 == argc) {
                usage((byte_t*)argv[0]);
                return (byte_t*)0;
        }
        if (3 >= argc) {
                for(int i = 1; i < argc; i++) {
                        if (strcmp(argv[i], "-t")) {
                                return (byte_t*)argv[i];
                        }
                }
        }
        usage((byte_t*)argv[0]);
        assert(!argv[0]);
        exit(1);
}
//------------------------------------------------------------------;
