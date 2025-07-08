#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

#define HE_MEM_TRACK

#if defined(HE_MEM_TRACK)
#include <extc/extc_vec.h>
struct mem_pair {
    void* ptr;
    size_t size;
};

vector_template_def(mem_pair, struct mem_pair) vector_template_impl(mem_pair, struct mem_pair)

        static vec_mem_pair allocated_mem = {0, 0, NULL, NULL};

#endif

void* tmalloc(size_t size) {
    void* ptr = malloc(size);
#if defined(HE_MEM_TRACK)
    if (allocated_mem.data == NULL)
        allocated_mem = vec_mem_pair_init();
    struct mem_pair pair = {ptr, size};
    vec_mem_pair_push_back(&allocated_mem, pair);
    size_t used = get_allocated_memory();
    printf("USED MEM: %zu\n", used);
#endif
    return ptr;
}

void* trealloc(void* ptr, size_t size) {
#if defined(HE_MEM_TRACK)
    if (allocated_mem.data == NULL)
        allocated_mem = vec_mem_pair_init();
    for (size_t i = 0; i < allocated_mem.size; i++) {
        if (allocated_mem.data[i].ptr == ptr) {
            allocated_mem.data[i].size = size;
            break;
        }
    }
#endif
    return realloc(ptr, size);
}

void tfree(void* ptr) {
#if defined(HE_MEM_TRACK)
    if (allocated_mem.data == NULL)
        allocated_mem = vec_mem_pair_init();
    for (size_t i = 0; i < allocated_mem.size; i++) {
        if (allocated_mem.data[i].ptr == ptr) {
            vec_mem_pair_erase(&allocated_mem, i);
            break;
        }
    }
#endif
    free(ptr);
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
