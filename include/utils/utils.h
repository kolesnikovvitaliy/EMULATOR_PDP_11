#ifndef UTILS_H
#    define UTILS_H
#    include "pdp_11/pdp_11_p.h"
#    include "types/types.h"

int  __is_valid_address(pdp_11_t *pdp, const address_byte_t); //проверка адресса
void usage(const byte_t *progname); // сообщение об неоходимости использывать
                                    // перфикс [-t];
address_byte_t
        __get_size_buffer(); //получение дефолтного значения буфера при инициализации;
file_t *pdp_file_open(const byte_t *, const byte_t *);
void    pdp_file_close(const file_t *);
void    usage(const byte_t *progname);
#endif
#pragma once
