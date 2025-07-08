#include "log.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"

void he_log_fatal(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    exit(1);
}
