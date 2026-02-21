#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "pdp_11/pdp_11_p.h"
#include "memory/mem.h"

pdp_11_t* pdp_new()
{
        return (pdp_11_t*)malloc(sizeof(pdp_11_t));
}

void pdp_create(pdp_11_t* pdp)
{
        pdp->memory = (struct mem_t*)mem_new();
        assert(pdp->memory);
        mem_create(pdp->memory);
}

void pdp_destroy(pdp_11_t* pdp)
{
        mem_destroy((struct mem_t*)pdp->memory);
        free(pdp->memory);
}

int __is_valid_address(const address_byte_t addr)
{
        unsigned char size = 0x0;
        if((addr > (__get_size_buffer() - 1)) ||
                        (addr < size)) {
               return 0;
       }
        return 1;
}

void b_write(pdp_11_t* pdp, address_byte_t addr, byte_t data)
{
        assert(__is_valid_address(addr));
        byte_write((struct mem_t*)pdp->memory, addr, data);
}

byte_t b_read(pdp_11_t* pdp, address_byte_t addr)
{
        assert(__is_valid_address(addr));
        return (byte_t)byte_read((struct mem_t*)pdp->memory, addr);
}

//------------------------------------------------------------------;
void w_write(pdp_11_t* pdp,
                address_word_t addr, word_t data)
{
        assert(__is_valid_address(addr));
        word_write((struct mem_t*)pdp->memory, addr, data);
}

word_t w_read(pdp_11_t* pdp, address_word_t addr)
{
        assert(__is_valid_address(addr));
        return (word_t)word_read((struct mem_t*)pdp->memory, addr);
}
file_t *pdp_file_open(const byte_t *filename, const byte_t *mode)
{
        file_t *fd;
        if((fd = fopen((char*)filename, (char*)mode)) == NULL) {
                perror((char*)filename);
                exit(errno);
        }
        return fd;
}

void pdp_file_close(const file_t *fd) {
        fclose((file_t*)fd);
}
void usage(const byte_t* progname) {
        fprintf(stderr, "USAGE: %s [-t] filename - input data\n", progname);
}

void pdp_load_data(pdp_11_t *pdp, const byte_t *filename) {
        file_t * fp = pdp_file_open((byte_t*)filename,(byte_t*)"rb");
        word_t addr, count_str, data;
        addr = count_str = data = 0x0;

        while(2 == fscanf(fp, "%hx%hx", &addr, &count_str)) {

                for(byte_t ind = 0x0; ind < count_str; ind++) {
                        fscanf(fp, "%hx", &data);
                        b_write(pdp, (addr+ind), data);
                }
        }
        pdp_file_close(fp);
}

void pdp_mem_dump(pdp_11_t *pdp, address_word_t addr, word_t size) {
        for(word_t ind= 0x0; ind < size; ind++) {
                word_t ch = w_read(pdp, addr + ind);
                fprintf(stdout,
                                "%06o: %06o %04hx\r\n",
                                (addr + ind++), ch,  ch);
        }
}


