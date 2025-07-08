#pragma once

typedef struct dylib_handle dylib_handle;

/**
 * @brief Open library with OS specific extension (.so, .dylib, .dll)
 *
 * @param path
 * @return if success returns dylib_handle, else returns NULL
 */
dylib_handle* dylib_open(const char* path);

/**
 * @brief Open library without OS specific extension, as is
 *
 * @param path
 * @return if success returns dylib_handle, else returns NULL
 */
dylib_handle* dylib_open_raw(const char* path);

/**
 * @brief Close library
 *
 * @param handle
 */
void dylib_close(dylib_handle* handle);

/**
 * @brief Get symbol from library
 *
 * @param handle
 * @param name
 * @return if found returns pointer to symbol, else returns NULL
 */
void* dylib_sym(dylib_handle* handle, const char* name);

/**
 * @brief Get error message
 *
 * @return
 */
char* dylib_error(void);
