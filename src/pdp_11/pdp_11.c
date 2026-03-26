#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "pdp_11/pdp_11_p.h"
#include "pdp_11/memory/mem.h"
#include "pdp_11/register/register.h"
#include "pdp_11/device_io/device_io.h"
#include "pdp_11/command/command.h"
#include "pdp_11/command/command_p.h"
#include "utils/utils.h"
#include "utils/logger/logger.h"

/////////////////////////////////////////////////////
pdp_11_t* pdp_new()
{
        return (pdp_11_t*)malloc(sizeof(pdp_11_t)); // Выделение памяти под обЬетк PDP_11;
}
//------------------------------------------------------------------;
void pdp_create(pdp_11_t* pdp)
{
        pdp->memory = (struct mem_t*)mem_new(); // Выделение памяти под обЬетк  памяти типа byte и word;
        pdp->device_io = (struct dev_io_t*)dev_io_new(); // Выделение памяти под обЬетк ввода вывода;
        pdp->regist = (struct register_t*)register_new(); // Выделение памяти под обЬетк Регистры;
        pdp->command = (struct command_t**)command_new();
        // Выделение памяти под обЬетк машинные Команды;
        assert(pdp->regist); // проверка
        assert(pdp->memory); // проверка
        assert(pdp->device_io); // проверка
        assert(pdp->command); // проверка

        mem_create(pdp->memory); // Инициализация обЬекта памяти
        dev_io_create(pdp->device_io); // Инициализация обЬекта ввода вывода
        reg_create((struct pdp_11_t*)pdp, pdp->regist);// Инициализация обЬекта Регистры
        command_create(pdp->command);// Инициализация обЬекта машинные Команды;

}
//------------------------------------------------------------------;
void pdp_destroy(pdp_11_t* pdp)
{
        mem_destroy((struct mem_t*)pdp->memory); // Уничтожение оБьекта память;
        free(pdp->memory); // Высвобождение памяти;
        dev_io_destroy((struct dev_io_t*)pdp->device_io); // Уничтожение оБьекта Ввода-Вывода;
        free(pdp->device_io); // Высвобождение памяти;
        reg_destroy((struct pdp_11_t*)pdp); // Уничтожение оБьекта Регистры;
        free(pdp->regist);
        command_destroy((struct command_t**)pdp->command); // Уничтожение оБьекта машинные команды;
        free(pdp->command);

}
//////////////////////////////////////////////////////

//------------------------------------------------------------------;
//------------------------------------------------------------------;
void b_write(pdp_11_t* pdp, address_byte_t addr, byte_t data)
{
        /* Записывает байт по указанному адрессу */
        assert(__is_valid_address(addr));
        byte_write((struct mem_t*)pdp->memory, addr, data);
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
byte_t b_read(pdp_11_t* pdp, address_byte_t addr)
{
        /* Читает байт из указанного адреса */
        assert(__is_valid_address(addr));
        return (byte_t)byte_read((struct mem_t*)pdp->memory, addr);
}

//------------------------------------------------------------------;
//------------------------------------------------------------------;
void w_write(pdp_11_t* pdp,
                address_word_t addr, word_t data)
{
        /* Записывает слово по указанному адрессу */
        assert(__is_valid_address(addr));
        word_write((struct mem_t*)pdp->memory, addr, data);
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
word_t w_read(pdp_11_t* pdp, address_word_t addr)
{
        /* Читает слово из указанного адреса */
        assert(__is_valid_address(addr));
        return (word_t)word_read((struct mem_t*)pdp->memory, addr);
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
void pdp_load_data(pdp_11_t* pdp, byte_t* filename)
{
        /**
         * Чтение потока данных из
         * Терминала или Файла
         * в зависимости от наличия
         * пути к файлу
         **/
        dev_io_load_data((struct pdp_11_t*)pdp,
                        (struct dev_io_t*)pdp->device_io,
                        (byte_t*)filename
                        );
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
void pdp_mem_dump(pdp_11_t* pdp, address_word_t addr, word_t size)
{
        /**
         * Форматированный вывод данных из памяти
         * из указанного адресса
         * определенного количества байт
         **/
        dev_io_mem_dump((struct pdp_11_t*)pdp, addr, size);
}
//------------------------------------------------------------------;
//------------------------------------------------------------------;
byte_t*
pdp_parse_filename(int argc, char **argv) {
        // Проверка наличия пути к файлу;
        if (1 == argc) {

                usage((byte_t*)argv[0]);
                /**
                 * Указание о возможности
                 * использовать программы с префиксом -t:
                 * "USAGE: ./pdp.out [-t] filename - input data:
                 **/
                return (byte_t*)0;
        }
        if (3 >= argc) {
                for(int i = 1; i < argc; i++) {
                        if (strcmp(argv[i], "-t")) {
                                return (byte_t*)argv[i];
                        }
                }
        }
        usage((byte_t*)argv[0]);
        assert(!argv[0]);
        exit(1);
}
//------------------------------------------------------------------;
word_t* do_command(pdp_11_t* pdp, command_t** commands,
                const address_word_t addr)
{
        extern byte_t count_commands;
        word_t *ptr_pc = pdp->R7;
        *ptr_pc = addr;
        word_t word_command;     // текущее слово, которое содержит команду
        // читаем текущее слово
        byte_t flag = 0;
        word_command = w_read(pdp, *ptr_pc);
        for (int i = 1; i < count_commands; i++) {
                 if ((word_command & commands[i]->mask) == commands[i]->opcode) {
                         commands[i]->do_commands_command((struct pdp_11_t*)pdp, addr, word_command);
                         flag = 1;
                         break;
                }
        }
        if (!flag) {
                commands[0]->do_commands_command((struct pdp_11_t*)pdp, addr, word_command);
        }
        // печатаем адрес и слово по этому адресу, как в листинге
        //TRACE("%06o %06o: ", *ptr_pc, w);
        // pc сразу же указывает на следующее неразобранное слово
        *ptr_pc += 2;
        return ptr_pc;
}
//------------------------------------------------------------------;

