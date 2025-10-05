#pragma once

#include <stddef.h>

/**
 * @brief Malloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param size
 * @return void*
 *
 * @api
 */
void* tmalloc(size_t size);

/**
 * @brief Realloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 * @param size
 * @return void*
 *
 * @api
 */
void* trealloc(void* ptr, size_t size);

/**
 * @brief Free with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 *
 * @api
 */
void tfree(void* ptr);


void* trace_tmalloc(const char* ___file__, int __line__, size_t size);

void* trace_trealloc(const char* ___file__, int __line__, void* ptr, size_t size);

#if defined(HE_MEM_TRACK) && defined(HE_MEM_TRACK_TRACE) && !defined(HE_MEM_NO_MACRO)
#define tmalloc(size) trace_tmalloc(__FILE__, __LINE__, size)
#define trealloc(ptr, size) trace_trealloc(__FILE__, __LINE__, ptr, size)
#endif


/**
 * @brief Get allocated memory
 *
 * @return size in bytes if compiled with HE_MEM_TRACK, 0 otherwise
 *
 * @api
 */
size_t get_allocated_memory(void);
