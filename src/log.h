#pragma once

extern const char* full_trace_mod_level_func;
extern const char* full_trace_mod_level_file;
extern int full_trace_mod_level_line;

void he_update_full_trace_info(const char* func, const char* file, int line);

#define HATE_INFO(...)
#define HATE_WARN(...) he_log_warning(__LINE__, __FILE__, __VA_ARGS__);
#define HATE_ERROR(...) he_log_error(__LINE__, __FILE__, __VA_ARGS__);
#define HATE_FATAL(...) he_log_fatal(__LINE__, __FILE__, __VA_ARGS__);

#define HATE_DEBUG(...)

void he_log_fatal(int line, const char* file, const char* fmt, ...);

void he_log_info(int line, const char* file, const char* fmt, ...);

void he_log_debug(int line, const char* file, const char* fmt, ...);

void he_log_error(int line, const char* file, const char* fmt, ...);

void he_log_warning(int line, const char* file, const char* fmt, ...);