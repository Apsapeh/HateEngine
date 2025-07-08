#include "vec_def.h"
#include "extc/extc_vec.h"
#include "platform/memory.h"
#include <string.h>

#define VECTOR_MALLOC(size) tmalloc(size)
#define VECTOR_REALLOC(ptr, size) trealloc(ptr, size)
#define VECTOR_FREE(ptr) tfree(ptr)
#define VECTOR_MEMMOVE(dest, src, size) memmove(dest, src, size)
#define VECTOR_MEMCPY(dest, src, size) memcpy(dest, src, size)

vector_template_impl(ptr, void*)
