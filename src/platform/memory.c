#include "log.h"
#define HE_MEM_NO_MACRO
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(HE_MEM_TRACK)

#define VECTOR_NO_TRACK
#include <types/vector.h>

struct mem_pair {
    void* ptr;
    size_t size;
    const char* file;
    int line;
};

vector_template_def(mem_pair, struct mem_pair);
vector_template_impl(mem_pair, struct mem_pair);

static vec_mem_pair allocated_mem = {0, 0, NULL, NULL};

void mem_atexit() {
    size_t used = get_allocated_memory();
    LOG_ERROR("UNALLOCATED MEM SIZE: %zu bytes", used);

    for (size_t i = 0; i < allocated_mem.size; i++) {
        struct mem_pair pair = allocated_mem.data[i];
        LOG_ERROR("UNALLOCATED %zu bytes AT %s:%d", pair.size, pair.file, pair.line);
    }
}

void init_mem_tracking() {
    if (allocated_mem.data == NULL) {
        allocated_mem = vec_mem_pair_init();
        atexit(mem_atexit);
    }
}
#endif

void* tmalloc(size_t size) {
    void* ptr = malloc(size);
#if defined(HE_MEM_TRACK)
    init_mem_tracking();
    struct mem_pair pair = {ptr, size, "", -1};
    vec_mem_pair_push_back(&allocated_mem, pair);
    size_t used = get_allocated_memory();
    printf("USED MEM: %zu\n", used);
#endif
    return ptr;
}

void* trealloc(void* ptr, size_t size) {
#if defined(HE_MEM_TRACK)
    init_mem_tracking();
    if (ptr == NULL) {
        return tmalloc(size);
    } else {
        void* new_ptr = realloc(ptr, size);
        for (size_t i = 0; i < allocated_mem.size; i++) {
            if (allocated_mem.data[i].ptr == ptr) {
                allocated_mem.data[i].ptr = ptr;
                allocated_mem.data[i].size = size;
                break;
            }
        }
        return new_ptr;
    }
#else
    return realloc(ptr, size);
#endif
}

void tfree(void* ptr) {
#if defined(HE_MEM_TRACK)
    init_mem_tracking();
    for (size_t i = 0; i < allocated_mem.size; i++) {
        if (allocated_mem.data[i].ptr == ptr) {
            vec_mem_pair_erase(&allocated_mem, i);
            break;
        }
    }
#endif
    free(ptr);
}

void* trace_tmalloc(const char* ___file__, int __line__, size_t size) {
    void* ptr = malloc(size);
#if defined(HE_MEM_TRACK) && defined(HE_MEM_TRACK_TRACE)
    init_mem_tracking();
    struct mem_pair pair = {ptr, size, ___file__, __line__};
    vec_mem_pair_push_back(&allocated_mem, pair);
    size_t used = get_allocated_memory();
    printf("USED MEM: %zu\n", used);
#endif
    return ptr;
}

void* trace_trealloc(const char* ___file__, int __line__, void* ptr, size_t size) {
#if defined(HE_MEM_TRACK) && defined(HE_MEM_TRACK_TRACE)
    init_mem_tracking();
    if (ptr == NULL) {
        return trace_tmalloc(___file__, __line__, size);
    } else {
        void* new_ptr = realloc(ptr, size);
        for (size_t i = 0; i < allocated_mem.size; i++) {
            if (allocated_mem.data[i].ptr == ptr) {
                allocated_mem.data[i].ptr = new_ptr;
                allocated_mem.data[i].size = size;
                allocated_mem.data[i].file = ___file__;
                allocated_mem.data[i].line = __line__;
                break;
            }
        }
        return new_ptr;
    }
#else
    return realloc(ptr, size);
#endif
}


size_t get_allocated_memory(void) {
#if defined(HE_MEM_TRACK)
    size_t total = 0;
    for (size_t i = 0; i < allocated_mem.size; i++) {
        total += allocated_mem.data[i].size;
    }
    return total;
#else
    return 0;
#endif
}
