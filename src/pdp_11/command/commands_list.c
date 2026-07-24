#include "pdp_11/command/commands_list.h"

#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "utils/logger/logger.h"

#include <stdio.h>
#include <stdlib.h>
// TODO: ADD commands
command_t template_commands[] = {
    { 0000000, 0111111, (byte_t *) "unknown", command_do_unknown, NO_PARAMS },
    { 0177777, 0000000, (byte_t *) "halt", command_do_halt, NO_PARAMS },
    { 0170000, 0010000, (byte_t *) "mov", command_do_mov, HAS_SS | HAS_DD },
    { 0170000, 0060000, (byte_t *) "add", command_do_add, HAS_SS | HAS_DD },
    { 0177700, 0105500, (byte_t *) "adcb", command_do_adcb, HAS_DD },
    { 0177700, 0005200, (byte_t *) "inc", command_do_inc, HAS_DD },
    { 0170000,
      0110000,
      (byte_t *) "movb",
      command_do_movb,
      HAS_B | HAS_SS | HAS_DD },
    { 0177000, 0077000, (byte_t *) "sob", command_do_sob, HAS_R | HAS_NN },
    { 0177700, 0005000, (byte_t *) "clr", command_do_clr, HAS_DD },

    // добавить остальные команды
};

/* размер списка комманд */

byte_t commands_list = sizeof(template_commands) / sizeof(command_t);

/*
    {0177700, 0005500,  "adc",      do_adc,     HAS_DD},
    {0177700, 0105500,  "adcb",     do_adc,     HAS_DD},
    {0170000, 0060000,  "add",      do_add,     HAS_SS | HAS_DD},
    {0177000, 0072000,  "ash",      do_ash,     HAS_RLEFT | HAS_DD},
    {0177000, 0073000,  "ashc",     do_ashc,    HAS_RLEFT | HAS_DD},
    {0177700, 0006300,  "asl",      do_asl,     HAS_DD},
    {0177700, 0106300,  "aslb",     do_asl,     HAS_DD},
    {0177700, 0006200,  "asr",      do_asr,     HAS_DD},
    {0177700, 0106200,  "asrb",     do_asr,     HAS_DD},
    {0177400, 0103000,  "bcc",      do_bcc,     HAS_XX},
    {0177400, 0103400,  "bcs",      do_bcs,     HAS_XX},
    {0177400, 0001400,  "beq",      do_beq,     HAS_XX},
    {0177400, 0002000,  "bge",      do_bge,     HAS_XX},
    {0177400, 0003000,  "bgt",      do_bgt,     HAS_XX},
    {0170000, 0040000,  "bic",      do_bic,     HAS_SS | HAS_DD},
    {0170000, 0140000,  "bicb",     do_bic,     HAS_SS | HAS_DD},
    {0177400, 0101000,  "bhi",      do_bhi,     HAS_XX},
    {0177400, 0003400,  "ble",      do_ble,     HAS_XX},
    {0177400, 0002400,  "blt",      do_blt,     HAS_XX},
    {0177400, 0101400,  "blos",     do_blos,    HAS_XX},
    {0177400, 0100400,  "bmi",      do_bmi,     HAS_XX},
    {0177400, 0001000,  "bne",      do_bne,     HAS_XX},
    {0177400, 0100000,  "bpl",      do_bpl,     HAS_XX},
    {0177400, 0000400,  "br",       do_br,      HAS_XX},
    {0177400, 0102000,  "bvc",      do_bvc,     HAS_XX},
    {0177400, 0102400,  "bvs",      do_bvs,     HAS_XX},
    {0177777, 0000257,  "ccc",      do_clr_fl,  NO_PARAMS},
    {0177777, 0000241,  "clc",      do_clr_fl,  NO_PARAMS},
    {0177777, 0000250,  "cln",      do_clr_fl,  NO_PARAMS},
    {0177700, 0005000,  "clr",      do_clr,     HAS_DD},
    {0177700, 0105000,  "clrb",     do_clr,     HAS_DD},
    {0177777, 0000242,  "clv",      do_clr_fl,  NO_PARAMS},
    {0177777, 0000244,  "clz",      do_clr_fl,  NO_PARAMS},
    {0170000, 0020000,  "cmp",      do_cmp,     HAS_SS | HAS_DD},
    {0170000, 0120000,  "cmpb",     do_cmp,     HAS_SS | HAS_DD},
    {0177700, 0005100,  "com",      do_com,     HAS_DD},
    {0177700, 0105100,  "comb",     do_com,     HAS_DD},
    {0177700, 0005300,  "dec",      do_dec,     HAS_DD},
    {0177700, 0105300,  "decb",     do_dec,     HAS_DD},
    {0177000, 0071000,  "div",      do_div,     HAS_RLEFT | HAS_DD},
    {0177777, 0000000,  "halt",     do_halt,    NO_PARAMS},
    {0177700, 0005200,  "inc",      do_inc,     HAS_DD},
    {0177700, 0105200,  "incb",     do_inc,     HAS_DD},
    {0177700, 0000100,  "jmp",      do_jmp,     HAS_DD},
    {0177000, 0004000,  "jsr",      do_jsr,     HAS_RLEFT | HAS_DD},
    {0170000, 0010000,  "mov",      do_mov,     HAS_SS | HAS_DD},
    {0170000, 0110000,  "movb",     do_mov,     HAS_SS | HAS_DD},
    {0177000, 0070000,  "mul",      do_mul,     HAS_RLEFT | HAS_DD},
    {0177700, 0005400,  "neg",      do_neg,     HAS_DD},
    {0177700, 0105400,  "negb",     do_neg,     HAS_DD},
    {0177777, 0000240,  "nop",      do_clr_fl,  NO_PARAMS},
    {0177777, 0000005,  "reset",    do_reset,   NO_PARAMS},
    {0177700, 0006100,  "rol",      do_rol,     HAS_DD},
    {0177700, 0106100,  "rolb",     do_rol,     HAS_DD},
    {0177700, 0006000,  "ror",      do_ror,     HAS_DD},
    {0177700, 0106000,  "rorb",     do_ror,     HAS_DD},
    {0177770, 0000200,  "rts",      do_rts,     HAS_RRIGHT},
    {0177700, 0005600,  "sbc",      do_sbc,     HAS_DD},
    {0177700, 0105600,  "sbcb",     do_sbc,     HAS_DD},
    {0177777, 0000277,  "scc",      do_set_fl,  NO_PARAMS},
    {0177777, 0000261,  "sec",      do_set_fl,  NO_PARAMS},
    {0177777, 0000270,  "sen",      do_set_fl,  NO_PARAMS},
    {0177777, 0000262,  "sev",      do_set_fl,  NO_PARAMS},
    {0177777, 0000264,  "sez",      do_set_fl,  NO_PARAMS},
    {0177000, 0077000,  "sob",      do_sob,     HAS_RLEFT | HAS_NN},
    {0170000, 0160000,  "sub",      do_sub,     HAS_SS | HAS_DD},
    {0177700, 0000300,  "swab",     do_swab,    HAS_DD},
    {0177700, 0006700,  "sxt",      do_sxt,     HAS_DD},
    {0177700, 0005700,  "tst",      do_tst,     HAS_DD},
    {0177700, 0105700,  "tstb",     do_tst,     HAS_DD},
    {0177000, 0074000,  "xor",      do_xor,     HAS_RLEFT | HAS_DD},
    {0000000, 0000000,  "unknown",  do_unknown, NO_PARAMS}
*/
