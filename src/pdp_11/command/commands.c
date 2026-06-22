#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands_list.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "utils/logger/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// TODO : ADD COMMAND CLR, SOB:

void
__command_reg_dump(struct pdp_11_t *pdp)
{
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    reg_t *   ptr_reg = (reg_t *) ptr_pdp->regist;

    PRINT_RESULT("\nr0:%o r1:%o r2:%o r3:%o r4:%o r5:%o r6:%o r7:%o\n",
                 ptr_reg->R0,
                 ptr_reg->R1,
                 ptr_reg->R2,
                 ptr_reg->R3,
                 ptr_reg->R4,
                 ptr_reg->R5,
                 ptr_reg->SP,
                 ptr_reg->PC);
}
// COMMANDS

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
    __command_reg_dump(pdp);
    // pdp_mem_dump(pdp, 0x40, 0x20);
    // pdp_mem_dump(pdp, 0x200, 0x26);
    PRINT_RESULT("THE END!!!\n", "");

    pdp_destroy(pdp);
    free(pdp);
    exit(0);
}

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
    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp) {
        opcode = __get_mr(pdp, word_command, params);
    }
    w_write(pdp, opcode.dd.addr, (word_t)(opcode.ss.value + opcode.dd.value));
    pdp_reg_set_var(
        pdp, opcode.dd.addr, (word_t)(opcode.ss.value + opcode.dd.value));
}

void
command_do_mov(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{

    (void) addr;
    //(void) params;
    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (!pdp) {
        return;
    }
    opcode = __get_mr(pdp, word_command, params);
    w_write(pdp, opcode.dd.addr, opcode.ss.value);
    pdp_reg_set_var(pdp, opcode.dd.addr, opcode.ss.value);
}

void
command_do_inc(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;
    //(void) params;
    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (!pdp) {
        return;
    }
    opcode = __get_mr(pdp, word_command, params);
    // w_write(pdp, opcode.dd.addr, opcode.ss.value);
    pdp_reg_set_var(pdp, opcode.dd.addr, opcode.ss.value);
}

void
command_do_clr(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    (void) addr;
    //(void) params;
    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (!pdp) {
        return;
    }
    opcode = __get_mr(pdp, word_command, params);
    // PRINT_RESULT("\nopcode.dd.addr = %o\n", opcode.dd.addr);
    //  __command_reg_dump(pdp);
    //
    w_write(pdp, opcode.dd.addr, (word_t)(opcode.ss.value + opcode.dd.value));
    // PRINT_RESULT("\n", "");
    // pdp_reg_set_var(pdp, opcode.dd.addr, opcode.ss.value + );
    // pdp_mem_dump(pdp, 0x40, 0x20);
    // pdp_mem_dump(pdp, 0x200, 0x26);
    // __command_reg_dump(pdp);
}

void
command_do_sob(struct pdp_11_t *pdp,
               address_word_t   addr,
               word_t           word_command,
               byte_t           params)
{
    //(void) addr;
    //(void) params;
    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (!pdp) {
        return;
    }
    opcode = __get_mr(pdp, word_command, params);
    //(void) opcode;
    // word_t num_register             = (word_command >> 6) & 7;
    // word_t register_value_decrement = pdp_reg_get_var(pdp, num_register);
    // PRINT_RESULT("\nregister_value_decrement = %o\n",
    //              register_value_decrement);
    // PRINT_RESULT("\nr = %o\n", num_register);
    // PRINT_RESULT("\naddr = %o\n", addr);
    // PRINT_RESULT("\nopcode.ss.addr = %o\n", opcode.ss.addr);
    // PRINT_RESULT("\nopcode.ss.value = %o\n", opcode.ss.value);
    // PRINT_RESULT("\nopcode.dd.addr = %o\n", opcode.dd.addr);
    // PRINT_RESULT("\nopcode.dd.value = %o\n", opcode.dd.value);
    // word_t pc = pdp_reg_get_var(pdp, 7);
    //
    // word_t w      = w_read(pdp, (address_word_t)(pc - 2));
    // word_t r      = (w >> 6) & 7;
    // word_t offset = w & 077;
    // pdp_reg_set_var(pdp, r, (word_t)(pdp_reg_get_var(pdp, r) - 1));
    // if (pdp_reg_get_var(pdp, r) != 0) {
    //     pdp_reg_set_var(
    //         pdp, 7, (word_t)(pdp_reg_get_var(pdp, 7) - 2 * offset));
    // }
    /*word w = w_read(pc - 2);
    int r = (w >> 6) & 7;  // 8-6 бит
    word offset = w & 077;  // смещение числа 0-5 биты

    reg[r]--;

    if(reg[r] != 0)
    {
        pc = pc - 2*offset;
    }*/

    // reg[r]--; // Уменьшаем регистр на 1
    // word_t temp_register_value = pdp_reg_get_var(pdp, opcode.dd.addr);
    // pdp_reg_set_var(pdp, opcode.dd.addr, word_t(temp_register_value - 2));

    // if ((word_t)(temp_register_value - 2) != 0) {
    //     PC = PC - (nn * 2); // Переход назад
    // }
    // return;
    // w_write(pdp, opcode.dd.addr, opcode.ss.value);
    // word_t register_value_decrement = pdp_reg_get_var(pdp, num_register);
    // if (register_value_decrement != 0) {
    //     pdp_reg_set_var(
    //         pdp, num_register, (word_t)(register_value_decrement - 1));
    //     pdp_reg_set_var(pdp, 7, (word_t)(opcode.ss.addr));
    // }

    __command_reg_dump(pdp);
    sleep(5);
}

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
