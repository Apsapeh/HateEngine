#pragma once

#include "types/types.h"

extern c_str full_trace_mod_level_func;
extern c_str full_trace_mod_level_file;
extern i32 full_trace_mod_level_line;

/**
 * @brief Inner function to update the full trace info. Used for tracing with HEAPI_FULL_TRACE. Don't use
 directly
 *
 * @param func function_name, "" for reset
 * @param file "" for reset
 * @param line -1 for reset

 * @api
 */
void __he_update_full_trace_info(const char* func, const char* file, i32 line);

#define LOG_INFO(...)
#define LOG_WARN(...) he_log_warning(__LINE__, __FILE__, __VA_ARGS__);
#define LOG_ERROR(...) he_log_error(__LINE__, __FILE__, __VA_ARGS__);
#define LOG_FATAL(...) he_log_fatal(__LINE__, __FILE__, __VA_ARGS__);

#define LOG_DEBUG(...)

void he_log_fatal(i32 line, const char* file, const char* fmt, ...);

void he_log_info(i32 line, const char* file, const char* fmt, ...);

void he_log_debug(i32 line, const char* file, const char* fmt, ...);

void he_log_error(i32 line, const char* file, const char* fmt, ...);

void he_log_warning(i32 line, const char* file, const char* fmt, ...);
