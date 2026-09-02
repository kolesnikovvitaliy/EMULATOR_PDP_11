#ifndef TYPES_H
#    define TYPES_H
#    include <stdio.h>

enum { size_buffer_default = 0xffff };

typedef unsigned char      byte_t;
typedef char               text_t;
typedef unsigned short int word_t;

typedef char byte8_t;

typedef int                word_32_t;
typedef unsigned int       uword_32_t;
typedef short int          word_16_t;
typedef unsigned short int uword_16_t;

typedef int                bool_t;
typedef FILE               file_t;
typedef unsigned short int address_word_t;
typedef unsigned short int address_byte_t;
typedef unsigned short int size_byte_buffer;
typedef unsigned short int size_word_buffer;

#endif
#pragma once
