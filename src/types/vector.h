#pragma once

#include <string.h>
#include "../platform/memory.h"


#define VECTOR_NOSTD
#include <extc/extc_vec.h>

#define vector_template_impl_with_properties(                                                           \
        name, type, fn_prefix, _malloc, _realloc, _free, _memmove, _memcpy                              \
)                                                                                                       \
    extc_vector_template_impl_with_properties(                                                          \
            name, type, fn_prefix, _malloc, _realloc, _free, _memmove, _memcpy                          \
    )

#if !defined(VECTOR_NO_TRACK)
    #define vector_template_impl(name, type)                                                            \
        extc_vector_template_impl_with_properties(                                                      \
                name, type, , tmalloc, trealloc, tfree, memmove, memcpy                                 \
        )
#else
    #define vector_template_impl(name, type)                                                            \
        extc_vector_template_impl_with_properties(name, type, malloc, realloc, free, memmove, memcpy)
#endif


#define vector_template_def_with_properties(name, type, fn_prefix)                                      \
    extc_vector_template_def_with_properties(name, type, fn_prefix)
#define vector_template_def(name, type) extc_vector_template_def(name, type)

/* ============> Default Vector Types <============ */

vector_template_def(ptr, void*)
