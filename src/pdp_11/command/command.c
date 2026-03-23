#include <stdio.h>
#include <stdlib.h>

#include "pdp_11/command/command_p.h"
#include "pdp_11/pdp_11.h"


command_t* command_new()
{
        return (command_t*)malloc(sizeof(command_t)); // Выделение паамяти для обЪекта command_t
}

void command_create(command_t* command)
{
    command->mask = 0x0000;
    command->opcode = 0x0000;
    command->name = malloc(10 * sizeof(byte_t));
    command->do_command = NULL;
    return;
}

void command_destroy(command_t* command)
{
    command->mask = 0x0000;
    command->opcode = 0x0000;
    free(command->name);
    command->do_command = NULL;
    return;

}
