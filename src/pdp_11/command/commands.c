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

//##########################################################################
void
__command_reg_dump(struct pdp_11_t *pdp)
{
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    reg_t *   ptr_reg = (reg_t *) ptr_pdp->regist;

    PRINT_RESULT("\nr0=%06o r2=%06o r4=%06o sp=%06o\nr1=%06o r3=%06o r5=%06o "
                 "pc=%06o\n\npsw=%06o: cm=k pm=k pri=0     z  [%d]",
                 ptr_reg->R0,
                 ptr_reg->R2,
                 ptr_reg->R4,
                 ptr_reg->SP,
                 ptr_reg->R1,
                 ptr_reg->R3,
                 ptr_reg->R5,
                 ptr_reg->PC,
                 psw,
                 tick);
    PRINT_RESULT("\n", "");

    for (int i = 7; i >= 0; i--) {
        PRINT_RESULT("%o", (psw >> i) & 1);
    }

    /*---------------- halted ---------------
    r0=000133 r2=000105 r4=000000 sp=000000
    r1=000000 r3=000000 r5=000000 pc=001020
    psw=000004: cm=k pm=k pri=0    z   [21]*/
}
//##########################################################################
// GET_PSW_BIT(psw, FLAG(NZVC))
byte_t
get_flag(word_t flag)
{
    return (byte_t) GET_PSW_BIT(psw, flag);
}
//##########################################################################
// SET_FLAGS_NZVC
//##########################################################################

//-------------------------------------------------------------------------
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

