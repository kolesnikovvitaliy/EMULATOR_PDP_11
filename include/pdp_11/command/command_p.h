#ifndef COMMAND_P_H
#    define COMMAND_P_H
#    include "types/types.h"

#    define NO_PARAMS 0
#    define HAS_DD    (1 << 0) // 1 DD Поле приемника (Destination field) 6 бит
#    define HAS_SS    (1 << 1) // 2 SS Поле источника (Source field) 6 бит
#    define HAS_NN    (1 << 2) // 4 NN Числовое значение 6 бит
#    define HAS_XX                                                            \
        (1 << 3) // 8 XX Смещение (Адрес относительного перехода от -128 до
                 // +127) 8 бит
#    define HAS_R (1 << 4) // 16 R Индекс регистра (R0-R5, SP, PC) 3 бита
#    define HAS_N (1 << 5) // 32 N Числовое значение 3 бита
#    define HAS_B                                                             \
        (1 << 6) // 64 B Признак разрядности: 0 — слово (word), 1 — байт
                 // (byte) 1 бит
//#    define HAS_TT (1 << 8) // 128 TT Числовое значение 8 бит
// NZVC
#    define N    3
#    define Z    2
#    define V    1
#    define C    0
#    define ONE  1
#    define ZERO 0

#    define SET_PSW_BIT(                                                      \
        process_state_word, bit /* NZVC */, value /* 1 | 0 */)                \
        (process_state_word |= (word_t)(value << bit))

#    define OP_CODE_T_INIT op_code_t opcode = { 0 };

struct pdp_11_t;
typedef struct {
    word_t  mask;
    word_t  opcode;
    byte_t *name;
    void (*do_commands_command)(struct pdp_11_t *,
                                address_word_t,
                                word_t,
                                byte_t);
    byte_t params;
} command_t;

typedef struct {
    word_t value;
    word_t addr;
} arg_t;

typedef struct {
    arg_t  ss; // SS Поле источника (Source field) 6 бит
    arg_t  dd; // DD Поле приемника (Destination field) 6 бит
    byte_t r_reg; // R Индекс регистра (R0-R5, SP, PC) 3 бита
    byte_t value_nn;  // NN Числовое значение 6 бит
    byte_t value_n;   // N Числовое значение 3 бита
    byte_t offset_xx; // XX Смещение (Адрес относительного перехода от -128 до
                      // +127) 8 бит
    byte_t value_b; // B Признак разрядности: 0 — слово (word), 1 — байт (byte)
                    // 1 бит
    byte_t value_tt; // TT Числовое значение 8 бит

} op_code_t;

op_code_t __get_mr(struct pdp_11_t *pdp, word_t word_command, byte_t param);
arg_t     __get_args(struct pdp_11_t *pdp, word_t word_command);
void      __print_command(address_word_t addr,
                          word_t         word_command,
                          byte_t *       name_command);
#endif
#pragma once
