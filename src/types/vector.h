#pragma once

#include <string.h>
#include "../platform/memory.h"

#if !defined(VECTOR_NO_TRACK)
#define VECTOR_NOSTD
#define VECTOR_MALLOC(size) tmalloc(size)
#define VECTOR_REALLOC(ptr, size) trealloc(ptr, size)
#define VECTOR_FREE(ptr) tfree(ptr)
#define VECTOR_MEMMOVE(dest, src, size) memmove(dest, src, size)
#define VECTOR_MEMCPY(dest, src, size) memcpy(dest, src, size)
#endif

#include <extc/extc_vec.h>

#define vector_template_def(name, type) extc_vector_template_def(name, type)
#define vector_template_impl(name, type) extc_vector_template_impl(name, type)


/* ============> Default Vector Types <============ */

vector_template_def(ptr, void*)