//--------------------------------------------------------------------------
void
set_flag_NZ(word_t value)
{
    word_t res_neg  = 0;
    word_t res      = 0;
    byte_t shift    = 15;
    word_t temp_val = value;

    res_neg = (word_t)(signed char) temp_val;

    if ((word_t) HAS_B && (!res_neg)) {
        temp_val = (word_t)(temp_val & 0xFF);
        shift    = 7;
    }

    res = (temp_val == 0) ? ONE : ZERO;
    SET_PSW_BIT(psw, Z, (word_t) res);

    res = (word_t)((temp_val >> shift) & ONE);
    SET_PSW_BIT(psw, N, (word_t) res);

    return;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
set_flag_V(word_t value)
{

    SET_PSW_BIT(psw, V, (word_t) value);
}
//--------------------------------------------------------------------------

//##########################################################################
//
// EXEMPLE function
//##########################################################################
// void
// command_do_br(struct pdp_11_t *pdp,
//                 address_word_t   addr,
//                 word_t           word_command,
//                 byte_t           params)
// {
//     (void) addr;
//     (void) params;
//     (void) word_command;
//     (void) pdp;
// PRINT_RESULT("\nADDR_REG_3 = %o\n", pdp_reg_get_addr(pdp, 3));
// pdp_mem_dump(pdp, 0x40, 0x20);
// pdp_mem_dump(pdp, 0x200, 0x20);
//
// }
//##########################################################################

//##########################################################################
// COMMANDS
//##########################################################################
void
command_do_halt(struct pdp_11_t *pdp,
                address_word_t   addr,
                word_t           word_command,
                byte_t           params)
{
    (void) addr;
    (void) params;
    (void) word_command;
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    word_t *  ptr_pc  = ptr_pdp->PC;
    *ptr_pc           = (word_t)(*ptr_pc + 2);
    tick++;
    PRINT_RESULT("\n\n---------------- halted ---------------", "");
    __command_reg_dump(pdp);

    if (!current_log_level) {
        fprintf(stdout, "\n");
    }
    PRINT_RESULT("\n\nTHE END!!!\n", "");
    pdp_destroy(pdp);
    free(pdp);
    exit(0);
}
//##########################################################################

/**
@brief Выполняет операцию сложения (ADD) для процессора PDP-11.
Извлекает значения операндов источника (ss) и назначения (dd),
суммирует их и записывает результат по адресу назначения.
@param[in,out] pdp Указатель на структуру состояния процессора.
@param[in] addr Адрес текущей команды (не используется).
@param[in] word_command 16-битный код команды с параметрами адресации.
@note Параметр addr приводится к (void) для предотвращения
предупреждений компилятора.
*/
void
command_do_add(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{

    (void) addr;
    //(void) params;
    OP_CODE_T_INIT
    if (pdp) {
        opcode = __get_mr(pdp, word_command, params);
    }

    word_t src = opcode.ss.value;
    word_t dst = opcode.dd.value;
    word_t res = (word_t)(src + dst);

    w_write(pdp, opcode.dd.addr, (word_t)(res));
    pdp_reg_set_var(pdp, opcode.dd.addr, (word_t)(res));

    word_t v = (word_t)(((src ^ res) & (dst ^ res)) >> 15);

    set_flag_V((word_t) v);
    set_flag_C(res);
    set_flag_NZ(res);
}
//##########################################################################

//##########################################################################
void
command_do_mov(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{

    (void) addr;

    if (!pdp) {
        return;
    }
    OP_CODE_T_INIT
    opcode = __get_mr(pdp, word_command, params);

    if (opcode.dd.addr <= 7) {
        pdp_reg_set_var(pdp, opcode.dd.addr, opcode.ss.value);
    } else {
        w_write(pdp, opcode.dd.addr, opcode.ss.value);
    }
    set_flag_NZ(opcode.ss.value);
    set_flag_V(ZERO);
}
//##########################################################################
//##########################################################################
void
command_do_movb(struct pdp_11_t *pdp,
                address_word_t   addr,
                word_t           word_command,
                byte_t           params)
{

    (void) addr;

    if (!pdp) {
        return;
    }
    OP_CODE_T_INIT
    opcode = __get_mr(pdp, word_command, params);

    if ((word_t) opcode.dd.addr == 0177566) {
        putchar(opcode.ss.value);
    }

    if (opcode.dd.addr <= 7) {
        pdp_reg_set_var(pdp, opcode.dd.addr, (word_t) opcode.ss.value);
    } else {
        b_write(pdp, opcode.dd.addr, (byte_t)((word_t) opcode.ss.value));
    }
    set_flag_NZ(opcode.ss.value);
    set_flag_V(ZERO);
}
//##########################################################################

//##########################################################################
void
command_do_inc(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;

    OP_CODE_T_INIT
    if (!pdp) {
        return;
    }
    opcode = __get_mr(pdp, word_command, params);
    // w_write(pdp, opcode.dd.addr, opcode.ss.value);
    pdp_reg_set_var(pdp, opcode.dd.addr, opcode.ss.value);
}
//##########################################################################

//##########################################################################
void
command_do_clr(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;
    //(void) params;
    OP_CODE_T_INIT
    if (!pdp) {
        return;
    }
    opcode = __get_mr(pdp, word_command, params);

    w_write(pdp, opcode.dd.addr, (word_t)(opcode.ss.value + opcode.dd.value));
    SET_PSW_BIT(psw, N, (word_t) ZERO);
    SET_PSW_BIT(psw, Z, (word_t) ONE);
    SET_PSW_BIT(psw, V, (word_t) ZERO);
    SET_PSW_BIT(psw, C, (word_t) ZERO);
}
//##########################################################################

//##########################################################################
void
command_do_sob(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;
    //(void) params;
    OP_CODE_T_INIT
    opcode = __get_mr(pdp, word_command, params);
    // word_t offset_word  = (word_command & 077);
    word_t offset_word = (word_t) opcode.value_nn;

    // word_t num_register = (word_command >> 6) & 7;
    word_t num_register = (word_t) opcode.r_reg;

    word_t count_cycles = pdp_reg_get_var(pdp, num_register);

    pdp_reg_set_var(pdp, num_register, --count_cycles);
    count_cycles = pdp_reg_get_var(pdp, num_register);

    if (count_cycles != 0) {
        word_t PC = pdp_reg_get_var(pdp, 7);
        pdp_reg_set_var(pdp, 7, (word_t)((PC - (2 * offset_word))));
        PC = pdp_reg_get_var(pdp, 7);

        PRINT_RESULT("R%d %06o", num_register, (word_t)(PC + 2));
    } else {
        PRINT_RESULT(
            "R%d %06o",
            num_register,
            (word_t)((pdp_reg_get_var(pdp, 7)) - (2 * offset_word) + 2));
    }

    //__command_reg_dump(pdp);
    // sleep(5);
}
//##########################################################################

//##########################################################################
void
command_do_unknown(struct pdp_11_t *pdp,
                   address_word_t   addr,
                   word_t           word_command,
                   byte_t           params)
{
    (void) params;
    if (!pdp)
        return;
    __print_command(addr, word_command, (byte_t *) "unknown\n");
    // pdp_reg_set_var(pdp, addr, (word_t)(addr + 4));
}
//##########################################################################

//##########################################################################
void
command_do_adcb(struct pdp_11_t *pdp,
                address_word_t   addr,
                word_t           word_command,
                byte_t           params)
{
    (void) addr;
    //(void) params;
    OP_CODE_T_INIT
    if (!pdp) {
        return;
    }
    opcode = __get_mr(pdp, word_command, params);
    // test w_write TODO: Исправить
    w_write(pdp, opcode.dd.addr, opcode.ss.value);
    // pdp_reg_set_var(pdp, opcode.dd.addr, opcode.ss.value);
}
//##########################################################################

//##########################################################################
void
command_do_br(struct pdp_11_t *pdp,
              address_word_t   addr,
              word_t           word_command,
              byte_t           params)
{
    (void) addr;

    if (!pdp) {
        return;
    }
    OP_CODE_T_INIT
    opcode = __get_mr(pdp, word_command, params);

    word_t PC = pdp_reg_get_var(pdp, 7);
    pdp_reg_set_var(pdp, 7, (word_t)(PC + opcode.offset_xx * 2));
    PRINT_RESULT("%06o", (word_t)(pdp_reg_get_var(pdp, 7) + 2));
    // __command_reg_dump(pdp);
    return;
}
//##########################################################################
//##########################################################################
void
command_do_beq(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{

    if (!pdp) {
        return;
    }
    // __command_reg_dump(pdp);
    OP_CODE_T_INIT
    opcode = __get_mr(pdp, word_command, params);
    if (get_flag(Z)) {
        command_do_br(pdp, addr, word_command, params);
        return;
    }
    PRINT_RESULT("%06o",
                 (word_t)((pdp_reg_get_var(pdp, 7) + opcode.offset_xx * 2))
                     + 2);
}
//##########################################################################
//##########################################################################

//##########################################################################
void
command_do_tstb(struct pdp_11_t *pdp,
                address_word_t   addr,
                word_t           word_command,
                byte_t           params)
{
    (void) addr;
    //(void) params;
    OP_CODE_T_INIT
    if (!pdp) {
        return;
    }
    opcode = __get_mr(pdp, word_command, params);
    // PRINT_RESULT("\nopcode.dd.addr = %o\n", opcode.dd.addr);
    // __command_reg_dump(pdp);

    set_flag_NZ(opcode.dd.value);

    set_flag_V((word_t) 0);
    set_flag_C((word_t) 0);

    // PRINT_RESULT("\n", "");
    // pdp_reg_set_var(pdp, opcode.dd.addr, opcode.ss.value + );
    // pdp_mem_dump(pdp, 0x40, 0x20);
    // pdp_mem_dump(pdp, 0x200, 0x26);
    // __command_reg_dump(pdp);
}
//##########################################################################

//##########################################################################
void
command_do_bpl(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;
    //(void) params;
    if (!pdp) {
        return;
    }
    // __command_reg_dump(pdp);
    OP_CODE_T_INIT
    opcode = __get_mr(pdp, word_command, params);
    if (get_flag(N)) {
        command_do_br(pdp, addr, word_command, params);
        return;
    }
    PRINT_RESULT("%06o",
                 (word_t)((pdp_reg_get_var(pdp, 7) + opcode.offset_xx * 2))
                     + 2);
    // PRINT_RESULT("\n", "");
    // pdp_reg_set_var(pdp, opcode.dd.addr, opcode.ss.value + );
    // pdp_mem_dump(pdp, 0x40, 0x20);
    // pdp_mem_dump(pdp, 0x200, 0x26);
    // __command_reg_dump(pdp);
}
//##########################################################################
// JSR
//##########################################################################
void
command_do_jsr(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;
    (void) params;
    (void) word_command;
    (void) pdp;

    if (!pdp) {
        return;
    }

    OP_CODE_T_INIT
    opcode = __get_mr(pdp, word_command, params);

    word_t target_pc = pdp_reg_get_var(pdp, 7);

    word_t target_sp = pdp_reg_get_var(pdp, 6);

    if (opcode.r_reg != 7) {
        w_write(pdp, target_sp, pdp_reg_get_var(pdp, opcode.r_reg));
        pdp_reg_set_var(
            pdp, opcode.r_reg, (word_t)(pdp_reg_get_var(pdp, 7) + 2));
    } else {
        w_write(pdp, target_sp, (word_t)(target_pc + 2));
        pdp_reg_set_var(pdp, 7, (word_t)((opcode.dd.addr) + 2));
    }

    pdp_reg_set_var(pdp, 7, (word_t)((opcode.dd.addr) - 2));

    pdp_reg_set_var(pdp, 6, (word_t)(target_sp - 2));
}
//##########################################################################
// RTS
//##########################################################################
void
command_do_rts(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;
    (void) params;
    (void) word_command;
    (void) pdp;

    if (!pdp) {
        return;
    }

    text_t name_registers[3] = { '\0' };

    word_t target_reg = (word_t)(word_command & 07);
    word_t target_sp  = pdp_reg_get_var(pdp, 6);

    pdp_reg_set_var(pdp, 7, (word_t)(pdp_reg_get_var(pdp, target_reg) - 2));
    if (target_reg != 7) {
        pdp_reg_set_var(
            pdp, target_reg, (word_t)(w_read(pdp, (word_t)(target_sp + 2))));

    } else {
        pdp_reg_set_var(pdp,
                        target_reg,
                        (word_t)(w_read(pdp, (word_t)(target_sp + 2)) - 2));
    }

    pdp_reg_set_var(pdp, 6, (word_t)(target_sp + 2));

    if (target_reg == 6) {
        snprintf(name_registers, sizeof(name_registers), "ps");
    }
    if (target_reg == 7) {
        snprintf(name_registers, sizeof(name_registers), "pc");
    }
    if (target_reg < 6) {
        snprintf(name_registers, sizeof(name_registers), "r%d", target_reg);
    }

    // __command_reg_dump(pdp);
    PRINT_RESULT(" %s ", name_registers);
}
//##########################################################################
