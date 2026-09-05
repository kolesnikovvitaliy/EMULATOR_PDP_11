#ifndef INCLUDE_FOR_COMMANDS
#    define INCLUDE_FOR_COMMANDS
#    include "pdp_11/command/command_p.h"
#    include "pdp_11/command/commands.h"
#    include "pdp_11/pdp_11.h"
#    include "pdp_11/pdp_11_p.h"

#    include <stdlib.h>

extern word_t psw; // Переменная флагов состояния (NZVC)
extern byte_t set_has_b; // Проверка, команда типа byte или word
extern word_t      tick; // Количество выполненных машинных команд;
extern log_level_t current_log_level; // Уровень логирования;

extern void __command_reg_dump(struct pdp_11_t *);

extern void   set_flag_V(word_t);
extern void   set_flag_C(word_t);
extern void   set_flag_NZ(word_t);
extern byte_t get_flag(word_t);

#endif
#pragma once
