#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands_list.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "utils/logger/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern word_t psw; // Переменная флагов состояния (NZVC)
extern word_t      tick; // Количество выполненных машинных команд;
extern log_level_t current_log_level; // Уровень логирования;
extern byte_t      set_has_b;

//##########################################################################
// TRACE DUMP INFO REGISTERS
//##########################################################################
void
__command_reg_dump(struct pdp_11_t *pdp)
{
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    reg_t *   ptr_reg = (reg_t *) ptr_pdp->regist;

    PRINT_RESULT("\nr0=%06o r2=%06o r4=%06o sp=%06o\nr1=%06o r3=%06o r5=%06o "
                 "pc=%06o\n\npsw=%06o: cm=k pm=k pri=0     ",
                 ptr_reg->R0,
                 ptr_reg->R2,
                 ptr_reg->R4,
                 ptr_reg->SP,
                 ptr_reg->R1,
                 ptr_reg->R3,
                 ptr_reg->R5,
                 ptr_reg->PC,
                 psw);
    PRINT_RESULT("%c%c%c%c",
                 ((psw >> 3) & 1) ? 'n' : ' ',
                 ((psw >> 2) & 1) ? 'z' : ' ',
                 ((psw >> 1) & 1) ? 'v' : ' ',
                 (psw & 1) ? 'c' : ' ');
    PRINT_RESULT(" [%d]", tick);

    PRINT_RESULT("\n", "");
    TRACE_LOG("-------------------------------------", "");

    /*---------------- halted ---------------
    r0=000133 r2=000105 r4=000000 sp=000000
    r1=000000 r3=000000 r5=000000 pc=001020
    psw=000004: cm=k pm=k pri=0    z   [21]*/
}
//##########################################################################

//##########################################################################
// GET_PSW_BIT(psw, FLAG(NZVC)) // Получение значения бита ФЛАГА (PSW)
//##########################################################################
byte_t
get_flag(word_t flag)
{
    return (byte_t) GET_PSW_BIT(psw, flag);
}
//##########################################################################
// SET_FLAGS_NZVC // Установка значения бита ФЛАГА (PSW)
//##########################################################################

void
set_flag_C(word_t value)
{
    word_t res_neg  = 0;
    word_t res      = 0;
    byte_t shift    = 16;
    word_t temp_val = value;
    res_neg         = (word_t)(signed char) temp_val;

    if ((word_t) HAS_B && (!res_neg)) {
        shift = 8;
    }

    if (GET_PSW_BIT(psw, N)) {
        temp_val = (word_t)(~temp_val);
        res      = (word_t)(((signed short) temp_val >> shift) & ONE);
    } else {
        res = (word_t)(((word_t) temp_val >> shift) & ONE);
    }

    SET_PSW_BIT(psw, C, (word_t) res);
    return;
}

//--------------------------------------------------------------------------

void
set_flag_NZ(word_t value)
{
    uword_16_t val16 = (uword_16_t)(value & 0xFFFF);

    // Флаг Z: равен 1, если все 16 бит равны 0
    word_t z = (val16 == 0) ? ONE : ZERO;
    SET_PSW_BIT(psw, Z, z);

    // Флаг N: равен старшему (15-му) биту 16-битного слова
    word_t n = (word_t)((val16 >> 15) & 1);
    SET_PSW_BIT(psw, N, n);

    return;
}

//--------------------------------------------------------------------------

void
set_flag_V(word_t value)
{

    SET_PSW_BIT(psw, V, (word_t) value);
}

//##########################################################################
