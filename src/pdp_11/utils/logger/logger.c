#include "utils/logger/logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

log_level_t current_log_level = DEBUG;
// void log_message(log_level_t level, const char* file, int line,
//                const char* func, const char* fmt, ...)
void
log_message(log_level_t level, const char *fmt, ...)
{

    const char *levels[] = { "NONE",    "PRINT_RESULT", "ERROR", "INFO",
                             "WARNING", "TRACE",        "DEBUG" };
    const char *level_colors[]
        = { CLR_NONE,    CLR_PRINT_RESULT, CLR_ERROR, CLR_INFO,
            CLR_WARNING, CLR_TRACE,        CLR_DEBUG };

    // Если уровень сообщения выше текущего
    // порога, игнорируем его
    if (level > current_log_level) {
        return;
    }
    // Получение текущего времени
    time_t     now = time(NULL);
    struct tm *t   = localtime(&now);
    char       time_str[20];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", t);

    // Печать префикса уровня
    // printf("[%s] ", levels[level]);
    // printf("%s[%s]\t[%s] [%s:%d] %s(): ", level_colors[level],
    // levels[level],
    //                time_str, file, line, func);
    if (level != 0) {
        if (level == 1) {
            printf("%s", level_colors[level]);
        } else {
            printf("\n%s[%s]: ", level_colors[level], levels[level]);
        }
    }

    // Обработка переменного числа аргументов
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf(CLR_RESET "");
    return;
}

log_level_t
set_log_level(log_level_t level)
{
    // Установка нового порогового и возврат
    // предыдущего значения logger;
    log_level_t old_level = current_log_level;
    current_log_level     = level;
    return old_level;
}
