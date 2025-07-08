#pragma once

#include <stddef.h>

// [[API Generator]]
/**
 * @brief Malloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param size
 * @return void*
 */
void* tmalloc(size_t size);

// [[API Generator]]
/**
 * @brief Realloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 * @param size
 * @return void*
 */
void* trealloc(void* ptr, size_t size);

// [[API Generator]]
/**
 * @brief Free with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 */
void tfree(void* ptr);


// [[API Generator]]
/**
 * @brief Get allocated memory
 *
 * @return size in bytes if compiled with HE_MEM_TRACK, 0 otherwise
 */
size_t get_allocated_memory(void);
