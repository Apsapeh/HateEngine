#include "log.h"
#define HE_MEM_NO_MACRO
#include "memory.h"
#include <types/vector.h>

#include <stdlib.h>

#if defined(HE_MEM_TRACK_TRACE)
struct allocationData {
    void* ptr;
    usize size;
    c_str file;
    i32 line;
    c_str user_func;
    c_str user_file;
    i32 user_line;
};
#elif defined(HE_MEM_TRACK)
struct allocationData {
    void* ptr;
    usize size;
    c_str user_func;
    c_str user_file;
    i32 user_line;
};
#endif

vector_template_def_with_properties(allocationData, struct allocationData, static);
vector_template_impl_with_properties(
        allocationData, struct allocationData, static, malloc, realloc, free, memmove, memcpy
);

// FIXME: Add mutex
static vec_allocationData AllocatedMemory;

static inline void* _tmalloc(usize size, c_str file, i32 line);
static inline void* _trealloc(void* ptr, usize size, c_str file, i32 line);

void memory_init(void) {
#if defined(HE_MEM_TRACK) | defined(HE_MEM_TRACK_TRACE)
    AllocatedMemory = vec_allocationData_init();
#endif
}

void memory_exit(void) {
#if defined(HE_MEM_TRACK) | defined(HE_MEM_TRACK_TRACE)
    u64 used = get_allocated_memory();
    if (used != 0) {
        __he_update_full_trace_info("", "", -1);
        LOG_ERROR_NO_ALLOC("Unallocated memory size: %zu bytes", used);

        for (usize i = 0; i < AllocatedMemory.size; ++i) {
            struct allocationData* data = &AllocatedMemory.data[i];

            if (data->user_line != -1) {
    #if defined(HE_MEM_TRACK_TRACE)
                LOG_ERROR_NO_ALLOC(
                        "\t%zu bytes AT %s:%d IN \"%s\"@%s:%d", data->size, data->file, data->line,
                        data->user_func, data->user_file, data->user_line
                );
    #else
                LOG_ERROR_NO_ALLOC(
                        "\t%zu bytes IN \"%s\"@%s:%d", data->size, data->user_func, data->user_file,
                        data->user_line
                );
    #endif
            } else {
    #if defined(HE_MEM_TRACK_TRACE)
                LOG_ERROR_NO_ALLOC("\t%zu bytes AT %s:%d", data->size, data->file, data->line);
    #endif
            }
        }
    }

    vec_allocationData_free(&AllocatedMemory);
#endif
}


void* tmalloc(u64 size) {
    return _tmalloc((usize) size, "", -1);
}

void* trealloc(void* ptr, u64 size) {
    return _trealloc(ptr, (usize) size, "", -1);
}

void tfree(void* ptr) {
#if defined(HE_MEM_TRACK) | defined(HE_MEM_TRACK_TRACE)
    for (usize i = 0; i < AllocatedMemory.size; i++) {
        if (AllocatedMemory.data[i].ptr == ptr) {
            vec_allocationData_erase(&AllocatedMemory, i);
            break;
        }
    }
#endif
    free(ptr);
}

u64 get_allocated_memory(void) {
#if defined(HE_MEM_TRACK) | defined(HE_MEM_TRACK_TRACE)
    u64 total = 0;
    for (usize i = 0; i < AllocatedMemory.size; i++) {
        total += AllocatedMemory.data[i].size;
    }
    return total;
#else
    return 0;
#endif
}


static inline void* _tmalloc(usize size, c_str file, i32 line) {
#if defined(HE_MEM_TRACK_TRACE)
    void* ptr = malloc(size);
    struct allocationData data = {ptr,
                                  size,
                                  file,
                                  line,
                                  full_trace_mod_level_func,
                                  full_trace_mod_level_file,
                                  full_trace_mod_level_line};
    vec_allocationData_push_back(&AllocatedMemory, data);
    return ptr;
#elif defined(HE_MEM_TRACK)
    void* ptr = malloc(size);
    struct allocationData data = {
            ptr, size, full_trace_mod_level_func, full_trace_mod_level_file, full_trace_mod_level_line
    };
    vec_allocationData_push_back(&AllocatedMemory, data);
    return ptr;
#else
    return malloc(size)
#endif
}

static inline void* _trealloc(void* ptr, usize size, c_str file, i32 line) {
// Default behavior
#if defined(HE_MEM_TRACK) | defined(HE_MEM_TRACK_TRACE)
    if (ptr == NULL)
        return _tmalloc(size, file, line);

    void* new_ptr = realloc(ptr, size);
    for (usize i = 0; i < AllocatedMemory.size; ++i) {
        struct allocationData* data = &AllocatedMemory.data[i];
        if (data->ptr == ptr) {
            data->ptr = ptr;
            data->size = size;
            data->user_file = full_trace_mod_level_file;
            data->user_func = full_trace_mod_level_func;
            data->user_line = full_trace_mod_level_line;
    #if defined(HE_MEM_TRACK_TRACE)
            data->file = file;
            data->line = line;
    #endif
            break;
        }
    }
    return new_ptr;
#else
    return realloc(ptr, size)
#endif
}


#if defined(HE_MEM_TRACK_TRACE)
void* trace_tmalloc(const char* file, i32 line, u64 size) {
    return _tmalloc(size, file, line);
}

void* trace_trealloc(const char* file, i32 line, void* ptr, u64 size) {
    return _trealloc(ptr, size, file, line);
}
#endif
