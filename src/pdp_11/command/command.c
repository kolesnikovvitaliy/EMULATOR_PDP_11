#include "pdp_11/command/command_p.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "utils/logger/logger.h"

#include <stdio.h>
#include <stdlib.h>

command_t template_commands[] = {
    { 0000000, 0111111, (byte_t *) "unknown", command_do_unknown, NO_PARAMS },
    { 0177777, 0000000, (byte_t *) "halt", command_do_halt, NO_PARAMS },
    { 0170000, 0010000, (byte_t *) "mov", command_do_mov, HAS_SS | HAS_DD },
    { 0170000, 0060000, (byte_t *) "add", command_do_add, HAS_SS | HAS_DD },
    { 0107700, 0005200, (byte_t *) "inc", command_do_inc, HAS_SS | HAS_DD }

    // добавить остальные команды
};

/* размер списка комманд */
byte_t commands_list = sizeof(template_commands) / sizeof(command_t);

command_t **
command_new()
{
    return (command_t **) malloc(
        commands_list
        * sizeof(command_t *)); // Выделение паамяти для обЪекта command_t
}

void
command_create(command_t **commands)
{
    for (int i = 0; i < commands_list; i++) {
        commands[i] = (command_t *) malloc(sizeof(command_t));
        if (commands[i] != NULL) {
            *commands[i] = template_commands[i];
        }
    }
}

void
command_destroy(command_t **commands)
{
    for (int i = 0; i < commands_list; i++) {
        free(commands[i]);
    }
}

void
print_command(address_word_t addr, word_t word_command, byte_t *name_command)
{
    PRINT_RESULT("%06o %06o : %s", addr, word_command, name_command);
}

void
command_reg_dump(struct pdp_11_t *pdp)
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
command_do_halt(struct pdp_11_t *pdp, address_word_t addr, word_t word_command)
{
    (void) addr;
    (void) word_command;
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    word_t *  ptr_pc  = ptr_pdp->PC;
    *ptr_pc += 2;
    command_reg_dump(pdp);
    // pdp_mem_dump(pdp, 0x40, 0x20);
    // pdp_mem_dump(pdp, 0x200, 0x26);
    PRINT_RESULT("THE END!!!\n", "");

    pdp_destroy(pdp);
    free(pdp);
    exit(0);
}

arg_t
get_args(struct pdp_11_t *pdp, word_t word_command)
{
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;

    arg_t  res          = { 0, 0 };
    word_t num_register = word_command & 7;
    byte_t mode         = (word_command >> 3) & 7;

    switch (mode) {
    case 0:
        res.addr  = num_register;
        res.value = *(ptr_pdp->R0 + res.addr);

        // ss -откуда, dd - куда;

        PRINT_RESULT("R%d ", res.addr);
        break;
    // мода 1, (R1)
    case 1:
        res.addr = *(ptr_pdp->R0 + num_register); // в регистре адрес
        res.value = w_read(pdp, res.addr); // по адресу - значение
        // ss -откуда, dd - куда;

        PRINT_RESULT("(R%d) ", num_register);
        break;
    // мода 2, (R1)+ или #3
    case 2:
        *(ptr_pdp->R0 + num_register) += 2;       // TODO: +1
        res.addr = *(ptr_pdp->R0 + num_register); // в регистре адрес
        res.value = w_read(pdp, res.addr); // по адресу - значение
        // печать разной мнемоники для PC и других регистров
        if (num_register == 7)
            PRINT_RESULT("#%o ", res.value);
        else
            PRINT_RESULT("(R%d)+ ", num_register);
        break;
    //мы еще не дописали другие моды
    default:
        ERROR("\nMode %d not implemented yet!\n", mode);
        exit(1);
    }
    return res;
}

op_code_t
__get_mr(struct pdp_11_t *pdp, word_t word_command)
{
    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    // ss -откуда, dd - куда;
    opcode.ss = get_args(pdp, word_command >> 6);
    opcode.dd = get_args(pdp, word_command);
    return opcode;
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
command_do_add(struct pdp_11_t *pdp, address_word_t addr, word_t word_command)
{

    (void) addr;
    op_code_t opcode  = { { 0, 0 }, { 0, 0 } };
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    if (pdp)
        opcode = __get_mr(pdp, word_command);
    *(ptr_pdp->R0 + opcode.dd.addr) = opcode.ss.value + opcode.dd.value;
}

void
command_do_mov(struct pdp_11_t *pdp, address_word_t addr, word_t word_command)
{

    (void) addr;
    op_code_t opcode = { { 0, 0 }, { 0, 0 } };
    if (pdp)
        opcode = __get_mr(pdp, word_command);
    pdp_11_t *ptr_pdp               = (pdp_11_t *) pdp;
    *(ptr_pdp->R0 + opcode.dd.addr) = (word_t) opcode.ss.value;
}

void
command_do_inc(struct pdp_11_t *pdp, address_word_t addr, word_t word_command)
{
    (void) addr;
    if (pdp)
        __get_mr(pdp, word_command);
}

void
command_do_unknown(struct pdp_11_t *pdp,
                   address_word_t   addr,
                   word_t           word_command)
{
    if (pdp)
        print_command(addr, word_command, (byte_t *) "unknown");
}
