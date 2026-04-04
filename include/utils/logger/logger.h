#ifndef LOGGER_H
#    define LOGGER_H

typedef enum {
    PRINT_RESULT = 0,
    ERROR,
    INFO,
    WARNING,
    TRACE,
    DEBUG
} log_level_t;

#    define CLR_PRINT_RESULT "\x1b[37m" // Белый
#    define CLR_ERROR        "\x1b[31m" // Красный
#    define CLR_INFO         "\x1b[32m" // Зеленый
#    define CLR_WARNING      "\x1b[33m" // Желтый
#    define CLR_TRACE        "\x1b[94m" // Светло-синий
#    define CLR_DEBUG        "\x1b[36m" // Циан
#    define CLR_RESET        "\x1b[0m"

// void log_message(log_level_t, const char*, int,
//                const char*, const char*, ...);
void        log_message(log_level_t, const char *, ...);
log_level_t set_log_level(log_level_t);

// Макросы для удобного вызова
//#define ERROR(fmt, ...) log_message(ERROR, __FILE__, __LINE__, __func__,
// fmt,##__VA_ARGS__) #define INFO(fmt, ...) log_message(INFO, __FILE__,
//__LINE__, __func__, fmt,##__VA_ARGS__) #define WARNING(fmt, ...)
// log_message(WARNING,  __FILE__, __LINE__, __func__, fmt,##__VA_ARGS__)
//#define TRACE(fmt, ...) log_message(TRACE, __FILE__, __LINE__, __func__,
// fmt,##__VA_ARGS__) #define DEBUG(fmt, ...) log_message(DEBUG, __FILE__,
//__LINE__, __func__, fmt,##__VA_ARGS__)
#    define PRINT_RESULT(fmt, ...)                                            \
        log_message(PRINT_RESULT, fmt, ##__VA_ARGS__)
#    define ERROR(fmt, ...)   log_message(ERROR, fmt, ##__VA_ARGS__)
#    define INFO(fmt, ...)    log_message(INFO, fmt, ##__VA_ARGS__)
#    define WARNING(fmt, ...) log_message(WARNING, fmt, ##__VA_ARGS__)
#    define TRACE(fmt, ...)   log_message(TRACE, fmt, ##__VA_ARGS__)
#    define DEBUG(fmt, ...)   log_message(DEBUG, fmt, ##__VA_ARGS__)
#endif
#pragma once
