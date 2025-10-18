#ifndef EXTC_VECTOR_GUARD_H
#define EXTC_VECTOR_GUARD_H

/*
#ifndef VECTOR_NOSTD
    #include <stdlib.h>
    #include <string.h>

    #define VECTOR_MALLOC(size) malloc(size)
    #define VECTOR_REALLOC(ptr, size) realloc(ptr, size)
    #define VECTOR_FREE(ptr) free(ptr)
    #define VECTOR_MEMMOVE(dest, src, size) memmove(dest, src, size)
    #define VECTOR_MEMCPY(dest, src, size) memcpy(dest, src, size)
#endif
*/

#define extc_vector_template_def_with_properties(name, type, fn_prefix)\
    typedef struct {\
        unsigned long capacity;\
        unsigned long size;\
        type* data;\
        void (*free_func)(type*);\
    } vec_##name;\
\
    fn_prefix vec_##name vec_##name##_init(void);\
    fn_prefix unsigned char vec_##name##_reserve(vec_##name * v, const unsigned long n);\
    fn_prefix unsigned char vec_##name##_insert(vec_##name * v,const unsigned long n, type o);\
    fn_prefix unsigned char vec_##name##_insert_data(vec_##name * v, const unsigned long n, type* d, const unsigned long count);\
    fn_prefix unsigned char vec_##name##_push_back(vec_##name * v, type o);\
    fn_prefix unsigned char vec_##name##_append(vec_##name * v, vec_##name * v2);\
    fn_prefix unsigned char vec_##name##_append_data(vec_##name * v, type* d, const unsigned long n);\
    fn_prefix type vec_##name##_pop_back(vec_##name * v);\
    fn_prefix unsigned char vec_##name##_erase(vec_##name * v, const unsigned long index);\
    fn_prefix unsigned char vec_##name##_erase_range(vec_##name * v, const unsigned long index, const unsigned long count);\
    fn_prefix unsigned char vec_##name##_shrink_to_fit(vec_##name * v);\
    fn_prefix void vec_##name##_set_free_func(vec_##name * v, void (*free_func)(type*));\
    fn_prefix void vec_##name##_free(vec_##name * v);\
    fn_prefix void vec_##name##_clear(vec_##name * v);


#define extc_vector_template_def(name, type) extc_vector_template_def_with_properties(name, type, )
    

