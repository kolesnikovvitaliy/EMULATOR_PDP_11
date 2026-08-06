/**
 * @file pdp_11.c
 * @brief Основная реализация эмулятора PDP-11.
 */

#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "pdp_11/device_io/device_io.h"
#include "pdp_11/memory/mem.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register.h"
#include "utils/logger/logger.h"
#include "utils/utils.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

word_t psw  = 0;
word_t tick = 0;

/**
 * @brief Создает новый экземпляр структуры PDP-11.
 * @return Указатель на выделенную память для pdp_11_t.
 */
pdp_11_t *
pdp_new(void)
{
    return (pdp_11_t *) malloc(sizeof(pdp_11_t));
}

/**
 * @brief Инициализирует компоненты PDP-11 (память, устройства ввода-вывода,
 * регистры, команды).
 * @param pdp Указатель на экземпляр PDP-11.
 */
void
pdp_create(pdp_11_t *pdp)
{
    pdp->memory    = (struct mem_t *) mem_new();
    pdp->device_io = (struct dev_io_t *) dev_io_new();
    pdp->regist    = (struct reg_t *) register_new();
    pdp->command   = (struct command_t **) command_new();

    assert(pdp->regist);
    assert(pdp->memory);
    assert(pdp->device_io);
    assert(pdp->command);

    mem_create(pdp->memory);
    dev_io_create(pdp->device_io);
    reg_create((struct pdp_11_t *) pdp, pdp->regist);
    command_create(pdp->command);
}

/**
 * @brief Освобождает ресурсы, связанные с экземпляром PDP-11.
 * @param pdp Указатель на экземпляр PDP-11 для уничтожения.
 */
void
pdp_destroy(pdp_11_t *pdp)
{
    mem_destroy((struct mem_t *) pdp->memory);
    free(pdp->memory);
    dev_io_destroy((struct dev_io_t *) pdp->device_io);
    free(pdp->device_io);
    reg_destroy((struct pdp_11_t *) pdp);
    free(pdp->regist);
    command_destroy((struct command_t **) pdp->command);
    free(pdp->command);
}

/**
 * @brief Записывает байт в память PDP-11.
 * @param pdp Указатель на экземпляр PDP-11.
 * @param addr Адрес памяти для записи.
 * @param data Данные (байт) для записи.
 */
void
b_write(pdp_11_t *pdp, address_byte_t addr, byte_t data)
{
    assert(__is_valid_address(pdp, addr));
    byte_write((struct mem_t *) pdp->memory, addr, data);
}

/**
 * @brief Читает байт из памяти PDP-11.
 * @param pdp Указатель на экземпляр PDP-11.
 * @param addr Адрес памяти для чтения.
 * @return Прочитанный байт.
 */
byte_t
b_read(pdp_11_t *pdp, address_byte_t addr)
{
    assert(__is_valid_address(pdp, addr));
    return (byte_t) byte_read((struct mem_t *) pdp->memory, addr);
}

/**
 * @brief Записывает слово (16 бит) в память PDP-11.
 * @param pdp Указатель на экземпляр PDP-11.
 * @param addr Адрес памяти для записи.
 * @param data Данные (слово) для записи.
 */
void
w_write(pdp_11_t *pdp, address_word_t addr, word_t data)
{
    assert(__is_valid_address(pdp, addr));
    word_write((struct mem_t *) pdp->memory, addr, data);
}

/**
 * @brief Читает слово (16 бит) из памяти PDP-11.
 * @param pdp Указатель на экземпляр PDP-11.
 * @param addr Адрес памяти для чтения.
 * @return Прочитанное слово.
 */
word_t
w_read(pdp_11_t *pdp, address_word_t addr)
{
    assert(__is_valid_address(pdp, addr));
    return (word_t) word_read((struct mem_t *) pdp->memory, addr);
}

/**
 * @brief Загружает данные в PDP-11 из файла или терминала.
 * @param pdp Указатель на экземпляр PDP-11.
 * @param filename Путь к файлу (если NULL, чтение из терминала).
 */
void
pdp_load_data(pdp_11_t *pdp, byte_t *filename)
{
    dev_io_load_data((struct pdp_11_t *) pdp,
                     (struct dev_io_t *) pdp->device_io,
                     (byte_t *) filename);
}

