#include <stdio.h>
#include <stdlib.h>

#include "pdp_11/command/command_p.h"
#include "pdp_11/pdp_11.h"
#include "utils/logger/logger.h"

command_t template_commands[] = {
    {0000000, 011111, (byte_t*)"unknown", command_do_nothing},
    {0177777, 000000, (byte_t*)"halt", command_do_halt},
    {0170000, 010000, (byte_t*)"mov", command_do_mov},
    {0170000, 060000, (byte_t*)"add", command_do_add},

    // добавить остальные команды
};


byte_t count_commands = sizeof(
                template_commands) / sizeof(command_t);


command_t**  command_new()
{
        return (command_t**)malloc(count_commands * sizeof(command_t*)); // Выделение паамяти для обЪекта command_t
}

void command_create(command_t** commands)
{
        for (int i = 0; i < count_commands; i++) {
                commands[i] = (command_t*)malloc(sizeof(command_t));
                if (commands[i] != NULL) {
                         *commands[i] = template_commands[i];
                }
        }
}

void command_destroy(command_t** commands)
{
        for (int i = 0; i < count_commands; i++) {
                free(commands[i]);
        }

}

void print_command(address_word_t addr, word_t word_command, byte_t* name_command)
{
        fprintf(stdout, "\n%06o %06o : %s", addr, word_command, name_command);
}

// COMMANDS

void command_do_halt(struct pdp_11_t* pdp,
                address_word_t addr, word_t word_command)
{
        print_command(addr, word_command, (byte_t*)"halt");
        INFO("\n THE END!!!\n", "");
        TRACE("\nHALT\n", "");
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

void command_do_nothing(struct pdp_11_t* pdp,
        address_word_t addr,  word_t word_command)
{
        struct pdp_11_t *ptr_pdp = pdp;
        if (ptr_pdp) printf("\b");
       print_command(addr, word_command, (byte_t*)"unknown");
}