#define extc_vector_template_impl_with_properties(name, type, fn_prefix, VECTOR_MALLOC, VECTOR_REALLOC, VECTOR_FREE, VECTOR_MEMMOVE, VECTOR_MEMCPY)\
    fn_prefix vec_##name vec_##name##_init(void) {\
        vec_##name res;\
        res.size = 0;\
        res.capacity = 0;\
        res.data = NULL;\
        res.free_func = NULL;\
        vec_##name##_reserve(&res, 1);\
        return res;\
    }\
    \
    fn_prefix unsigned char vec_##name##_reserve(vec_##name * v, const unsigned long n){\
        type* new_data;\
        if (v->capacity == n) return 0;\
        new_data = (type*)VECTOR_REALLOC(v->data, n * sizeof(type));\
        if ((void*)new_data == NULL) return 0;\
        v->data = new_data;\
        v->capacity = n;\
        return 1;\
    }\
    \
    fn_prefix unsigned char vec_##name##_insert(vec_##name * v, const unsigned long index, type o) {\
        if (v->capacity < v->size+1) \
            if (!vec_##name##_reserve(v, v->capacity*2)) return 0;\
        VECTOR_MEMMOVE(v->data+index+1, v->data+index, (v->size-index)*sizeof(type));\
        v->data[index] = o;\
        ++v->size;\
        return 1;\
    }\
    \
    fn_prefix unsigned char vec_##name##_insert_data(vec_##name * v, const unsigned long index, type* d, const unsigned long count) {\
        if (v->capacity < v->size+count) {\
            if (v->capacity*2 < v->size+count) {\
                if (!vec_##name##_reserve(v, v->size+count)) return 0;\
            } else {\
                if (!vec_##name##_reserve(v, v->capacity*2)) return 0;\
            }\
        }\
        VECTOR_MEMMOVE(v->data+index+count, v->data+index, (v->size-index)*sizeof(type));\
        VECTOR_MEMCPY(v->data+index, d, count*sizeof(type));\
        v->size += count;\
        return 1;\
    }\
    \
    fn_prefix unsigned char vec_##name##_push_back(vec_##name * v, type o) {\
        if(v->capacity < v->size+1) \
            if (!vec_##name##_reserve(v, v->capacity*2)) return 0;\
        v->data[v->size++] = o;\
        return 1;\
    }\
    \
    fn_prefix unsigned char vec_##name##_append(vec_##name * v, vec_##name * v2) {\
        if (v->capacity < v->size+v2->size) {\
            if (v->capacity*2 < v->size+v2->size) {\
                if (!vec_##name##_reserve(v, v->size+v2->size)) return 0;\
            } else {\
                if (!vec_##name##_reserve(v, v->capacity*2)) return 0;\
            }\
        }\
        VECTOR_MEMCPY(v->data + v->size, v2->data, v2->size * sizeof(type));\
        v->size = v->size+v2->size;\
        return 1;\
    }\
    fn_prefix unsigned char vec_##name##_append_data(vec_##name * v, type* d, const unsigned long n) {\
        if (v->capacity < v->size+n) {\
            if (v->capacity*2 < v->size+n) {\
                if (!vec_##name##_reserve(v, v->size+n)) return 0;\
            } else {\
                if (!vec_##name##_reserve(v, v->capacity*2)) return 0;\
            }\
        }\
        VECTOR_MEMCPY(v->data + v->size, d, n * sizeof(type));\
        v->size = v->size+n;\
        return 1;\
    }\
    \
    fn_prefix type vec_##name##_pop_back(vec_##name * v) {\
        if (v->free_func) v->free_func(&v->data[v->size-1]);\
        return v->data[--v->size];\
    }\
    \
    fn_prefix unsigned char vec_##name##_erase(vec_##name * v, const unsigned long index) {\
        if (index >= v->size) return 0;\
        if (v->free_func) v->free_func(&v->data[index]);\
        VECTOR_MEMCPY(v->data+index, v->data+index+1, (v->size-index-1)*sizeof(type));\
        --v->size;\
        return 1;\
    }\
    \
    fn_prefix unsigned char vec_##name##_erase_range(vec_##name * v, const unsigned long index, const unsigned long count) {\
        if (index+count > v->size) return 0;\
        {\
            unsigned long i;\
            for (i = 0; i < count; ++i)\
                if (v->free_func) v->free_func(&v->data[index+i]);\
        }\
        VECTOR_MEMCPY(v->data+index, v->data+index+count, (v->size-index-count)*sizeof(type));\
        v->size -= count;\
        return 1;\
    }\
    \
    fn_prefix unsigned char vec_##name##_shrink_to_fit(vec_##name * v) {\
        return vec_##name##_reserve(v, v->size);\
    }\
    \
    fn_prefix void vec_##name##_set_free_func(vec_##name * v, void (*free_func)(type*)) {\
        v->free_func = free_func;\
    }\
    fn_prefix void vec_##name##_free(vec_##name * v) {\
        {\
            unsigned long i;\
            for (i = 0; i < v->size; ++i)\
                if (v->free_func) v->free_func(&v->data[i]);\
        }\
        v->size = 0;\
        v->capacity = 0;\
        v->free_func = NULL;\
        VECTOR_FREE(v->data);\
    }\
    fn_prefix void vec_##name##_clear(vec_##name * v) {\
        {\
            unsigned long i;\
            for (i = 0; i < v->size; ++i)\
                if (v->free_func) v->free_func(&v->data[i]);\
        }\
        v->size = 0;\
        v->capacity = 0;\
        vec_##name##_reserve(v, 1);\
    }\

    
#ifndef VECTOR_NOSTD
    #include <stdlib.h>
    #include <string.h>

    #define extc_vector_template_impl(name, type) extc_vector_template_impl_with_properties(name, type, , malloc, realloc, free, memmove, memcpy)
#endif
    
    
#endif
