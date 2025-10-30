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

#define vector_template_impl(name, type)                                                                \
    vector_template_impl_with_properties(name, type, , tmalloc, trealloc, tfree, memmove, memcpy)

#define vector_template_impl_static(name, type)                                                         \
    vector_template_impl_with_properties(name, type, static, tmalloc, trealloc, tfree, memmove, memcpy)


#define vector_template_def_with_properties(name, type, fn_prefix)                                      \
    extc_vector_template_def_with_properties(name, type, fn_prefix)

#define vector_template_def(name, type) extc_vector_template_def(name, type)

#define vector_template_def_static(name, type) vector_template_def_with_properties(name, type, static)

/* ============> Default Vector Types <============ */

vector_template_def(ptr, void*)

        vector_template_def(u8, u8)
