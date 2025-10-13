#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include "platform/datetime.h"
#include "platform/memory.h"
#include "stdlib.h"
#include "types/types.h"
#include <platform/platform.h>

const usize FORMAT_BUFFER_SIZE = 8192;
const usize TIME_BUFFER_SIZE = 80;

c_str full_trace_mod_level_func = (c_str) "";
c_str full_trace_mod_level_file = (c_str) "";
i32 full_trace_mod_level_line = -1;

void __he_update_full_trace_info(const char* func, const char* file, i32 line) {
    full_trace_mod_level_func = func;
    full_trace_mod_level_file = file;
    full_trace_mod_level_line = line;
    // if (line != -1)
    //     printf("\tCalled [%s:%d]: %s\n", file, line, func);
}


static c_str set_default(void);
static c_str set_debug_color(void);
static c_str set_info_color(void);
static c_str set_warn_color(void);
static c_str set_error_color(void);
static c_str set_fatal_color(void);
static c_str set_trace_color(void);

static str format_buffer = NULL;
static void update_format_buffer(c_str fmt, va_list args) {
    if (!format_buffer) {
        format_buffer = tmalloc(sizeof(char) * FORMAT_BUFFER_SIZE);
    }
    vsnprintf(format_buffer, FORMAT_BUFFER_SIZE, fmt, args);
}

static datetime_handle datetime = NULL;
static void update_time_buffer(void) {
    if (!datetime) {
        datetime = datetime_new();
    }

    datetime_update(datetime);
}

// 13-10-2025 13:36:45.348 [DEBUG] [dev.c:examples/dev.cpp@138] Debug message
//     Throwed in 'mat4_mult' at game.c@158


static void print_log(
        c_str type, c_str (*color_begin)(void), i32 line, const char* file, const char* fmt, va_list args
) {
    update_format_buffer(fmt, args);
    update_time_buffer();

    u8 day = datetime_get_day(datetime);
    u8 month = datetime_get_month(datetime);
    u16 year = datetime_get_year(datetime);

    u8 hour = datetime_get_hour(datetime);
    u8 minute = datetime_get_minute(datetime);
    u8 second = datetime_get_second(datetime);
    u32 nanosecond = datetime_get_nanosecond(datetime) / 1000000;

#if defined(PLATFORM_WINDOWS)
    color_begin();
    fprintf(stderr, "%02u-%02u-%04u %02u:%02u:%02u.%03u [%s] [%s:%d] %s\n", day, month, year, hour,
            minute, second, nanosecond, type, file, line, format_buffer);
    set_default();
#elif defined(PLATFORM_UNIX)
    fprintf(stderr, "%s%02u-%02u-%04u %02u:%02u:%02u.%03u [%s] [%s:%d] %s%s\n", color_begin(), day,
            month, year, hour, minute, second, nanosecond, type, file, line, format_buffer,
            set_default());
#endif

    if (full_trace_mod_level_line != -1) {
#if defined(PLATFORM_WINDOWS)
        set_trace_color();
        fprintf(stderr, "\tThrowed in '%s' at %s@%d%s\n", full_trace_mod_level_func,
                full_trace_mod_level_file, full_trace_mod_level_line);
        set_default();
#elif defined(PLATFORM_UNIX)
        fprintf(stderr, "%s\tThrowed in '%s' at %s@%d%s\n", set_trace_color(), full_trace_mod_level_func,
                full_trace_mod_level_file, full_trace_mod_level_line, set_default());
#endif
    }
}

void __he_log_info(i32 line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log("INFO", set_info_color, line, file, fmt, args);
    va_end(args);
}

void __he_log_warning(i32 line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log("WARN", set_warn_color, line, file, fmt, args);
    va_end(args);
}


void __he_log_error(i32 line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log("ERROR", set_error_color, line, file, fmt, args);
    va_end(args);
}


void __he_log_fatal(i32 line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log("FATAL", set_fatal_color, line, file, fmt, args);
    va_end(args);
    exit(1);
}

void __he_log_debug(i32 line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log("DEBUG", set_debug_color, line, file, fmt, args);
    va_end(args);
}


#if defined(PLATFORM_WINDOWS) // Window
#include <windows.h>

static c_str set_default(void) {
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    if (hConsole)
        SetConsoleTextAttribute(hConsole, 0);
    return "";
}

static c_str set_debug_color(void) {
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    if (hConsole)
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    return "";
}

static c_str set_info_color(void) {
    return "";
}

static c_str set_warn_color(void) {
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    if (hConsole)
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    return "";
}

static c_str set_error_color(void) {
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    if (hConsole)
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    return "";
}

static c_str set_fatal_color(void) {
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    if (hConsole)
        SetConsoleTextAttribute(
                hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED |
                                  COMMON_LVB_UNDERSCORE
        );
    return "";
}

static c_str set_trace_color(void) {
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    if (hConsole)
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    return "";
}

#elif defined(PLATFORM_UNIX) // UNIX (ANSI escapes)
static c_str set_default(void) {
    return "\033[0m";
}

static c_str set_debug_color(void) {
    return "\033[32m";
}

static c_str set_info_color(void) {
    return "";
}

static c_str set_warn_color(void) {
    return "\033[33m";
}

static c_str set_error_color(void) {
    return "\033[31m";
}

static c_str set_fatal_color(void) {
    return "\033[37;41;4m";
}

static c_str set_trace_color(void) {
    return "\033[34m";
}
#endif
