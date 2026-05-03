#include "pdp_11/command/command_p.h"
#include "pdp_11/command/commands.h"
#include "pdp_11/command/commands_list.h"
#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "utils/logger/logger.h"

#include <stdio.h>
#include <stdlib.h>

extern command_t template_commands[];

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
__print_command(address_word_t addr, word_t word_command, byte_t *name_command)
{
    PRINT_RESULT("%06o %06o : %s", addr, word_command, name_command);
}

arg_t
__get_args(struct pdp_11_t *pdp, word_t word_command)
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
    opcode.ss = __get_args(pdp, word_command >> 6);
    opcode.dd = __get_args(pdp, word_command);
    return opcode;
}
