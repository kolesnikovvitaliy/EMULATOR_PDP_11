#ifndef TYPES_FUNC_P
#define TYPES_FUNC_P


typedef byte_t (*read_byte_t)(void*, address_byte_t);
typedef void (*write_byte_t)(void*, address_byte_t, byte_t);
typedef word_t (*read_word_t)(void*, address_word_t);
typedef void (*write_word_t)(void*, address_word_t, word_t);

#endif
