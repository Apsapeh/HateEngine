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


void* trace_tmalloc(const char* ___file__, int __line__, size_t size);

void* trace_trealloc(const char* ___file__, int __line__, void* ptr, size_t size);

#if defined(HE_MEM_TRACK) && defined(HE_MEM_TRACK_TRACE) && !defined(HE_MEM_NO_MACRO)
#define tmalloc(size) trace_tmalloc(__FILE__, __LINE__, size)
#define trealloc(ptr, size) trace_trealloc(__FILE__, __LINE__, ptr, size)
#endif


// [[API Generator]]
/**
 * @brief Get allocated memory
 *
 * @return size in bytes if compiled with HE_MEM_TRACK, 0 otherwise
 */
size_t get_allocated_memory(void);
