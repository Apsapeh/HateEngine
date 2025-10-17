#pragma once

#include "types/types.h"


void memory_init(void);

void memory_exit(void);

/**
 * @brief Malloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param size
 * @return void*
 *
 * @api
 */
void* tmalloc(u64 size);

/**
 * @brief Realloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 * @param size
 * @return void*
 *
 * @api
 */
void* trealloc(void* ptr, u64 size);

/**
 * @brief Free with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 *
 * @api
 */
void tfree(void* ptr);

/**
 * @brief Get allocated memory
 *
 * @return size in bytes if compiled with HE_MEM_TRACK, 0 otherwise
 *
 * @api
 */
u64 get_allocated_memory(void);


#if defined(HE_MEM_TRACK_TRACE) && !defined(HE_MEM_NO_MACRO)
void* trace_tmalloc(const char* ___file__, i32 __line__, u64 size);
void* trace_trealloc(const char* ___file__, i32 __line__, void* ptr, u64 size);

    #define tmalloc(size) trace_tmalloc(__FILE__, __LINE__, size)
    #define trealloc(ptr, size) trace_trealloc(__FILE__, __LINE__, ptr, size)
#endif

#define tmalloc_or_err(size, var_name)                                                                  \
    tmalloc(size);                                                                                      \
    if (!var_name) {                                                                                    \
        return ERROR_ALLOCATION_FAILED;                                                                 \
    }