/**
 * @brief Выводит дамп памяти в форматированном виде.
 * @param pdp Указатель на экземпляр PDP-11.
 * @param addr Начальный адрес.
 * @param size Количество байт для вывода.
 */
void
pdp_mem_dump(pdp_11_t *pdp, address_word_t addr, word_t size)
{
    dev_io_mem_dump((struct pdp_11_t *) pdp, addr, size);
}

/**
 * @brief Разбирает аргументы командной строки для поиска имени файла.
 * @param argc Количество аргументов.
 * @param argv Массив строк аргументов.
 * @return Указатель на имя файла или 0, если файл не найден/не указан.
 */
byte_t *
pdp_parse_filename(int argc, char **argv)
{
    if (1 == argc) {
        usage((byte_t *) argv[0]);
        return (byte_t *) 0;
    }
    if (2 == argc) {
        if (!strcmp("-d", argv[1])) {
            return (byte_t *) 0;
        }
    }
    if (3 <= argc) {
        for (int i = 0; i < argc; i++) {
            if (!strcmp("-f", argv[i])) {
                for (int j = (i + 1); j < argc; j++) {
                    if (strcmp("-t", argv[j]) == 0) {
                        continue;
                    }
                    if (strcmp("-d", argv[j]) == 0) {
                        continue;
                    }

                    if ((strlen(argv[j]) > 4) && (argv[j][0] != '-')) {
                        return (byte_t *) argv[j];
                    }
                }
            }
        }
    }
    assert(!argv[0]);
    exit(1);
}
//////////////////////////////////////////////////////////////////
// Установка и получение значения регистра
word_t
pdp_reg_get_var(struct pdp_11_t *pdp, byte_t num_register)
{
    assert(pdp);
    if (num_register > 07) {
        assert(num_register);
    }
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    word_t *  ptr_reg = ptr_pdp->R0 + num_register;
    return *ptr_reg;
}
void
pdp_reg_set_var(struct pdp_11_t *pdp, byte_t num_register, word_t value)
{
    assert(pdp);
    if (num_register > 07) {
        return;
    }
    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    word_t *  ptr_reg = ptr_pdp->R0 + num_register;
    *ptr_reg          = value;
}
void
pdp_reg_clear(struct pdp_11_t *pdp)
{
    for (byte_t i = 0; i <= 6; i++) {
        pdp_reg_set_var(pdp, i, 00);
    }
}
address_word_t *
pdp_reg_get_addr(struct pdp_11_t *pdp, byte_t num_register)
{
    assert(pdp);

    pdp_11_t *ptr_pdp = (pdp_11_t *) pdp;
    word_t *  ptr_reg = ptr_pdp->R0 + num_register;
    return (address_word_t *) ptr_reg;
}
///////////////////////////////////////////////////////////
/**
 * @brief Выполняет одну машинную команду по указанному адресу.
 * @param pdp Указатель на экземпляр PDP-11.
 * @param commands Список доступных команд.
 * @param addr Адрес команды для выполнения.
 * @return Указатель на обновленный Program Counter (PC).
 */
word_t *
do_command(pdp_11_t *pdp, command_t **commands, const address_word_t addr)
{
    extern byte_t commands_list;
    word_t *      ptr_pc = pdp->PC;
    *ptr_pc              = addr;
    word_t word_command;

    byte_t flag  = 0;
    word_command = w_read(pdp, *ptr_pc);
    for (int i = 1; i < commands_list; i++) {
        if ((word_command & commands[i]->mask) == commands[i]->opcode) {
            PRINT_RESULT(
                "%06o %06o : %s ", addr, word_command, commands[i]->name);
            commands[i]->do_commands_command((struct pdp_11_t *) pdp,
                                             addr,
                                             word_command,
                                             commands[i]->params);
            PRINT_RESULT("\n", "");
            flag = 1;
            break;
        }
    }

    if (!flag) {
        commands[0]->do_commands_command(
            (struct pdp_11_t *) pdp, addr, word_command, commands[0]->params);
    }
    tick++;
    *ptr_pc = (word_t)(*ptr_pc + 2);
    return ptr_pc;
}
