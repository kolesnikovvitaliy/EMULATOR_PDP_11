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
    unsigned char size = 0x0;

    if (addr <= *pdp->PC && addr >= *pdp->R0) {
        return 1;
    }
    if ((addr > ~(-__get_size_buffer())) || (addr < size)) {
        return 0; // замена (__get_size_buffer() - 1)  на ~(-);
    }
    return 1;
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
void
usage(const byte_t *progname)
{
    fprintf(stderr, "USAGE: %s [-t] filename - input data\n", progname);
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
file_t *
pdp_file_open(const byte_t *filename, const byte_t *mode)
{
    file_t *fd;
    DEBUG("\nFILENAME = %s\n", filename);
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
