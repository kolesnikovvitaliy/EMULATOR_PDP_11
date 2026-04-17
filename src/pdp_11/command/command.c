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

    PRINT_RESULT("r0:%o r1:%o r2:%o r3:%o r4:%o r5:%o r6:%o r7:%o",
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
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    word_t *  ptr_pc  = ptr_pdp->PC;
    *ptr_pc += 2;
    print_command(addr, word_command, (byte_t *) "halt");
    command_reg_dump(pdp);
    PRINT_RESULT("THE END!!!", "");

    pdp_destroy(pdp);
    free(pdp);
    exit(0);
}

arg_t
__get_mr(struct pdp_11_t *pdp, word_t word_command)
{
    // TODO: Получить значения мод;
    pdp_11_t *ptr_pdp      = (pdp_11_t *) pdp;
    arg_t     res          = { 0, 0 };
    word_t    num_register = word_command & 7;
    byte_t    num_mode     = (word_command >> 3) & 7;

    switch (num_mode) {
    case 0:
        res.addr  = num_register;
        res.value = *(ptr_pdp->R0 + num_register);
        TRACE("R%d\t ADDR = %06o\t VALUE = %06o\t ",
              num_register,
              res.addr,
              res.value);
        break;
        // мода 1, (R1)
        ////    case 1:
        //        res.addr = ;           // в регистре адрес
        //        res.val = w_read(res.adr);  // по адресу - значение
        //        trace(TRACE, "(R%d) ", r);
        //        break;
        //
        //
        //    // мода 2, (R1)+ или #3
        //    case 2:
        //        res.adr = reg[r];           // в регистре адрес
        //        res.val = w_read(res.adr);  // по адресу - значение
        //        reg[r] += 2;                // TODO: +1
        //        // печать разной мнемоники для PC и других регистров
        //        if (r == 7)
        //            trace(TRACE, "#%o ", res.val);
        //        else
        //            trace(TRACE, "(R%d)+ ", r);
        //        break;
        //
        //
        //    // мы еще не дописали другие моды
        //    default:
        //        trace(ERROR, "Mode %d not implemented yet!\n", m);
        //        exit(1);
    }
    return res;
}

void
command_do_add(struct pdp_11_t *pdp, address_word_t addr, word_t word_command)
{
    arg_t     res;
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    if (pdp)
        res = __get_mr(pdp, word_command);
    w_write(pdp, (ptr_pdp->R0 + res.addr), res.value);
}

void
command_do_mov(struct pdp_11_t *pdp, address_word_t addr, word_t word_command)
{
    arg_t res;
    if (pdp)
        res = __get_mr(pdp, word_command);

    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    w_write(pdp, (ptr_pdp->R0 + res.addr), res.value);

    // w_write(pdp, , res.value);
}

void
command_do_inc(struct pdp_11_t *pdp, address_word_t addr, word_t word_command)
{
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
