#include <stdio.h>
#include <stdlib.h>

#include "pdp_11/command/command_p.h"
#include "pdp_11/pdp_11.h"
#include "utils/logger/logger.h"

#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"

command_t template_commands[] = {
    {0000000, 0111111, (byte_t*)"unknown", command_do_unknown},
    {0177777, 0000000, (byte_t*)"halt", command_do_halt},
    {0170000, 0010000, (byte_t*)"mov", command_do_mov},
    {0170000, 0060000, (byte_t*)"add", command_do_add},
    {0107700, 0005200, (byte_t*)"inc", command_do_inc}

    // добавить остальные команды
};


byte_t commands_list = sizeof(
                template_commands) / sizeof(command_t);


command_t**  command_new()
{
        return (command_t**)malloc(commands_list * sizeof(command_t*)); // Выделение паамяти для обЪекта command_t
}

void command_create(command_t** commands)
{
        for (int i = 0; i < commands_list; i++) {
                commands[i] = (command_t*)malloc(sizeof(command_t));
                if (commands[i] != NULL) {
                         *commands[i] = template_commands[i];
                }
        }
}

void command_destroy(command_t** commands)
{
        for (int i = 0; i < commands_list; i++) {
                free(commands[i]);
        }

}

void print_command(address_word_t addr, word_t word_command, byte_t* name_command)
{
        PRINT_RESULT("%06o %06o : %s", addr, word_command, name_command);
}

void command_reg_dump(struct pdp_11_t* pdp)
{
        pdp_11_t* ptr_pdp = (pdp_11_t*)pdp;
        reg_t* ptr_reg = (reg_t*)ptr_pdp->regist;


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

void command_do_halt(struct pdp_11_t* pdp,
                address_word_t addr, word_t word_command)
{
        print_command(addr, word_command, (byte_t*)"halt");
        command_reg_dump(pdp);
        PRINT_RESULT("THE END!!!", "");

        pdp_destroy(pdp);
        free(pdp);
        exit(0);

}

void command_do_add(struct pdp_11_t* pdp,
        address_word_t addr,  word_t word_command)
{
        struct pdp_11_t *ptr_pdp = pdp;
        if (ptr_pdp) printf("\b");
       print_command(addr, word_command, (byte_t*)"add");
}

void command_do_mov(struct pdp_11_t* pdp,
        address_word_t addr,  word_t word_command)
{
        struct pdp_11_t *ptr_pdp = pdp;
        if (ptr_pdp) printf("\b");
       print_command(addr, word_command, (byte_t*)"mov");
}

void command_do_inc(struct pdp_11_t* pdp,
        address_word_t addr,  word_t word_command)
{
        struct pdp_11_t *ptr_pdp = pdp;
        if (ptr_pdp) printf("\b");
       print_command(addr, word_command, (byte_t*)"inc");
}

void command_do_unknown(struct pdp_11_t* pdp,
        address_word_t addr,  word_t word_command)
{
        struct pdp_11_t *ptr_pdp = pdp;
        if (ptr_pdp) printf("\b");
       print_command(addr, word_command, (byte_t*)"unknown");
}
