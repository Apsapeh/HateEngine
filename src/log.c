#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include "platform/datetime.h"
#include "platform/memory.h"
#include "platform/mutex.h"
#include "stdlib.h"
#include "types/types.h"
#include <platform/platform.h>
#include <platform/thread_local_storage.h>

#define FORMAT_BUFFER_SIZE 8192
#define TIME_BUFFER_SIZE 80

// clang-format off
thread_local_storage_impl(c_str, g_fullTraceFuncTLS)
thread_local_storage_impl(c_str, g_fullTraceFileTLS)
thread_local_storage_impl(i32, g_fullTraceLineTLS)
struct DummyClangFormat; // This code dirty hack for normal formatting. It's does nothing for program
// clang-format on

static mutex_handle g_printMutex = NULL;
static str g_formatBuffer = NULL;
static datetime_handle g_datetime = NULL;


void log_init(void) {
    g_fullTraceFuncTLS_try_init("");
    g_fullTraceFileTLS_try_init("");
    g_fullTraceLineTLS_try_init(-1);

    g_printMutex = mutex_new();
    g_formatBuffer = tmalloc(sizeof(char) * FORMAT_BUFFER_SIZE);
    g_datetime = datetime_new();
}

void log_exit(void) {
    mutex_free(g_printMutex);
    tfree(g_formatBuffer);
    datetime_free(g_datetime);
}

void __he_update_full_trace_info(const char* func, const char* file, i32 line) {
    g_fullTraceFuncTLS_set_value(func);
    g_fullTraceFileTLS_set_value(file);
    g_fullTraceLineTLS_set_value(line);

    if (line == 0) {
        LOG_FATAL_NO_ALLOC("OEUEOU")
    }
}

log_full_trace_info log_full_trace_get_info(void) {
    return (log_full_trace_info) {.func = g_fullTraceFuncTLS_get_value(),
                                  .file = g_fullTraceFileTLS_get_value(),
                                  .line = g_fullTraceLineTLS_get_value()};
}


static c_str set_default(void);
static c_str set_debug_color(void);
static c_str set_info_color(void);
static c_str set_warn_color(void);
static c_str set_error_color(void);
static c_str set_fatal_color(void);
static c_str set_trace_color(void);


static void update_format_buffer(c_str fmt, va_list args) {
    vsnprintf(g_formatBuffer, FORMAT_BUFFER_SIZE, fmt, args);
}

static void update_time_buffer(void) {
    datetime_update(g_datetime);
}

// 13-10-2025 13:36:45.348 [DEBUG] [dev.c:examples/dev.cpp@138] Debug message
//     Throwed in 'mat4_mult' at game.c@158


static void print_log(
        c_str type, c_str (*color_begin)(void), i32 line, const char* file, const char* fmt, va_list args
) {
    mutex_lock(g_printMutex);
    update_format_buffer(fmt, args);
    update_time_buffer();

    u8 day = datetime_get_day(g_datetime);
    u8 month = datetime_get_month(g_datetime);
    u16 year = datetime_get_year(g_datetime);

    u8 hour = datetime_get_hour(g_datetime);
    u8 minute = datetime_get_minute(g_datetime);
    u8 second = datetime_get_second(g_datetime);
    u32 nanosecond = datetime_get_nanosecond(g_datetime) / 1000000;

#if defined(PLATFORM_WINDOWS)
    color_begin();
    fprintf(stderr, "%02u-%02u-%04u %02u:%02u:%02u.%03u [%s] [%s:%d] %s\n", day, month, year, hour,
            minute, second, nanosecond, type, file, line, g_formatBuffer);
    set_default();
#elif defined(PLATFORM_UNIX)
    fprintf(stderr, "%s%02u-%02u-%04u %02u:%02u:%02u.%03u [%s] [%s:%d] %s%s\n", color_begin(), day,
            month, year, hour, minute, second, nanosecond, type, file, line, g_formatBuffer,
            set_default());
#endif

    if (g_fullTraceLineTLS_get_value() != -1) {
#if defined(PLATFORM_WINDOWS)
        set_trace_color();
        fprintf(stderr, "\tThrowed in '%s' at %s@%d%s\n", g_fullTraceFuncTLS_get_value(),
                g_fullTraceFileTLS_get_value(), g_fullTraceLineTLS_get_value());
        set_default();
#elif defined(PLATFORM_UNIX)
        fprintf(stderr, "%s\tThrowed in '%s' at %s@%d%s\n", set_trace_color(),
                g_fullTraceFuncTLS_get_value(), g_fullTraceFileTLS_get_value(),
                g_fullTraceLineTLS_get_value(), set_default());
#endif
    }
    mutex_unlock(g_printMutex);
}


static void print_log_no_alloc(
        c_str type, c_str (*color_begin)(void), i32 line, const char* file, const char* fmt, va_list args
) {
    // mutex_lock(PrintMutex);  Mutex can be not initialized here
#if defined(PLATFORM_WINDOWS)
    color_begin();
    fprintf(stderr, "[%s] [%s:%d] ", type, file, line);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    set_default();
#elif defined(PLATFORM_UNIX)
    fprintf(stderr, "%s[%s] [%s:%d] ", color_begin(), type, file, line);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "%s\n", set_default());
#endif

    if (g_fullTraceLineTLS_get_value() != -1) {
#if defined(PLATFORM_WINDOWS)
        set_trace_color();
        fprintf(stderr, "\tThrowed in '%s' at %s@%d%s\n", g_fullTraceFuncTLS_get_value(),
                g_fullTraceFileTLS_get_value(), g_fullTraceLineTLS_get_value());
        set_default();
#elif defined(PLATFORM_UNIX)
        fprintf(stderr, "%s\tThrowed in '%s' at %s@%d%s\n", set_trace_color(),
                g_fullTraceFuncTLS_get_value(), g_fullTraceFileTLS_get_value(),
                g_fullTraceLineTLS_get_value(), set_default());
#endif
    }
    // mutex_unlock(PrintMutex);  Mutex can be not initialized here
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


void __he_log_fatal_no_alloc(i32 line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log_no_alloc("FATAL", set_fatal_color, line, file, fmt, args);
    va_end(args);
    exit(1);
}

void __he_log_error_no_alloc(i32 line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log_no_alloc("ERROR", set_error_color, line, file, fmt, args);
    va_end(args);
}


#if defined(PLATFORM_WINDOWS) // Window
    #include <windows.h>

static c_str set_default(void) {
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    if (hConsole)
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
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
