#include "utils/utils.h"

#include "pdp_11/pdp_11_p.h"
#include "types/types.h"
#include "utils/logger/logger.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------;
//------------------------------------------------------------------;
int
__is_valid_address(pdp_11_t *pdp, const address_byte_t addr)
{
    // Защита от передачи пустого указателя на контекст процессора
    if (pdp == NULL) {
        return 0;
    }

    // Получаем максимальный размер буфера памяти эмулятора
    // (например, 64 * 1024 для полных 64 КБ)
    unsigned int mem_size = __get_size_buffer();

    // Адрес валиден, если он строго меньше размера буфера памяти.
    if (addr < mem_size) {
        return 1;
    }
    // }
    return 0;
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
void
usage(const byte_t *progname)
{
    fprintf(stderr, "USAGE: %s [-f] filename - input data\n", progname);
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
file_t *
pdp_file_open(const byte_t *filename, const byte_t *mode)
{
    file_t *fd;
    DEBUG_LOG("FILENAME = %s", filename);
    if ((fd = fopen((char *) filename, (char *) mode)) == NULL) {
        perror((char *) filename);
        exit(errno);
    }
    return fd;
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
void
pdp_file_close(const file_t *fd)
{
    fclose((file_t *) fd);
}
//------------------------------------------------------------------;
