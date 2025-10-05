#include "log.h"
#include <stdio.h>
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"

c_str full_trace_mod_level_func = (c_str) "";
c_str full_trace_mod_level_file = (c_str) "";
int full_trace_mod_level_line = -1;

void __he_update_full_trace_info(const char* func, const char* file, int line) {
    full_trace_mod_level_func = func;
    full_trace_mod_level_file = file;
    full_trace_mod_level_line = line;
    if (line != -1)
        printf("\tCalled [%s:%d]: %s\n", file, line, func);
}

void he_log_fatal(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

void he_log_error(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void he_log_warning(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
