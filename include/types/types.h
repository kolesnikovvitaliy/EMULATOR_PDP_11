#ifndef TYPES_H
#define TYPES_H

enum {size_buffer_default = 0xFFFF};
typedef unsigned char byte_t;
typedef unsigned short int word_t;

typedef int bool_t;

typedef unsigned int address_word_t;
typedef unsigned int address_byte_t;

typedef unsigned int size_byte_buffer;
typedef unsigned int size_word_buffer;
address_byte_t __get_size_buffer();


#endif
#pragma once
