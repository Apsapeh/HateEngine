
#pragma once
// clang-format off
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef size_t usize;

typedef float f32;
typedef double f64;

typedef uintptr_t uptr;
typedef intptr_t iptr;

typedef u8 boolean;
#define true 1
#define false 0

typedef char* str;
typedef const char* c_str;

typedef void (*fptr)(void);
typedef void* ptr;


#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX


#define I8_MAX INT8_MAX
#define I8_MIN INT8_MIN

#define I16_MAX INT16_MAX
#define I16_MIN INT16_MIN

#define I32_MAX INT32_MAX
#define I32_MIN INT32_MIN

#define I64_MAX INT64_MAX
#define I64_MIN INT64_MIN


typedef c_str Error;

#define ERROR_ASSERT_INFO(error, ...)                                                                   \
    do {                                                                                                \
        if (error != NULL) {                                                                            \
            LOG_INFO(__VA_ARGS__);                                                                      \
        }                                                                                               \
    } while (0)

#define ERROR_ASSERT_WARN(error, ...)                                                                   \
    do {                                                                                                \
        if (error != NULL) {                                                                            \
            LOG_WARN(__VA_ARGS__);                                                                      \
        }                                                                                               \
    } while (0)

#define ERROR_ASSERT_ERROR(error, ...)                                                                  \
    do {                                                                                                \
        if (error != NULL) {                                                                            \
            LOG_ERROR(__VA_ARGS__);                                                                     \
        }                                                                                               \
    } while (0)

#define ERROR_ASSERT_FATAL(error, ...)                                                                  \
    do {                                                                                                \
        if (error != NULL) {                                                                            \
            LOG_FATAL(__VA_ARGS__);                                                                     \
        }                                                                                               \
    } while (0)


#define ERROR_ARG_CHECK(arg, to_return)                                                                 \
    do {                                                                                                \
        if (!(arg)) {                                                                                   \
            LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (is NULL): " #arg);                              \
            set_error(ERROR_INVALID_ARGUMENT);                                                          \
            to_return                                                                                   \
        }                                                                                               \
    } while (0)

#define ERROR_ARGS_CHECK_1(a, end_block) ERROR_ARG_CHECK(a, end_block)
#define ERROR_ARGS_CHECK_2(a, b, end_block)                                                             \
    ERROR_ARG_CHECK(a, end_block);                                                                      \
    ERROR_ARG_CHECK(b, end_block)
#define ERROR_ARGS_CHECK_3(a, b, c, end_block)                                                          \
    ERROR_ARG_CHECK(a, end_block);                                                                      \
    ERROR_ARG_CHECK(b, end_block);                                                                      \
    ERROR_ARG_CHECK(c, end_block)
#define ERROR_ARGS_CHECK_4(a, b, c, d, end_block)                                                       \
    ERROR_ARG_CHECK(a, end_block);                                                                      \
    ERROR_ARG_CHECK(b, end_block);                                                                      \
    ERROR_ARG_CHECK(c, end_block);                                                                      \
    ERROR_ARG_CHECK(d, end_block)


#define ERROR_ALLOC_CHECK(arg, to_return)                                                               \
    do {                                                                                                \
        if (!(arg)) {                                                                                   \
            LOG_ERROR_OR_DEBUG_FATAL("Memory allocation failed for '" #arg "'");                        \
            set_error(ERROR_ALLOCATION_FAILED);                                                         \
            to_return                                                                                   \
        }                                                                                               \
    } while (0)


#define ERROR_ASSERT(error, ...) ERROR_ASSERT_FATAL(error, __VA_ARGS__)

#define ERROR_SUCCESS ((Error) NULL)

#define ERROR_NOT_IMPLEMENTED "NotImplemented"
#define ERROR_INVALID_ARGUMENT "InvalidArgument"
#define ERROR_ALREADY_EXISTS "AlreadyExists"
#define ERROR_NOT_FOUND "NotFound"
#define ERROR_INVALID_STATE "InvalidState"
#define ERROR_ALLOCATION_FAILED "AllocationFailed"
























// clang-format off
#define MAT4_NEW_M(                                                                                     \
        v_m00, v_m01, v_m02, v_m03, v_m10, v_m11, v_m12, v_m13, v_m20, v_m21, v_m22, v_m23, v_m30,      \
        v_m31, v_m32, v_m33                                                                             \
)                                                                                                       \
    (Mat4) {                                                                                            \
        .m = {                                                                                          \
            v_m00,                                                                                      \
            v_m01,                                                                                      \
            v_m02,                                                                                      \
            v_m03,                                                                                      \
            v_m10,                                                                                      \
            v_m11,                                                                                      \
            v_m12,                                                                                      \
            v_m13,                                                                                      \
            v_m20,                                                                                      \
            v_m21,                                                                                      \
            v_m22,                                                                                      \
            v_m23,                                                                                      \
            v_m30,                                                                                      \
            v_m31,                                                                                      \
            v_m32,                                                                                      \
            v_m33                                                                                       \
        }                                                                                               \
    }

#define MAT4_ZERO_M                                                                                     \
    MAT4_NEW_M(                                                                                         \
        0.0f, 0.0f, 0.0f, 0.0f,                                                                         \
        0.0f, 0.0f, 0.0f, 0.0f,                                                                         \
        0.0f, 0.0f, 0.0f, 0.0f,                                                                         \
        0.0f, 0.0f, 0.0f, 0.0f                                                                          \
    )
    
#define MAT4_ONE_M                                                                                      \
    MAT4_NEW_M(                                                                                         \
        1.0f, 0.0f, 0.0f, 0.0f,                                                                         \
        0.0f, 1.0f, 0.0f, 0.0f,                                                                         \
        0.0f, 0.0f, 1.0f, 0.0f,                                                                         \
        0.0f, 0.0f, 0.0f, 1.0f                                                                          \
    )
// clang-format on

#define IVEC2_NEW_M(v_x, v_y)                                                                           \
    (IVec2) {                                                                                           \
        .x = v_x, .y = v_y                                                                              \
    }
#define IVEC2_NEW_VAL_M IVEC2_NEW_M(v, v)
#define IVEC2_ZERO_M IVEC2_NEW_VAL_M(0)
#define IVEC2_ONE_M IVEC2_NEW_VAL_M(1)
#define IVEC2_MINUS_ONE_M IVEC2_NEW_VAL_M(-1)
#define IVEC2_MAX_M IVEC2_NEW_VAL_M(I32_MAX)
#define IVEC2_MIN_M IVEC2_NEW_VAL_M(I32_MIN)
#define IVEC3_NEW_M(v_x, v_y, v_z)                                                                      \
    (IVec3) {                                                                                           \
        .x = v_x, .y = v_y, .z = v_z                                                                    \
    }
#define IVEC3_NEW_VAL_M(v) IVEC3_NEW_M(v, v, v)
#define IVEC3_ZERO_M IVEC3_NEW_VAL_M(0)
#define IVEC3_ONE_M IVEC3_NEW_VAL_M(1)
#define IVEC3_MINUS_ONE_M IVEC3_NEW_VAL_M(-1)
#define IVEC3_MAX_M IVEC3_NEW_VAL_M(I32_MAX)
#define IVEC3_MIN_M IVEC3_NEW_VAL_M(I32_MIN)
#define IVEC4_NEW_M(v_x, v_y, v_z, v_w)                                                                 \
    (IVec4) {                                                                                           \
        .x = v_x, .y = v_y, .z = v_z, .w = v_w                                                          \
    }
#define IVEC4_NEW_VAL_M(v) IVEC4_NEW_M(v, v, v, v)
#define IVEC4_ZERO_M IVEC4_NEW_VAL_M(0);
#define IVEC4_ONE_M IVEC4_NEW_VAL_M(1);
#define IVEC4_MINUS_ONE_M IVEC4_NEW_VAL_M(-1);
#define IVEC4_MAX_M IVEC4_NEW_VAL_M(I32_MAX);
#define IVEC4_MIN_M IVEC4_NEW_VAL_M(I32_MIN);
// #define IVEC4_MIN_M (IVec4){.x=I32_MIN, .y=I32_MIN}
#define UVEC2_NEW_M(v_x, v_y)                                                                           \
    (UVec2) {                                                                                           \
        .x = v_x, .y = v_y                                                                              \
    }
#define UVEC2_NEW_VAL_M UVEC2_NEW_M(v, v)
#define UVEC2_ZERO_M UVEC2_NEW_VAL_M(0)
#define UVEC2_ONE_M UVEC2_NEW_VAL_M(1)
#define UVEC2_MAX_M UVEC2_NEW_VAL_M(I32_MAX)
#define UVEC3_NEW_M(v_x, v_y, v_z)                                                                      \
    (UVec3) {                                                                                           \
        .x = v_x, .y = v_y, .z = v_z                                                                    \
    }
#define UVEC3_NEW_VAL_M(v) UVEC3_NEW_M(v, v, v)
#define UVEC3_ZERO_M UVEC3_NEW_VAL_M(0)
#define UVEC3_ONE_M UVEC3_NEW_VAL_M(1)
#define UVEC3_MAX_M UVEC3_NEW_VAL_M(U32_MAX)
#define UVEC4_NEW_M(v_x, v_y, v_z, v_w)                                                                 \
    (UVec4) {                                                                                           \
        .x = v_x, .y = v_y, .z = v_z, .w = v_w                                                          \
    }
#define UVEC4_NEW_VAL_M(v) UVEC4_NEW_M(v, v, v, v)
#define UVEC4_ZERO_M UVEC4_NEW_VAL_M(0);
#define UVEC4_ONE_M UVEC4_NEW_VAL_M(1);
#define UVEC4_MAX_M UVEC4_NEW_VAL_M(U32_MAX);
// #define UVEC4_MIN_M (UVec4){.x=I32_MIN, .y=I32_MIN}
#define VEC2_NEW_M(v_x, v_y)                                                                            \
    (Vec2) {                                                                                            \
        .x = v_x, .y = v_y                                                                              \
    }
#define VEC2_NEW_VAL_M(v) VEC2_NEW_M(v, v)
#define VEC2_ZERO_M VEC2_NEW_VAL_M(0.0f)
#define VEC2_ONE_M VEC2_NEW_VAL_M(1.0f)
#define VEC2_MINUS_ONE_M VEC2_NEW_VAL_M(-1.0f)
// #define VEC2_MAX_M (Vec2){.x=I32_MAX, .y=I32_MAX}
// #define VEC2_MIN_M (Vec2){.x=I32_MIN, .y=I32_MIN}

#define VEC3_NEW_M(v_x, v_y, v_z)                                                                       \
    (Vec3) {                                                                                            \
        .x = v_x, .y = v_y, .z = v_z                                                                    \
    }
#define VEC3_NEW_VAL_M(v) VEC3_NEW_M(v, v, v)
#define VEC3_ZERO_M VEC3_NEW_VAL_M(0.0f)
#define VEC3_ONE_M VEC3_NEW_VAL_M(1.0f)
#define VEC3_MINUS_ONE_M VEC3_NEW_VAL_M(-1.0f)
// #define VEC3_MAX_M (Vec3){.x=I32_MAX, .y=I32_MAX}
// #define VEC3_MIN_M (Vec3){.x=I32_MIN, .y=I32_MIN}
#define VEC4_NEW_M(v_x, v_y, v_z, v_w)                                                                  \
    (Vec4) {                                                                                            \
        .x = v_x, .y = v_y, .z = v_z, .w = v_w                                                          \
    }
#define VEC4_NEW_VAL_M(v) VEC4_NEW_M(v, v, v, v)
#define VEC4_ZERO_M VEC4_NEW_VAL_M(0.0f)
#define VEC4_ONE_M VEC4_NEW_VAL_M(1.0f)
#define VEC4_MINUS_ONE_M VEC4_NEW_VAL_M(-1.0f)
// #define VEC4_MAX_M (Vec4){.x=I32_MAX, .y=I32_MAX}
// #define VEC4_MIN_M (Vec4){.x=I32_MIN, .y=I32_MIN}















/**
 * DateTime handler for get UTC and local date and time
 *
 * @api
 */
typedef struct datetime_handle * datetime_handle;

/**
 * @brief Opaque mutex handle
 * @api
 */
typedef struct mutex_handle * mutex_handle;

/**
 * @brief Chunk memory allocator for elements with fixed type and size
 *
 * Specially optimized allocator for small types, like thousands allocations/freeing of int in each frame
 *
 * @api
 */
typedef struct ChunkMemoryAllocator ChunkMemoryAllocator;

/**
 * @api
 */
typedef struct Object Object;

/**
 * @brief
 *
 * @api
 */
typedef struct Node Node;

/**
 * @brief
 *
 * @api
 */
typedef struct Node3D Node3D;

/**
 * @brief
 *
 * @api
 */
typedef struct Window Window;

/**
 * @brief
 *
 * @api
 */
typedef struct CanvasItem CanvasItem;

/**
 * @brief
 *
 * @api
 */
typedef struct Control Control;

/**
 * @brief
 *
 * @api
 */
typedef struct Viewport Viewport;

/**
 * @api
 */
typedef struct string string;

/**
 * @api
 */
typedef struct string_itr string_itr;

/**
 * @api
 */
typedef struct string_slice string_slice;

/**
 * @api
 */
typedef struct string_utf8 string_utf8;

/**
 * @api
 */
typedef struct string_itr_utf8 string_itr_utf8;

/**
 * @brief
 *
 * @api
 */
typedef struct FileStream FileStream;

/**
 * @brief
 *
 * @api
 */
typedef struct WindowServerWindow WindowServerWindow;

/**
 * @brief
 *
 * @api
 */
typedef struct WindowServerDisplay WindowServerDisplay;

/**
 * @api server
 * @api_config {
 *     "fn_prefix": "window_server_",
 *     "init_method": "___hate_engine_runtime_init_window_server"
 * }
 */
typedef struct WindowServerBackend WindowServerBackend;

/**
 * @brief
 *
 * @api
 */
typedef struct RenderContextSurface RenderContextSurface;

/**
 * @api server
 * @api_config {
 *     "fn_prefix": "render_context_",
 *     "init_method": "___hate_engine_runtime_init_render_context"
 * }
 */
typedef struct RenderContextBackend RenderContextBackend;

/**
 * @api server
 * @api_config {
 *     "fn_prefix": "render_server_",
 *     "init_method": "___hate_engine_runtime_init_render_server"
 * }
 */
typedef struct RenderServerBackend RenderServerBackend;

/**
 * @brief Primitive 4x4 matrix
 *
 * Raw data - m
 *
 * @api forward
 */
typedef struct Mat4 {
    float m[4];
} Mat4;

/**
 * @brief Primitive 2D vector
 *
 * @api forward
 */
typedef struct IVec2 {
    i32 x;
    i32 y;
} IVec2;

/**
 * @brief Primitive 3D vector
 *
 * @api forward
 */
typedef struct IVec3 {
    i32 x;
    i32 y;
    i32 z;
} IVec3;

/**
 * @brief Primitive 4D vector
 *
 * @api forward
 */
typedef struct IVec4 {
    i32 x;
    i32 y;
    i32 z;
    i32 w;
} IVec4;

/**
 * @brief Primitive 2D vector
 *
 * @api forward
 */
typedef struct UVec2 {
    u32 x;
    u32 y;
} UVec2;

/**
 * @brief Primitive 3D vector
 *
 * @api forward
 */
typedef struct UVec3 {
    u32 x;
    u32 y;
    u32 z;
} UVec3;

/**
 * @brief Primitive 4D vector
 *
 * @api forward
 */
typedef struct UVec4 {
    u32 x;
    u32 y;
    u32 z;
    u32 w;
} UVec4;

/**
 * @brief Primitive 2D vector
 *
 * @api forward
 */
typedef struct Vec2 {
    float x;
    float y;
} Vec2;

/**
 * @brief Primitive 3D vector
 *
 * @api forward
 */
typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

/**
 * @brief Primitive 4D vector
 *
 * @api forward
 */
typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

/**
 * @brief Virtual memory ptr
 *
 * @api
 */
typedef u32 chunk_mem_ptr;

/**
 * 's' - Seek from start
 *
 * 'c' - Seek from current
 *
 * 'e' - Seek from end
 *
 * @api
 */
typedef char FSSeekFrom;

/**
 * @brief
 *
 * @api
 */
typedef u64 UID;

/**
 * 'd' - disabled
 *
 * 'e' - enabled
 *
 * 'a' - enabled async
 *
 * @api
 */
typedef u8 WindowServerWindowVSync;

/**
 * 'w' - windowed
 *
 * 'f' - exclusive fullscreen
 *
 * 'b' - borderless fullscreen
 *
 * @api
 */
typedef u8 WindowServerWindowMode;

#define FS_SEEK_FROM_START 's'

#define FS_SEEK_FROM_CURRENT 'c'

#define FS_SEEK_FROM_END 'e'

#define WINDOW_SERVER_WINDOW_V_SYNC_UNKNOWN '\0'

#define WINDOW_SERVER_WINDOW_V_SYNC_DISABLED 'd'

#define WINDOW_SERVER_WINDOW_V_SYNC_ENABLED 'e'

#define WINDOW_SERVER_WINDOW_V_SYNC_ENABLED_ASYNC 'a'

#define WINDOW_SERVER_WINDOW_MODE_UNKNOWN '\0'

#define WINDOW_SERVER_WINDOW_MODE_WINDOWED 'w'

#define WINDOW_SERVER_WINDOW_MODE_FULLSCREEN 'f'

#define WINDOW_SERVER_WINDOW_MODE_BORDERLESS_FULLSCREEN 'b'



#if defined(HEAPI_COMPILE_TIME)

#error "HEAPI_COMPILE_TIME not supported yet"


#else

/**
 * @brief Set a last error that occurred on the current thread.
 *
 * @api
 */
extern void (*raw_set_error)(Error err);

/**
 * @brief Retrieve a last error that occurred on the current thread.
 *
 * @api
 */
extern Error (*raw_get_error)(void);

/**
 * @brief Inner function to update the full trace info. Used for tracing with HEAPI_FULL_TRACE. Don't use
 directly
 *
 * @param func function_name, "" for reset
 * @param file "" for reset
 * @param line -1 for reset

 * @api
 */
extern void (*raw___he_update_full_trace_info)(const char * func, const char * file, i32 line);

/**
 * @brief Create time instance and init with current time
 * @warning You should free datetime_handle with 'time_free'
 *
 * @api
 */
extern datetime_handle (*raw_datetime_new)(void);

/**
 * @brief free datetime_handle
 *
 * @api
 */
extern void (*raw_datetime_free)(datetime_handle handle);

/**
 * @brief Update existed time instance
 *
 * @api
 */
extern void (*raw_datetime_update)(datetime_handle handle);

/**
 * @brief get local year
 *
 * @api
 */
extern u16 (*raw_datetime_get_year)(datetime_handle handle);

/**
 * @brief get local month
 *
 * @api
 */
extern u8 (*raw_datetime_get_month)(datetime_handle handle);

/**
 * @brief get local day
 *
 * @api
 */
extern u8 (*raw_datetime_get_day)(datetime_handle handle);

/**
 * @brief get local hour
 *
 * @api
 */
extern u8 (*raw_datetime_get_hour)(datetime_handle handle);

/**
 * @brief get local minute
 *
 * @api
 */
extern u8 (*raw_datetime_get_minute)(datetime_handle handle);

/**
 * @brief get local second
 *
 * @api
 */
extern u8 (*raw_datetime_get_second)(datetime_handle handle);

/**
 * @brief get local nanosecond
 *
 * @api
 */
extern u32 (*raw_datetime_get_nanosecond)(datetime_handle handle);

/**
 * @brief get UTC year
 *
 * @api
 */
extern u16 (*raw_datetime_get_utc_year)(datetime_handle handle);

/**
 * @brief get UTC month
 *
 * @api
 */
extern u8 (*raw_datetime_get_utc_month)(datetime_handle handle);

/**
 * @brief get UTC day
 *
 * @api
 */
extern u8 (*raw_datetime_get_utc_day)(datetime_handle handle);

/**
 * @brief get UTC hour
 *
 * @api
 */
extern u8 (*raw_datetime_get_utc_hour)(datetime_handle handle);

/**
 * @brief get UTC minute
 *
 * @api
 */
extern u8 (*raw_datetime_get_utc_minute)(datetime_handle handle);

/**
 * @brief get UTC second
 *
 * @api
 */
extern u8 (*raw_datetime_get_utc_second)(datetime_handle handle);

/**
 * @brief Malloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param size
 * @return void*
 *
 * @api
 */
extern void * (*raw_tmalloc)(u64 size);

/**
 * @brief Realloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 * @param size
 * @return void*
 *
 * @api
 */
extern void * (*raw_trealloc)(void * ptr, u64 size);

/**
 * @brief Free with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 *
 * @api
 */
extern void (*raw_tfree)(void * ptr);

/**
 * @brief Get allocated memory
 *
 * @return size in bytes if compiled with HE_MEM_TRACK, 0 otherwise
 *
 * @api
 */
extern u64 (*raw_get_allocated_memory)(void);

/**
 * @brief Create a new mutex
 * @return Mutex handle or NULL on failure
 * @warning Must be freed with mutex_free()
 * @api
 */
extern mutex_handle (*raw_mutex_new)(void);

/**
 * @brief Create a new mutex which allows same thread to lock multiple times
 * @return Mutex handle or NULL on failure
 * @warning Must be freed with mutex_free()
 * @api
 */
extern mutex_handle (*raw_mutex_new_recursive)(void);

/**
 * @brief Free a mutex
 * @param handle Valid mutex handle
 * @warning Behavior undefined if mutex is locked
 * @api
 */
extern void (*raw_mutex_free)(mutex_handle handle);

/**
 * @brief Lock a mutex (blocking)
 * @param handle Valid mutex handle
 * @warning Undefined behavior if handle is NULL
 * @api
 */
extern void (*raw_mutex_lock)(mutex_handle handle);

/**
 * @brief Try to lock a mutex without blocking
 * @param handle Valid mutex handle
 * @return true if locked successfully, false if already locked
 * @api
 */
extern boolean (*raw_mutex_try_lock)(mutex_handle handle);

/**
 * @brief Unlock a mutex
 * @param handle Valid mutex handle
 * @warning Must be called by the thread that locked it
 * @api
 */
extern void (*raw_mutex_unlock)(mutex_handle handle);

/**
 * @brief Free object by type
 *
 * @param object
 * @api
 */
extern void (*raw_auto_free)(Object * object);

/**
 * @api
 */
extern Node * (*raw_node_new)(const char * name);

/**
 * @api
 */
extern Node * (*raw_from_node)(Node * node);

/**
 * @api
 */
extern void (*raw_node_set_name)(Node * node, const char * name);

/**
 * @api
 */
extern const char * (*raw_node_get_name)(Node * node);

/**
 * @brief Add a child node to a parent node
 * @warning Take ownership of child. Do not free it!
 * @param node
 * @param child
 *
 * @api
 */
extern void (*raw_node_add_child)(Node * node, Node * child);

/**
 * @brief Remove and free a child node by pointer from a parent node
 *
 * @param node
 * @param child
 * @return true if child was removed
 * @return false if child was not found
 *
 * @api
 */
extern boolean (*raw_node_remove_child)(Node * node, Node * child);

/**
 * @brief Remove and free a child node by name from a parent node
 *
 * @param node
 * @param name
 * @return true if child was removed
 * @return false if child was not found
 *
 * @api
 */
extern boolean (*raw_node_remove_child_by_name)(Node * node, const char * name);

/**
 * @brief Remove and free a child node by uid from a parent node
 *
 * @param node
 * @param uid
 * @return true if child was removed
 * @return false if child was not found
 *
 * @api
 */
extern boolean (*raw_node_remove_child_by_uid)(Node * node, UID uid);

/**
 * @api
 */
extern boolean (*raw_node_remove_all_children)(Node * node);

/**
 * @brief Get a child node by name
 *
 * @param node
 * @param name
 * @return Pointer to found child, NULL if not found
 *
 * @api
 */
extern Node * (*raw_node_get_child_by_name)(Node * node, const char * name);

/**
 * @api
 */
extern Node * (*raw_node_get_child_by_uid)(Node * node, UID uid);

/**
 * @brief
 *
 * @api
 */
extern Node3D * (*raw_node3d_new)(const char * name);

/**
 * @param name Node name
 * @param title Window title
 * @param w Width
 * @param h Height
 *
 * @api
 */
extern Window * (*raw_window_new)(const char * name, const char * title, i32 w, i32 h);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_window_set_title)(Window * this, const char * title);

/**
 * @brief
 *
 * @api
 */
extern const char * (*raw_window_get_title)(const Window * this);

/**
 * @api
 */
extern CanvasItem * (*raw_canvas_item_new)(const char * name);

/**
 * @brief
 *
 * @api
 */
extern Control * (*raw_control_new)(const char * name);

/**
 * @brief
 *
 * @api
 */
extern Viewport * (*raw_viewport_new)(const char * name);

/**
 * @brief
 *
 * @param m00
 * @param m01
 * @param m02
 * @param m03
 * @param m10
 * @param m11
 * @param m12
 * @param m13
 * @param m20
 * @param m21
 * @param m22
 * @param m23
 * @param m30
 * @param m31
 * @param m32
 * @param m33
 * @param [out] Result Mat4
 *
 * @api
 */
extern void (*raw_mat4_init)(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33, Mat4 *const out);

/**
 * @brief
 *
 * @param array
 * @return Mat4
 *
 * @api
 */
extern void (*raw_mat4_from_array)(const float *const array, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
extern void (*raw_mat4_clone)(const Mat4 *const a, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
extern void (*raw_mat4_add)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
extern void (*raw_mat4_sub)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
extern void (*raw_mat4_mul)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @param factor
 * @return Mat4
 *
 * @api
 */
extern void (*raw_mat4_scale)(const Mat4 *const a, const float factor, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
extern void (*raw_mat4_transpose)(const Mat4 *const a, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
extern void (*raw_mat4_inverse)(const Mat4 *const a, Mat4 *const out);

/**
 * @api
 */
extern void (*raw_ivec2_init)(const i32 x, const i32 y, IVec2 *const out);

/**
 * @api
 */
extern void (*raw_ivec2_add)(const IVec2 *const a, const IVec2 *const b, IVec2 *const out);

/**
 * @api
 */
extern void (*raw_ivec2_sub)(const IVec2 *const a, const IVec2 *const b, IVec2 *const out);

/**
 * @api
 */
extern void (*raw_ivec2_scale)(const IVec2 *const a, const float factor, IVec2 *const out);

/**
 * @api
 */
extern float (*raw_ivec2_dot)(const IVec2 *const a, const IVec2 *const b);

/**
 * @api
 */
extern float (*raw_ivec2_length)(const IVec2 *const a);

/**
 * @api
 */
extern void (*raw_ivec2_normalize)(const IVec2 *const a, IVec2 *const out);

/**
 * @api
 */
extern float (*raw_ivec2_distance)(const IVec2 *const a, const IVec2 *const b);

/**
 * @brief Add b to a
 *
 * @param[in/out] a
 * @param[in] b
 *
 * @api
 */
extern void (*raw_ivec2_add_in)(IVec2 *const to, const IVec2 *const what);

/**
 * @brief
 *
 * @param[in/out] from
 * @param[in] what
 *
 * @api
 */
extern void (*raw_ivec2_sub_in)(IVec2 *const from, const IVec2 *const what);

/**
 * @brief
 *
 * @param[in/out] to
 * @param[in] factor
 *
 * @api
 */
extern void (*raw_ivec2_scale_in)(IVec2 *const to, const float factor);

/**
 * @brief
 *
 * @param[in/out] a
 *
 * @api
 */
extern void (*raw_ivec2_normalize_in)(IVec2 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_ivec3_init)(i32 x, i32 y, i32 z, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_ivec3_add)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_ivec3_sub)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_ivec3_scale)(const IVec3 *const a, const float factor, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_ivec3_cross)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_ivec3_dot)(const IVec3 *const a, const IVec3 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_ivec3_length)(const IVec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_ivec3_normalize)(const IVec3 *const a, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_ivec3_distance)(const IVec3 *const a, const IVec3 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*raw_ivec3_add_in)(IVec3 *const to, const IVec3 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*raw_ivec3_sub_in)(IVec3 *const from, const IVec3 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*raw_ivec3_scale_in)(IVec3 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*raw_ivec3_normalize_in)(IVec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_ivec4_init)(i32 x, i32 y, i32 z, i32 w, IVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_ivec4_add)(const IVec4 *const a, const IVec4 *const b, IVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_ivec4_sub)(const IVec4 *const a, const IVec4 *const b, IVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_ivec4_scale)(const IVec4 *const a, const float factor, IVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_ivec4_dot)(const IVec4 *const a, const IVec4 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_ivec4_length)(const IVec4 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_ivec4_normalize)(const IVec4 *const a, IVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_ivec4_distance)(const IVec4 *const a, const IVec4 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*raw_ivec4_add_in)(IVec4 *const to, const IVec4 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*raw_ivec4_sub_in)(IVec4 *const from, const IVec4 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*raw_ivec4_scale_in)(IVec4 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*raw_ivec4_normalize_in)(IVec4 *const a);

/**
 * @api
 */
extern void (*raw_uvec2_init)(const u32 x, const u32 y, UVec2 *const out);

/**
 * @api
 */
extern void (*raw_uvec2_add)(const UVec2 *const a, const UVec2 *const b, UVec2 *const out);

/**
 * @api
 */
extern void (*raw_uvec2_sub)(const UVec2 *const a, const UVec2 *const b, UVec2 *const out);

/**
 * @api
 */
extern void (*raw_uvec2_scale)(const UVec2 *const a, const float factor, UVec2 *const out);

/**
 * @api
 */
extern float (*raw_uvec2_dot)(const UVec2 *const a, const UVec2 *const b);

/**
 * @api
 */
extern float (*raw_uvec2_length)(const UVec2 *const a);

/**
 * @api
 */
extern void (*raw_uvec2_normalize)(const UVec2 *const a, UVec2 *const out);

/**
 * @api
 */
extern float (*raw_uvec2_distance)(const UVec2 *const a, const UVec2 *const b);

/**
 * @brief Add b to a
 *
 * @param[in/out] a
 * @param[in] b
 *
 * @api
 */
extern void (*raw_uvec2_add_in)(UVec2 *const to, const UVec2 *const what);

/**
 * @brief
 *
 * @param[in/out] from
 * @param[in] what
 *
 * @api
 */
extern void (*raw_uvec2_sub_in)(UVec2 *const from, const UVec2 *const what);

/**
 * @brief
 *
 * @param[in/out] to
 * @param[in] factor
 *
 * @api
 */
extern void (*raw_uvec2_scale_in)(UVec2 *const to, const float factor);

/**
 * @brief
 *
 * @param[in/out] a
 *
 * @api
 */
extern void (*raw_uvec2_normalize_in)(UVec2 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_uvec3_init)(u32 x, u32 y, u32 z, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_uvec3_add)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_uvec3_sub)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_uvec3_scale)(const UVec3 *const a, const float factor, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_uvec3_cross)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_uvec3_dot)(const UVec3 *const a, const UVec3 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_uvec3_length)(const UVec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_uvec3_normalize)(const UVec3 *const a, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_uvec3_distance)(const UVec3 *const a, const UVec3 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*raw_uvec3_add_in)(UVec3 *const to, const UVec3 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*raw_uvec3_sub_in)(UVec3 *const from, const UVec3 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*raw_uvec3_scale_in)(UVec3 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*raw_uvec3_normalize_in)(UVec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_uvec4_init)(u32 x, u32 y, u32 z, u32 w, UVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_uvec4_add)(const UVec4 *const a, const UVec4 *const b, UVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_uvec4_sub)(const UVec4 *const a, const UVec4 *const b, UVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_uvec4_scale)(const UVec4 *const a, const float factor, UVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_uvec4_dot)(const UVec4 *const a, const UVec4 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_uvec4_length)(const UVec4 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_uvec4_normalize)(const UVec4 *const a, UVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_uvec4_distance)(const UVec4 *const a, const UVec4 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*raw_uvec4_add_in)(UVec4 *const to, const UVec4 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*raw_uvec4_sub_in)(UVec4 *const from, const UVec4 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*raw_uvec4_scale_in)(UVec4 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*raw_uvec4_normalize_in)(UVec4 *const a);

/**
 * @api
 */
extern void (*raw_vec2_init)(const float x, const float y, Vec2 *const out);

/**
 * @api
 */
extern void (*raw_vec2_add)(const Vec2 *const a, const Vec2 *const b, Vec2 *const out);

/**
 * @api
 */
extern void (*raw_vec2_sub)(const Vec2 *const a, const Vec2 *const b, Vec2 *const out);

/**
 * @api
 */
extern void (*raw_vec2_scale)(const Vec2 *const a, const float factor, Vec2 *const out);

/**
 * @api
 */
extern float (*raw_vec2_dot)(const Vec2 *const a, const Vec2 *const b);

/**
 * @api
 */
extern float (*raw_vec2_length)(const Vec2 *const a);

/**
 * @api
 */
extern void (*raw_vec2_normalize)(const Vec2 *const a, Vec2 *const out);

/**
 * @api
 */
extern float (*raw_vec2_distance)(const Vec2 *const a, const Vec2 *const b);

/**
 * @brief Add b to a
 *
 * @param[in/out] a
 * @param[in] b
 *
 * @api
 */
extern void (*raw_vec2_add_in)(Vec2 *const to, const Vec2 *const what);

/**
 * @brief
 *
 * @param[in/out] from
 * @param[in] what
 *
 * @api
 */
extern void (*raw_vec2_sub_in)(Vec2 *const from, const Vec2 *const what);

/**
 * @brief
 *
 * @param[in/out] to
 * @param[in] factor
 *
 * @api
 */
extern void (*raw_vec2_scale_in)(Vec2 *const to, const float factor);

/**
 * @brief
 *
 * @param[in/out] a
 *
 * @api
 */
extern void (*raw_vec2_normalize_in)(Vec2 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_vec3_init)(float x, float y, float z, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_vec3_add)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_vec3_sub)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_vec3_scale)(const Vec3 *const a, const float factor, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_vec3_cross)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_vec3_dot)(const Vec3 *const a, const Vec3 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_vec3_length)(const Vec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_vec3_normalize)(const Vec3 *const a, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_vec3_distance)(const Vec3 *const a, const Vec3 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*raw_vec3_add_in)(Vec3 *const to, const Vec3 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*raw_vec3_sub_in)(Vec3 *const from, const Vec3 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*raw_vec3_scale_in)(Vec3 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*raw_vec3_normalize_in)(Vec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_vec4_init)(float x, float y, float z, float w, Vec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_vec4_add)(const Vec4 *const a, const Vec4 *const b, Vec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_vec4_sub)(const Vec4 *const a, const Vec4 *const b, Vec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_vec4_scale)(const Vec4 *const a, const float factor, Vec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_vec4_dot)(const Vec4 *const a, const Vec4 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_vec4_length)(const Vec4 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*raw_vec4_normalize)(const Vec4 *const a, Vec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*raw_vec4_distance)(const Vec4 *const a, const Vec4 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*raw_vec4_add_in)(Vec4 *const to, const Vec4 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*raw_vec4_sub_in)(Vec4 *const from, const Vec4 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*raw_vec4_scale_in)(Vec4 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*raw_vec4_normalize_in)(Vec4 *const a);

/**
 * @api
 */
extern Error (*raw_string_new)(string ** str);

/**
 * @api
 */
extern string * (*raw_string_from)(const char * c_str);

/**
 * @api
 */
extern string * (*raw_string_clone)(const string * c_str);

/**
 * @api
 */
extern string * (*raw_string_set_cstr)(string * self, const char * c_cstr);

/**
 * @api
 */
extern string * (*raw_string_set)(string * self, const string * c_str);

/**
 * @api
 */
extern char * (*raw_string_cstr)(const string * c_str);

/**
 * @api
 */
extern usize (*raw_string_len)(const string * c_str);

/**
 * @api
 */
extern usize (*raw_string_size)(const string * c_str);

/**
 * @api
 */
extern string * (*raw_string_push_back)(string * dest, const string * src);

/**
 * @api
 */
extern string * (*raw_string_push_back_cstr)(string * dest, const char * src);

/**
 * @api
 */
extern string * (*raw_string_push_front)(string * dest, const string * src);

/**
 * @api
 */
extern string * (*raw_string_push_front_cstr)(string * dest, const char * src);

/**
 * @api
 */
extern string * (*raw_string_insert)(string * dest, const string * src, const usize i);

/**
 * @api
 */
extern string * (*raw_string_insert_cstr)(string * dest, const char * src, const usize i);

/**
 * @api
 */
extern string * (*raw_string_insert_cstr_ex)(string * dest, const char * src, const usize i);

/**
 * @api
 */
extern string * (*raw_string_insert_cstr_by_byte)(string * dest, const char * src, const usize b);

/**
 * @api
 */
extern string * (*raw_string_insert_by_byte)(string * dest, const string * src, const usize b);

/**
 * @api
 */
extern string * (*raw_string_remove)(string * self, const usize i);

/**
 * @api
 */
extern string * (*raw_string_remove_by_byte)(string * self, const usize b);

/**
 * @api
 */
extern string * (*raw_string_remove_n)(string * self, const usize i, const usize n);

/**
 * @api
 */
extern string * (*raw_string_remove_n_by_byte)(string * self, const usize b, const usize b_n);

/**
 * @api
 */
extern boolean (*raw_string_equals)(const string * str1, const string * str2);

/**
 * @api
 */
extern boolean (*raw_string_equals_cstr)(const string * str, const char * cstr);

/**
 * @api
 */
extern void (*raw_string_free)(string * str);

/**
 * @api
 */
extern void (*raw_string_itr_free)(string_itr * str_itr);

/**
 * @api
 */
extern string_itr * (*raw_string_get_itr)(const string * c_str);

/**
 * @api
 */
extern u8 (*raw_string_itr_next)(string_itr * str_itr);

/**
 * @api
 */
extern string_slice * (*raw_string_get_slice)(const string * str, const usize s, const usize e);

/**
 * @api
 */
extern string * (*raw_string_from_slice)(const string_slice * str_sl);

/**
 * @api
 */
extern boolean (*raw_string_equals_slice)(string_slice * str_sl_1, string_slice * str_sl_2);

/**
 * @api
 */
extern string_slice * (*raw_string_set_slice)(string_slice * self, const string_slice * str_sl);

/**
 * @api
 */
extern string * (*raw_string_push_back_slice)(string * dest, const string_slice * src);

/**
 * @api
 */
extern string * (*raw_string_push_front_slice)(string * dest, const string_slice * src);

/**
 * @api
 */
extern string * (*raw_string_insert_slice_ex)(string * dest, const string_slice * src, const usize i);

/**
 * @api
 */
extern string * (*raw_string_insert_slice)(string * dest, const string_slice * src, const usize i);

/**
 * @api
 */
extern string * (*raw_string_insert_slice_by_byte)(string * dest, const string_slice * src, const usize b);

/**
 * @api
 */
extern void (*raw_string_slice_free)(string_slice * str_sl);

/**
 * @api
 */
extern Error (*raw_string_utf8_new)(string_utf8 ** str);

/**
 * @api
 */
extern Error (*raw_string_utf8_to_string)(string ** dest, const string_utf8 * str);

/**
 * @api
 */
extern Error (*raw_string_utf8_from)(string_utf8 ** str, const char * c_str);

/**
 * @api
 */
extern Error (*raw_string_utf8_len)(usize * len, const string_utf8 * c_str);

/**
 * @api
 */
extern Error (*raw_string_utf8_size)(usize * size, const string_utf8 * c_str);

/**
 * @api
 */
extern Error (*raw_string_utf8_clone)(string_utf8 ** str, const string_utf8 * c_str);

/**
 * @api
 */
extern Error (*raw_string_utf8_push_back_cstr)(string_utf8 * dest, const char * src);

/**
 * @api
 */
extern Error (*raw_string_utf8_push_back)(string_utf8 * dest, const string_utf8 * src);

/**
 * @api
 */
extern Error (*raw_string_utf8_insert)(string_utf8 * dest, const string_utf8 * src, const usize i);

/**
 * @api
 */
extern Error (*raw_string_utf8_push_front)(string_utf8 * dest, const string_utf8 * src);

/**
 * @api
 */
extern void (*raw_string_utf8_free)(string_utf8 * str);

/**
 * @brief
 *
 * @api
 */
extern UID (*raw_uid_new)(void);

/**
 * @brief Mount Resource File (*.hefs) to Virtual File System
 *
 * @param path Path to resource file
 * @param mount_point Mount point
 * @return true
 * @return false
 *
 * @api
 */
extern boolean (*raw_vfs_mount_res)(const char * path, const char * mount_point);

/**
 * @brief Unmount Resource File from Virtual File System
 *
 * @param mount_point Mount point
 * @return true
 * @return false
 *
 * @api
 */
extern boolean (*raw_vfs_unmount_res)(const char * mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res
 *
 * @return true if mounted successfully
 * @return false if mount failed
 *
 * @api
 */
extern boolean (*raw_vfs_mount_rfs)(const char * mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res only with allowed files in whitelist
 *
 * @return true if mounted successfully
 * @return false if mount failed
 *
 * @api
 */
extern boolean (*raw_vfs_mount_rfs_whitelist)(const char ** whitelist, u64 count, const char * mount_point);

/**
 * @brief Unmount Real File System from Virtual File System Res
 *
 * @return true if unmounted successfully
 * @return false if unmount failed
 *
 * @api
 */
extern boolean (*raw_vfs_unmount_rfs)(void);

/**
 * @brief Check if file exists
 *
 * @param path
 * @return true if exists
 * @return false if not exists
 *
 * @api
 */
extern boolean (*raw_vfs_res_path_exists)(const char * path);

/**
 * @brief Check if file exists in User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return true if exists
 * @return false if not exists
 *
 * @api
 */
extern boolean (*raw_vfs_usr_path_exists)(const char * path, boolean prefer_res);

/**
 * @brief Allocate memory and read file from Reosource scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @return Pointer to allocated memory with file data if success, else NULL
 *
 * @api
 */
extern void * (*raw_vfs_res_read_file)(const char * path, u64 * size);

/**
 * @brief Open file stream from Resource scope
 *
 * @param path
 * @return FileStream* If success, else NULL
 *
 * @api
 */
extern FileStream * (*raw_vfs_res_stream_open)(const char * path);

/**
 * @brief Allocate memory and read file from User scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @param prefer_res If true, try to read from Resource scope first
 * @return Pointer to allocated memory with file data if success, else NULL
 *
 * @api
 */
extern void * (*raw_vfs_usr_read_file)(const char * path, u64 * size, boolean prefer_res);

/**
 * @brief Write file to User scope
 *
 * @param path
 * @param data
 * @param size
 * @return true if success
 * @return false if fail
 *
 * @api
 */
extern boolean (*raw_vfs_usr_write_file)(const char * path, const void * data, u64 size);

/**
 * @brief Open file stream from User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return FileStream* If success, else NULL
 *
 * @api
 */
extern FileStream * (*raw_vfs_usr_stream_open)(const char * path, boolean prefer_res);

/**
 * @brief Get file stream size in bytes
 *
 * @param path
 * @return u64
 *
 * @api
 */
extern u64 (*raw_vfs_stream_size)(FileStream * stream);

/**
 * @brief Read from file stream n bytes to buffer
 *
 * @param stream
 * @param buffer
 * @param size
 * @return u64
 *
 * @api
 */
extern u64 (*raw_vfs_stream_read_n)(FileStream * stream, void * buffer, u64 size);

/**
 * @brief Allocate buffer and read all file to it
 *
 * @warning You must free returned buffer
 * @param stream
 * @param size
 * @return void*
 *
 * @api
 */
extern void * (*raw_vfs_stream_read_all)(FileStream * stream, u64 * size);

/**
 * @brief Write to file stream n bytes from buffer
 * @warning Only for 'res' scope
 *
 * @param stream
 * @param buffer
 * @param size
 *
 * @return u64 Number of bytes written. 0 if fail
 *
 * @api
 */
extern u64 (*raw_vfs_stream_write)(FileStream * stream, void * buffer, u64 size);

/**
 * @brief Seek file stream
 *
 * @param stream
 * @param whence
 * @param offset
 * @return true if success
 * @return false if fail
 *
 * @api
 */
extern boolean (*raw_vfs_stream_seek)(FileStream * stream, FSSeekFrom whence, u64 offset);

/**
 * @brief Get file stream current position
 *
 * @param stream
 * @param success Can be NULL
 * @return u64
 *
 * @api
 */
extern u64 (*raw_vfs_stream_tell)(FileStream * stream, boolean * success);

/**
 * @brief Flush file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 * @api
 */
extern boolean (*raw_vfs_stream_flush)(FileStream * stream);

/**
 * @brief Close file stream
 *
 * @param stream
 * @api
 */
extern void (*raw_vfs_stream_close)(FileStream * stream);

/**
 * @brief Register a backend
 * @return "InvalidArgument" if name is NULL or backend is NULL
 * @return "AlreadyExists" if a backend with the same name is already registered
 *
 * @api
 */
extern boolean (*raw_window_server_register_backend)(const char * name, WindowServerBackend * backend);

/**
 * @brief Load a backend. First you should register them via window_server_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if name is NULL
 * @return "NotFound" if a backend with the given name is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
extern boolean (*raw_window_server_load_backend)(const char * name);

/**
 * @brief Create a new WindowServerBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
extern WindowServerBackend * (*raw_window_server_backend_new)(void);

/**
 * @brieif Free a WindowServerBackend instance
 * @return "InvalidArgument" if backned is NULL
 *
 * @api
 */
extern boolean (*raw_window_server_backend_free)(WindowServerBackend * backend);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
extern boolean (*raw_window_server_backend_set_function)(WindowServerBackend * backend, const char * name, fptr function);

/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 *
 * @api
 */
extern fptr (*raw_window_server_backend_get_function)(WindowServerBackend * backend, const char * name);

/**
 * @brief Register a backend
 * @return "InvalidArgument" if render_server_name is NULL or window_server_name is NULL or backend is
 * NULL
 * @return "AlreadyExists" if a backend with the same render_server_name and window_server_name is
 * already registered
 *
 * @api
 */
extern boolean (*raw_render_context_register_backend)(const char * render_server_name, const char * window_server_name, RenderContextBackend * backend);

/**
 * @brief Load a backend. First you should register them via render_context_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if render_server_name is NULL or window_server_name
 * @return "NotFound" if a backend with the given names is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
extern boolean (*raw_render_context_load_backend)(const char * render_server_name, const char * window_server_name);

/**
 * @brief Create a new RenderContextBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
extern RenderContextBackend * (*raw_render_context_backend_new)(void);

/**
 * @brieif Free a RenderContextBackend instance
 * @return "InvalidArgument" if backned is NULL
 *
 * @api
 */
extern boolean (*raw_render_context_backend_free)(RenderContextBackend * backend);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
extern boolean (*raw_render_context_backend_set_function)(RenderContextBackend * backend, const char * name, fptr function);

/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 *
 * @api
 */
extern fptr (*raw_render_context_backend_get_function)(RenderContextBackend * backend, const char * name);

/**
 * @brief Register a backend
 * @return "InvalidArgument" if name is NULL or backend is NULL
 * @return "AlreadyExists" if a backend with the same name is already registered
 *
 * @api
 */
extern boolean (*raw_render_server_register_backend)(const char * name, RenderServerBackend * backend);

/**
 * @brief Load a backend. First you should register them via render_server_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if name is NULL
 * @return "NotFound" if a backend with the given name is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
extern boolean (*raw_render_server_load_backend)(const char * name);

/**
 * @brief Create a new RenderServerBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
extern RenderServerBackend * (*raw_render_server_backend_new)(void);

/**
 * @brieif Free a RenderServerBackend instance
 * @return "InvalidArgument" if backned is NULL
 *
 * @api
 */
extern boolean (*raw_render_server_backend_free)(RenderServerBackend * backend);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
extern boolean (*raw_render_server_backend_set_function)(RenderServerBackend * backend, const char * name, fptr function);

/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 *
 * @api
 */
extern fptr (*raw_render_server_backend_get_function)(RenderServerBackend * backend, const char * name);

extern WindowServerWindow * (*raw_window_server_create_window)(const char * title, IVec2 size, WindowServerWindow * parent);

extern boolean (*raw_window_server_destroy_window)(WindowServerWindow * this);

extern boolean (*raw_window_server_window_set_title)(WindowServerWindow * this, const char * title);

extern c_str (*raw_window_server_window_get_title)(WindowServerWindow * this);

extern boolean (*raw_window_server_window_set_mode)(WindowServerWindow * this, WindowServerWindowMode mode);

extern WindowServerWindowMode (*raw_window_server_window_get_mode)(WindowServerWindow * this);

extern boolean (*raw_window_server_window_set_size)(WindowServerWindow * this, IVec2 dimensions);

extern boolean (*raw_window_server_window_get_size)(WindowServerWindow * this, IVec2 * out);

extern boolean (*raw_window_server_window_set_position)(WindowServerWindow * this, IVec2 dimensions);

extern boolean (*raw_window_server_window_get_position)(WindowServerWindow * this, IVec2 * out);

extern RenderContextSurface * (*raw_render_context_create_surface)(WindowServerWindow * window);

extern boolean (*raw_render_context_destroy_surface)(RenderContextSurface * surface);

extern boolean (*raw_render_context_surface_make_current)(RenderContextSurface * surface);

extern boolean (*raw_render_context_surface_present)(RenderContextSurface * surface);

extern fptr (*raw_render_context_get_proc_addr)(const char * proc);



#if !defined(HEAPI_FULL_TRACE)
    /**
 * @brief Set a last error that occurred on the current thread.
 *
 * @api
 */
extern void (*set_error)(Error err);

/**
 * @brief Retrieve a last error that occurred on the current thread.
 *
 * @api
 */
extern Error (*get_error)(void);

/**
 * @brief Inner function to update the full trace info. Used for tracing with HEAPI_FULL_TRACE. Don't use
 directly
 *
 * @param func function_name, "" for reset
 * @param file "" for reset
 * @param line -1 for reset

 * @api
 */
extern void (*__he_update_full_trace_info)(const char * func, const char * file, i32 line);

/**
 * @brief Create time instance and init with current time
 * @warning You should free datetime_handle with 'time_free'
 *
 * @api
 */
extern datetime_handle (*datetime_new)(void);

/**
 * @brief free datetime_handle
 *
 * @api
 */
extern void (*datetime_free)(datetime_handle handle);

/**
 * @brief Update existed time instance
 *
 * @api
 */
extern void (*datetime_update)(datetime_handle handle);

/**
 * @brief get local year
 *
 * @api
 */
extern u16 (*datetime_get_year)(datetime_handle handle);

/**
 * @brief get local month
 *
 * @api
 */
extern u8 (*datetime_get_month)(datetime_handle handle);

/**
 * @brief get local day
 *
 * @api
 */
extern u8 (*datetime_get_day)(datetime_handle handle);

/**
 * @brief get local hour
 *
 * @api
 */
extern u8 (*datetime_get_hour)(datetime_handle handle);

/**
 * @brief get local minute
 *
 * @api
 */
extern u8 (*datetime_get_minute)(datetime_handle handle);

/**
 * @brief get local second
 *
 * @api
 */
extern u8 (*datetime_get_second)(datetime_handle handle);

/**
 * @brief get local nanosecond
 *
 * @api
 */
extern u32 (*datetime_get_nanosecond)(datetime_handle handle);

/**
 * @brief get UTC year
 *
 * @api
 */
extern u16 (*datetime_get_utc_year)(datetime_handle handle);

/**
 * @brief get UTC month
 *
 * @api
 */
extern u8 (*datetime_get_utc_month)(datetime_handle handle);

/**
 * @brief get UTC day
 *
 * @api
 */
extern u8 (*datetime_get_utc_day)(datetime_handle handle);

/**
 * @brief get UTC hour
 *
 * @api
 */
extern u8 (*datetime_get_utc_hour)(datetime_handle handle);

/**
 * @brief get UTC minute
 *
 * @api
 */
extern u8 (*datetime_get_utc_minute)(datetime_handle handle);

/**
 * @brief get UTC second
 *
 * @api
 */
extern u8 (*datetime_get_utc_second)(datetime_handle handle);

/**
 * @brief Malloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param size
 * @return void*
 *
 * @api
 */
extern void * (*tmalloc)(u64 size);

/**
 * @brief Realloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 * @param size
 * @return void*
 *
 * @api
 */
extern void * (*trealloc)(void * ptr, u64 size);

/**
 * @brief Free with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 *
 * @api
 */
extern void (*tfree)(void * ptr);

/**
 * @brief Get allocated memory
 *
 * @return size in bytes if compiled with HE_MEM_TRACK, 0 otherwise
 *
 * @api
 */
extern u64 (*get_allocated_memory)(void);

/**
 * @brief Create a new mutex
 * @return Mutex handle or NULL on failure
 * @warning Must be freed with mutex_free()
 * @api
 */
extern mutex_handle (*mutex_new)(void);

/**
 * @brief Create a new mutex which allows same thread to lock multiple times
 * @return Mutex handle or NULL on failure
 * @warning Must be freed with mutex_free()
 * @api
 */
extern mutex_handle (*mutex_new_recursive)(void);

/**
 * @brief Free a mutex
 * @param handle Valid mutex handle
 * @warning Behavior undefined if mutex is locked
 * @api
 */
extern void (*mutex_free)(mutex_handle handle);

/**
 * @brief Lock a mutex (blocking)
 * @param handle Valid mutex handle
 * @warning Undefined behavior if handle is NULL
 * @api
 */
extern void (*mutex_lock)(mutex_handle handle);

/**
 * @brief Try to lock a mutex without blocking
 * @param handle Valid mutex handle
 * @return true if locked successfully, false if already locked
 * @api
 */
extern boolean (*mutex_try_lock)(mutex_handle handle);

/**
 * @brief Unlock a mutex
 * @param handle Valid mutex handle
 * @warning Must be called by the thread that locked it
 * @api
 */
extern void (*mutex_unlock)(mutex_handle handle);

/**
 * @brief Free object by type
 *
 * @param object
 * @api
 */
extern void (*auto_free)(Object * object);

/**
 * @api
 */
extern Node * (*node_new)(const char * name);

/**
 * @api
 */
extern Node * (*from_node)(Node * node);

/**
 * @api
 */
extern void (*node_set_name)(Node * node, const char * name);

/**
 * @api
 */
extern const char * (*node_get_name)(Node * node);

/**
 * @brief Add a child node to a parent node
 * @warning Take ownership of child. Do not free it!
 * @param node
 * @param child
 *
 * @api
 */
extern void (*node_add_child)(Node * node, Node * child);

/**
 * @brief Remove and free a child node by pointer from a parent node
 *
 * @param node
 * @param child
 * @return true if child was removed
 * @return false if child was not found
 *
 * @api
 */
extern boolean (*node_remove_child)(Node * node, Node * child);

/**
 * @brief Remove and free a child node by name from a parent node
 *
 * @param node
 * @param name
 * @return true if child was removed
 * @return false if child was not found
 *
 * @api
 */
extern boolean (*node_remove_child_by_name)(Node * node, const char * name);

/**
 * @brief Remove and free a child node by uid from a parent node
 *
 * @param node
 * @param uid
 * @return true if child was removed
 * @return false if child was not found
 *
 * @api
 */
extern boolean (*node_remove_child_by_uid)(Node * node, UID uid);

/**
 * @api
 */
extern boolean (*node_remove_all_children)(Node * node);

/**
 * @brief Get a child node by name
 *
 * @param node
 * @param name
 * @return Pointer to found child, NULL if not found
 *
 * @api
 */
extern Node * (*node_get_child_by_name)(Node * node, const char * name);

/**
 * @api
 */
extern Node * (*node_get_child_by_uid)(Node * node, UID uid);

/**
 * @brief
 *
 * @api
 */
extern Node3D * (*node3d_new)(const char * name);

/**
 * @param name Node name
 * @param title Window title
 * @param w Width
 * @param h Height
 *
 * @api
 */
extern Window * (*window_new)(const char * name, const char * title, i32 w, i32 h);

/**
 * @brief
 *
 * @api
 */
extern void (*window_set_title)(Window * this, const char * title);

/**
 * @brief
 *
 * @api
 */
extern const char * (*window_get_title)(const Window * this);

/**
 * @api
 */
extern CanvasItem * (*canvas_item_new)(const char * name);

/**
 * @brief
 *
 * @api
 */
extern Control * (*control_new)(const char * name);

/**
 * @brief
 *
 * @api
 */
extern Viewport * (*viewport_new)(const char * name);

/**
 * @brief
 *
 * @param m00
 * @param m01
 * @param m02
 * @param m03
 * @param m10
 * @param m11
 * @param m12
 * @param m13
 * @param m20
 * @param m21
 * @param m22
 * @param m23
 * @param m30
 * @param m31
 * @param m32
 * @param m33
 * @param [out] Result Mat4
 *
 * @api
 */
extern void (*mat4_init)(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33, Mat4 *const out);

/**
 * @brief
 *
 * @param array
 * @return Mat4
 *
 * @api
 */
extern void (*mat4_from_array)(const float *const array, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
extern void (*mat4_clone)(const Mat4 *const a, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
extern void (*mat4_add)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
extern void (*mat4_sub)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
extern void (*mat4_mul)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @param factor
 * @return Mat4
 *
 * @api
 */
extern void (*mat4_scale)(const Mat4 *const a, const float factor, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
extern void (*mat4_transpose)(const Mat4 *const a, Mat4 *const out);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
extern void (*mat4_inverse)(const Mat4 *const a, Mat4 *const out);

/**
 * @api
 */
extern void (*ivec2_init)(const i32 x, const i32 y, IVec2 *const out);

/**
 * @api
 */
extern void (*ivec2_add)(const IVec2 *const a, const IVec2 *const b, IVec2 *const out);

/**
 * @api
 */
extern void (*ivec2_sub)(const IVec2 *const a, const IVec2 *const b, IVec2 *const out);

/**
 * @api
 */
extern void (*ivec2_scale)(const IVec2 *const a, const float factor, IVec2 *const out);

/**
 * @api
 */
extern float (*ivec2_dot)(const IVec2 *const a, const IVec2 *const b);

/**
 * @api
 */
extern float (*ivec2_length)(const IVec2 *const a);

/**
 * @api
 */
extern void (*ivec2_normalize)(const IVec2 *const a, IVec2 *const out);

/**
 * @api
 */
extern float (*ivec2_distance)(const IVec2 *const a, const IVec2 *const b);

/**
 * @brief Add b to a
 *
 * @param[in/out] a
 * @param[in] b
 *
 * @api
 */
extern void (*ivec2_add_in)(IVec2 *const to, const IVec2 *const what);

/**
 * @brief
 *
 * @param[in/out] from
 * @param[in] what
 *
 * @api
 */
extern void (*ivec2_sub_in)(IVec2 *const from, const IVec2 *const what);

/**
 * @brief
 *
 * @param[in/out] to
 * @param[in] factor
 *
 * @api
 */
extern void (*ivec2_scale_in)(IVec2 *const to, const float factor);

/**
 * @brief
 *
 * @param[in/out] a
 *
 * @api
 */
extern void (*ivec2_normalize_in)(IVec2 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*ivec3_init)(i32 x, i32 y, i32 z, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*ivec3_add)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*ivec3_sub)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*ivec3_scale)(const IVec3 *const a, const float factor, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*ivec3_cross)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*ivec3_dot)(const IVec3 *const a, const IVec3 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*ivec3_length)(const IVec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*ivec3_normalize)(const IVec3 *const a, IVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*ivec3_distance)(const IVec3 *const a, const IVec3 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*ivec3_add_in)(IVec3 *const to, const IVec3 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*ivec3_sub_in)(IVec3 *const from, const IVec3 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*ivec3_scale_in)(IVec3 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*ivec3_normalize_in)(IVec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*ivec4_init)(i32 x, i32 y, i32 z, i32 w, IVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*ivec4_add)(const IVec4 *const a, const IVec4 *const b, IVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*ivec4_sub)(const IVec4 *const a, const IVec4 *const b, IVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*ivec4_scale)(const IVec4 *const a, const float factor, IVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*ivec4_dot)(const IVec4 *const a, const IVec4 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*ivec4_length)(const IVec4 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*ivec4_normalize)(const IVec4 *const a, IVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*ivec4_distance)(const IVec4 *const a, const IVec4 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*ivec4_add_in)(IVec4 *const to, const IVec4 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*ivec4_sub_in)(IVec4 *const from, const IVec4 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*ivec4_scale_in)(IVec4 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*ivec4_normalize_in)(IVec4 *const a);

/**
 * @api
 */
extern void (*uvec2_init)(const u32 x, const u32 y, UVec2 *const out);

/**
 * @api
 */
extern void (*uvec2_add)(const UVec2 *const a, const UVec2 *const b, UVec2 *const out);

/**
 * @api
 */
extern void (*uvec2_sub)(const UVec2 *const a, const UVec2 *const b, UVec2 *const out);

/**
 * @api
 */
extern void (*uvec2_scale)(const UVec2 *const a, const float factor, UVec2 *const out);

/**
 * @api
 */
extern float (*uvec2_dot)(const UVec2 *const a, const UVec2 *const b);

/**
 * @api
 */
extern float (*uvec2_length)(const UVec2 *const a);

/**
 * @api
 */
extern void (*uvec2_normalize)(const UVec2 *const a, UVec2 *const out);

/**
 * @api
 */
extern float (*uvec2_distance)(const UVec2 *const a, const UVec2 *const b);

/**
 * @brief Add b to a
 *
 * @param[in/out] a
 * @param[in] b
 *
 * @api
 */
extern void (*uvec2_add_in)(UVec2 *const to, const UVec2 *const what);

/**
 * @brief
 *
 * @param[in/out] from
 * @param[in] what
 *
 * @api
 */
extern void (*uvec2_sub_in)(UVec2 *const from, const UVec2 *const what);

/**
 * @brief
 *
 * @param[in/out] to
 * @param[in] factor
 *
 * @api
 */
extern void (*uvec2_scale_in)(UVec2 *const to, const float factor);

/**
 * @brief
 *
 * @param[in/out] a
 *
 * @api
 */
extern void (*uvec2_normalize_in)(UVec2 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*uvec3_init)(u32 x, u32 y, u32 z, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*uvec3_add)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*uvec3_sub)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*uvec3_scale)(const UVec3 *const a, const float factor, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*uvec3_cross)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*uvec3_dot)(const UVec3 *const a, const UVec3 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*uvec3_length)(const UVec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*uvec3_normalize)(const UVec3 *const a, UVec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*uvec3_distance)(const UVec3 *const a, const UVec3 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*uvec3_add_in)(UVec3 *const to, const UVec3 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*uvec3_sub_in)(UVec3 *const from, const UVec3 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*uvec3_scale_in)(UVec3 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*uvec3_normalize_in)(UVec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*uvec4_init)(u32 x, u32 y, u32 z, u32 w, UVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*uvec4_add)(const UVec4 *const a, const UVec4 *const b, UVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*uvec4_sub)(const UVec4 *const a, const UVec4 *const b, UVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*uvec4_scale)(const UVec4 *const a, const float factor, UVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*uvec4_dot)(const UVec4 *const a, const UVec4 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*uvec4_length)(const UVec4 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*uvec4_normalize)(const UVec4 *const a, UVec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*uvec4_distance)(const UVec4 *const a, const UVec4 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*uvec4_add_in)(UVec4 *const to, const UVec4 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*uvec4_sub_in)(UVec4 *const from, const UVec4 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*uvec4_scale_in)(UVec4 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*uvec4_normalize_in)(UVec4 *const a);

/**
 * @api
 */
extern void (*vec2_init)(const float x, const float y, Vec2 *const out);

/**
 * @api
 */
extern void (*vec2_add)(const Vec2 *const a, const Vec2 *const b, Vec2 *const out);

/**
 * @api
 */
extern void (*vec2_sub)(const Vec2 *const a, const Vec2 *const b, Vec2 *const out);

/**
 * @api
 */
extern void (*vec2_scale)(const Vec2 *const a, const float factor, Vec2 *const out);

/**
 * @api
 */
extern float (*vec2_dot)(const Vec2 *const a, const Vec2 *const b);

/**
 * @api
 */
extern float (*vec2_length)(const Vec2 *const a);

/**
 * @api
 */
extern void (*vec2_normalize)(const Vec2 *const a, Vec2 *const out);

/**
 * @api
 */
extern float (*vec2_distance)(const Vec2 *const a, const Vec2 *const b);

/**
 * @brief Add b to a
 *
 * @param[in/out] a
 * @param[in] b
 *
 * @api
 */
extern void (*vec2_add_in)(Vec2 *const to, const Vec2 *const what);

/**
 * @brief
 *
 * @param[in/out] from
 * @param[in] what
 *
 * @api
 */
extern void (*vec2_sub_in)(Vec2 *const from, const Vec2 *const what);

/**
 * @brief
 *
 * @param[in/out] to
 * @param[in] factor
 *
 * @api
 */
extern void (*vec2_scale_in)(Vec2 *const to, const float factor);

/**
 * @brief
 *
 * @param[in/out] a
 *
 * @api
 */
extern void (*vec2_normalize_in)(Vec2 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*vec3_init)(float x, float y, float z, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*vec3_add)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*vec3_sub)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*vec3_scale)(const Vec3 *const a, const float factor, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*vec3_cross)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*vec3_dot)(const Vec3 *const a, const Vec3 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*vec3_length)(const Vec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*vec3_normalize)(const Vec3 *const a, Vec3 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*vec3_distance)(const Vec3 *const a, const Vec3 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*vec3_add_in)(Vec3 *const to, const Vec3 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*vec3_sub_in)(Vec3 *const from, const Vec3 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*vec3_scale_in)(Vec3 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*vec3_normalize_in)(Vec3 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*vec4_init)(float x, float y, float z, float w, Vec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*vec4_add)(const Vec4 *const a, const Vec4 *const b, Vec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*vec4_sub)(const Vec4 *const a, const Vec4 *const b, Vec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern void (*vec4_scale)(const Vec4 *const a, const float factor, Vec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*vec4_dot)(const Vec4 *const a, const Vec4 *const b);

/**
 * @brief
 *
 * @api
 */
extern float (*vec4_length)(const Vec4 *const a);

/**
 * @brief
 *
 * @api
 */
extern void (*vec4_normalize)(const Vec4 *const a, Vec4 *const out);

/**
 * @brief
 *
 * @api
 */
extern float (*vec4_distance)(const Vec4 *const a, const Vec4 *const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
extern void (*vec4_add_in)(Vec4 *const to, const Vec4 *const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
extern void (*vec4_sub_in)(Vec4 *const from, const Vec4 *const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
extern void (*vec4_scale_in)(Vec4 *const to, const float factor);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
extern void (*vec4_normalize_in)(Vec4 *const a);

/**
 * @api
 */
extern Error (*string_new)(string ** str);

/**
 * @api
 */
extern string * (*string_from)(const char * c_str);

/**
 * @api
 */
extern string * (*string_clone)(const string * c_str);

/**
 * @api
 */
extern string * (*string_set_cstr)(string * self, const char * c_cstr);

/**
 * @api
 */
extern string * (*string_set)(string * self, const string * c_str);

/**
 * @api
 */
extern char * (*string_cstr)(const string * c_str);

/**
 * @api
 */
extern usize (*string_len)(const string * c_str);

/**
 * @api
 */
extern usize (*string_size)(const string * c_str);

/**
 * @api
 */
extern string * (*string_push_back)(string * dest, const string * src);

/**
 * @api
 */
extern string * (*string_push_back_cstr)(string * dest, const char * src);

/**
 * @api
 */
extern string * (*string_push_front)(string * dest, const string * src);

/**
 * @api
 */
extern string * (*string_push_front_cstr)(string * dest, const char * src);

/**
 * @api
 */
extern string * (*string_insert)(string * dest, const string * src, const usize i);

/**
 * @api
 */
extern string * (*string_insert_cstr)(string * dest, const char * src, const usize i);

/**
 * @api
 */
extern string * (*string_insert_cstr_ex)(string * dest, const char * src, const usize i);

/**
 * @api
 */
extern string * (*string_insert_cstr_by_byte)(string * dest, const char * src, const usize b);

/**
 * @api
 */
extern string * (*string_insert_by_byte)(string * dest, const string * src, const usize b);

/**
 * @api
 */
extern string * (*string_remove)(string * self, const usize i);

/**
 * @api
 */
extern string * (*string_remove_by_byte)(string * self, const usize b);

/**
 * @api
 */
extern string * (*string_remove_n)(string * self, const usize i, const usize n);

/**
 * @api
 */
extern string * (*string_remove_n_by_byte)(string * self, const usize b, const usize b_n);

/**
 * @api
 */
extern boolean (*string_equals)(const string * str1, const string * str2);

/**
 * @api
 */
extern boolean (*string_equals_cstr)(const string * str, const char * cstr);

/**
 * @api
 */
extern void (*string_free)(string * str);

/**
 * @api
 */
extern void (*string_itr_free)(string_itr * str_itr);

/**
 * @api
 */
extern string_itr * (*string_get_itr)(const string * c_str);

/**
 * @api
 */
extern u8 (*string_itr_next)(string_itr * str_itr);

/**
 * @api
 */
extern string_slice * (*string_get_slice)(const string * str, const usize s, const usize e);

/**
 * @api
 */
extern string * (*string_from_slice)(const string_slice * str_sl);

/**
 * @api
 */
extern boolean (*string_equals_slice)(string_slice * str_sl_1, string_slice * str_sl_2);

/**
 * @api
 */
extern string_slice * (*string_set_slice)(string_slice * self, const string_slice * str_sl);

/**
 * @api
 */
extern string * (*string_push_back_slice)(string * dest, const string_slice * src);

/**
 * @api
 */
extern string * (*string_push_front_slice)(string * dest, const string_slice * src);

/**
 * @api
 */
extern string * (*string_insert_slice_ex)(string * dest, const string_slice * src, const usize i);

/**
 * @api
 */
extern string * (*string_insert_slice)(string * dest, const string_slice * src, const usize i);

/**
 * @api
 */
extern string * (*string_insert_slice_by_byte)(string * dest, const string_slice * src, const usize b);

/**
 * @api
 */
extern void (*string_slice_free)(string_slice * str_sl);

/**
 * @api
 */
extern Error (*string_utf8_new)(string_utf8 ** str);

/**
 * @api
 */
extern Error (*string_utf8_to_string)(string ** dest, const string_utf8 * str);

/**
 * @api
 */
extern Error (*string_utf8_from)(string_utf8 ** str, const char * c_str);

/**
 * @api
 */
extern Error (*string_utf8_len)(usize * len, const string_utf8 * c_str);

/**
 * @api
 */
extern Error (*string_utf8_size)(usize * size, const string_utf8 * c_str);

/**
 * @api
 */
extern Error (*string_utf8_clone)(string_utf8 ** str, const string_utf8 * c_str);

/**
 * @api
 */
extern Error (*string_utf8_push_back_cstr)(string_utf8 * dest, const char * src);

/**
 * @api
 */
extern Error (*string_utf8_push_back)(string_utf8 * dest, const string_utf8 * src);

/**
 * @api
 */
extern Error (*string_utf8_insert)(string_utf8 * dest, const string_utf8 * src, const usize i);

/**
 * @api
 */
extern Error (*string_utf8_push_front)(string_utf8 * dest, const string_utf8 * src);

/**
 * @api
 */
extern void (*string_utf8_free)(string_utf8 * str);

/**
 * @brief
 *
 * @api
 */
extern UID (*uid_new)(void);

/**
 * @brief Mount Resource File (*.hefs) to Virtual File System
 *
 * @param path Path to resource file
 * @param mount_point Mount point
 * @return true
 * @return false
 *
 * @api
 */
extern boolean (*vfs_mount_res)(const char * path, const char * mount_point);

/**
 * @brief Unmount Resource File from Virtual File System
 *
 * @param mount_point Mount point
 * @return true
 * @return false
 *
 * @api
 */
extern boolean (*vfs_unmount_res)(const char * mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res
 *
 * @return true if mounted successfully
 * @return false if mount failed
 *
 * @api
 */
extern boolean (*vfs_mount_rfs)(const char * mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res only with allowed files in whitelist
 *
 * @return true if mounted successfully
 * @return false if mount failed
 *
 * @api
 */
extern boolean (*vfs_mount_rfs_whitelist)(const char ** whitelist, u64 count, const char * mount_point);

/**
 * @brief Unmount Real File System from Virtual File System Res
 *
 * @return true if unmounted successfully
 * @return false if unmount failed
 *
 * @api
 */
extern boolean (*vfs_unmount_rfs)(void);

/**
 * @brief Check if file exists
 *
 * @param path
 * @return true if exists
 * @return false if not exists
 *
 * @api
 */
extern boolean (*vfs_res_path_exists)(const char * path);

/**
 * @brief Check if file exists in User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return true if exists
 * @return false if not exists
 *
 * @api
 */
extern boolean (*vfs_usr_path_exists)(const char * path, boolean prefer_res);

/**
 * @brief Allocate memory and read file from Reosource scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @return Pointer to allocated memory with file data if success, else NULL
 *
 * @api
 */
extern void * (*vfs_res_read_file)(const char * path, u64 * size);

/**
 * @brief Open file stream from Resource scope
 *
 * @param path
 * @return FileStream* If success, else NULL
 *
 * @api
 */
extern FileStream * (*vfs_res_stream_open)(const char * path);

/**
 * @brief Allocate memory and read file from User scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @param prefer_res If true, try to read from Resource scope first
 * @return Pointer to allocated memory with file data if success, else NULL
 *
 * @api
 */
extern void * (*vfs_usr_read_file)(const char * path, u64 * size, boolean prefer_res);

/**
 * @brief Write file to User scope
 *
 * @param path
 * @param data
 * @param size
 * @return true if success
 * @return false if fail
 *
 * @api
 */
extern boolean (*vfs_usr_write_file)(const char * path, const void * data, u64 size);

/**
 * @brief Open file stream from User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return FileStream* If success, else NULL
 *
 * @api
 */
extern FileStream * (*vfs_usr_stream_open)(const char * path, boolean prefer_res);

/**
 * @brief Get file stream size in bytes
 *
 * @param path
 * @return u64
 *
 * @api
 */
extern u64 (*vfs_stream_size)(FileStream * stream);

/**
 * @brief Read from file stream n bytes to buffer
 *
 * @param stream
 * @param buffer
 * @param size
 * @return u64
 *
 * @api
 */
extern u64 (*vfs_stream_read_n)(FileStream * stream, void * buffer, u64 size);

/**
 * @brief Allocate buffer and read all file to it
 *
 * @warning You must free returned buffer
 * @param stream
 * @param size
 * @return void*
 *
 * @api
 */
extern void * (*vfs_stream_read_all)(FileStream * stream, u64 * size);

/**
 * @brief Write to file stream n bytes from buffer
 * @warning Only for 'res' scope
 *
 * @param stream
 * @param buffer
 * @param size
 *
 * @return u64 Number of bytes written. 0 if fail
 *
 * @api
 */
extern u64 (*vfs_stream_write)(FileStream * stream, void * buffer, u64 size);

/**
 * @brief Seek file stream
 *
 * @param stream
 * @param whence
 * @param offset
 * @return true if success
 * @return false if fail
 *
 * @api
 */
extern boolean (*vfs_stream_seek)(FileStream * stream, FSSeekFrom whence, u64 offset);

/**
 * @brief Get file stream current position
 *
 * @param stream
 * @param success Can be NULL
 * @return u64
 *
 * @api
 */
extern u64 (*vfs_stream_tell)(FileStream * stream, boolean * success);

/**
 * @brief Flush file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 * @api
 */
extern boolean (*vfs_stream_flush)(FileStream * stream);

/**
 * @brief Close file stream
 *
 * @param stream
 * @api
 */
extern void (*vfs_stream_close)(FileStream * stream);

/**
 * @brief Register a backend
 * @return "InvalidArgument" if name is NULL or backend is NULL
 * @return "AlreadyExists" if a backend with the same name is already registered
 *
 * @api
 */
extern boolean (*window_server_register_backend)(const char * name, WindowServerBackend * backend);

/**
 * @brief Load a backend. First you should register them via window_server_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if name is NULL
 * @return "NotFound" if a backend with the given name is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
extern boolean (*window_server_load_backend)(const char * name);

/**
 * @brief Create a new WindowServerBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
extern WindowServerBackend * (*window_server_backend_new)(void);

/**
 * @brieif Free a WindowServerBackend instance
 * @return "InvalidArgument" if backned is NULL
 *
 * @api
 */
extern boolean (*window_server_backend_free)(WindowServerBackend * backend);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
extern boolean (*window_server_backend_set_function)(WindowServerBackend * backend, const char * name, fptr function);

/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 *
 * @api
 */
extern fptr (*window_server_backend_get_function)(WindowServerBackend * backend, const char * name);

/**
 * @brief Register a backend
 * @return "InvalidArgument" if render_server_name is NULL or window_server_name is NULL or backend is
 * NULL
 * @return "AlreadyExists" if a backend with the same render_server_name and window_server_name is
 * already registered
 *
 * @api
 */
extern boolean (*render_context_register_backend)(const char * render_server_name, const char * window_server_name, RenderContextBackend * backend);

/**
 * @brief Load a backend. First you should register them via render_context_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if render_server_name is NULL or window_server_name
 * @return "NotFound" if a backend with the given names is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
extern boolean (*render_context_load_backend)(const char * render_server_name, const char * window_server_name);

/**
 * @brief Create a new RenderContextBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
extern RenderContextBackend * (*render_context_backend_new)(void);

/**
 * @brieif Free a RenderContextBackend instance
 * @return "InvalidArgument" if backned is NULL
 *
 * @api
 */
extern boolean (*render_context_backend_free)(RenderContextBackend * backend);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
extern boolean (*render_context_backend_set_function)(RenderContextBackend * backend, const char * name, fptr function);

/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 *
 * @api
 */
extern fptr (*render_context_backend_get_function)(RenderContextBackend * backend, const char * name);

/**
 * @brief Register a backend
 * @return "InvalidArgument" if name is NULL or backend is NULL
 * @return "AlreadyExists" if a backend with the same name is already registered
 *
 * @api
 */
extern boolean (*render_server_register_backend)(const char * name, RenderServerBackend * backend);

/**
 * @brief Load a backend. First you should register them via render_server_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if name is NULL
 * @return "NotFound" if a backend with the given name is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
extern boolean (*render_server_load_backend)(const char * name);

/**
 * @brief Create a new RenderServerBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
extern RenderServerBackend * (*render_server_backend_new)(void);

/**
 * @brieif Free a RenderServerBackend instance
 * @return "InvalidArgument" if backned is NULL
 *
 * @api
 */
extern boolean (*render_server_backend_free)(RenderServerBackend * backend);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
extern boolean (*render_server_backend_set_function)(RenderServerBackend * backend, const char * name, fptr function);

/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 *
 * @api
 */
extern fptr (*render_server_backend_get_function)(RenderServerBackend * backend, const char * name);

extern WindowServerWindow * (*window_server_create_window)(const char * title, IVec2 size, WindowServerWindow * parent);

extern boolean (*window_server_destroy_window)(WindowServerWindow * this);

extern boolean (*window_server_window_set_title)(WindowServerWindow * this, const char * title);

extern c_str (*window_server_window_get_title)(WindowServerWindow * this);

extern boolean (*window_server_window_set_mode)(WindowServerWindow * this, WindowServerWindowMode mode);

extern WindowServerWindowMode (*window_server_window_get_mode)(WindowServerWindow * this);

extern boolean (*window_server_window_set_size)(WindowServerWindow * this, IVec2 dimensions);

extern boolean (*window_server_window_get_size)(WindowServerWindow * this, IVec2 * out);

extern boolean (*window_server_window_set_position)(WindowServerWindow * this, IVec2 dimensions);

extern boolean (*window_server_window_get_position)(WindowServerWindow * this, IVec2 * out);

extern RenderContextSurface * (*render_context_create_surface)(WindowServerWindow * window);

extern boolean (*render_context_destroy_surface)(RenderContextSurface * surface);

extern boolean (*render_context_surface_make_current)(RenderContextSurface * surface);

extern boolean (*render_context_surface_present)(RenderContextSurface * surface);

extern fptr (*render_context_get_proc_addr)(const char * proc);


#endif

#if defined(HEAPI_LOAD_IMPL)
        void (*raw_set_error)(Error err);
    Error (*raw_get_error)(void);
    void (*raw___he_update_full_trace_info)(const char * func, const char * file, i32 line);
    datetime_handle (*raw_datetime_new)(void);
    void (*raw_datetime_free)(datetime_handle handle);
    void (*raw_datetime_update)(datetime_handle handle);
    u16 (*raw_datetime_get_year)(datetime_handle handle);
    u8 (*raw_datetime_get_month)(datetime_handle handle);
    u8 (*raw_datetime_get_day)(datetime_handle handle);
    u8 (*raw_datetime_get_hour)(datetime_handle handle);
    u8 (*raw_datetime_get_minute)(datetime_handle handle);
    u8 (*raw_datetime_get_second)(datetime_handle handle);
    u32 (*raw_datetime_get_nanosecond)(datetime_handle handle);
    u16 (*raw_datetime_get_utc_year)(datetime_handle handle);
    u8 (*raw_datetime_get_utc_month)(datetime_handle handle);
    u8 (*raw_datetime_get_utc_day)(datetime_handle handle);
    u8 (*raw_datetime_get_utc_hour)(datetime_handle handle);
    u8 (*raw_datetime_get_utc_minute)(datetime_handle handle);
    u8 (*raw_datetime_get_utc_second)(datetime_handle handle);
    void * (*raw_tmalloc)(u64 size);
    void * (*raw_trealloc)(void * ptr, u64 size);
    void (*raw_tfree)(void * ptr);
    u64 (*raw_get_allocated_memory)(void);
    mutex_handle (*raw_mutex_new)(void);
    mutex_handle (*raw_mutex_new_recursive)(void);
    void (*raw_mutex_free)(mutex_handle handle);
    void (*raw_mutex_lock)(mutex_handle handle);
    boolean (*raw_mutex_try_lock)(mutex_handle handle);
    void (*raw_mutex_unlock)(mutex_handle handle);
    void (*raw_auto_free)(Object * object);
    Node * (*raw_node_new)(const char * name);
    Node * (*raw_from_node)(Node * node);
    void (*raw_node_set_name)(Node * node, const char * name);
    const char * (*raw_node_get_name)(Node * node);
    void (*raw_node_add_child)(Node * node, Node * child);
    boolean (*raw_node_remove_child)(Node * node, Node * child);
    boolean (*raw_node_remove_child_by_name)(Node * node, const char * name);
    boolean (*raw_node_remove_child_by_uid)(Node * node, UID uid);
    boolean (*raw_node_remove_all_children)(Node * node);
    Node * (*raw_node_get_child_by_name)(Node * node, const char * name);
    Node * (*raw_node_get_child_by_uid)(Node * node, UID uid);
    Node3D * (*raw_node3d_new)(const char * name);
    Window * (*raw_window_new)(const char * name, const char * title, i32 w, i32 h);
    void (*raw_window_set_title)(Window * this, const char * title);
    const char * (*raw_window_get_title)(const Window * this);
    CanvasItem * (*raw_canvas_item_new)(const char * name);
    Control * (*raw_control_new)(const char * name);
    Viewport * (*raw_viewport_new)(const char * name);
    void (*raw_mat4_init)(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33, Mat4 *const out);
    void (*raw_mat4_from_array)(const float *const array, Mat4 *const out);
    void (*raw_mat4_clone)(const Mat4 *const a, Mat4 *const out);
    void (*raw_mat4_add)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);
    void (*raw_mat4_sub)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);
    void (*raw_mat4_mul)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);
    void (*raw_mat4_scale)(const Mat4 *const a, const float factor, Mat4 *const out);
    void (*raw_mat4_transpose)(const Mat4 *const a, Mat4 *const out);
    void (*raw_mat4_inverse)(const Mat4 *const a, Mat4 *const out);
    void (*raw_ivec2_init)(const i32 x, const i32 y, IVec2 *const out);
    void (*raw_ivec2_add)(const IVec2 *const a, const IVec2 *const b, IVec2 *const out);
    void (*raw_ivec2_sub)(const IVec2 *const a, const IVec2 *const b, IVec2 *const out);
    void (*raw_ivec2_scale)(const IVec2 *const a, const float factor, IVec2 *const out);
    float (*raw_ivec2_dot)(const IVec2 *const a, const IVec2 *const b);
    float (*raw_ivec2_length)(const IVec2 *const a);
    void (*raw_ivec2_normalize)(const IVec2 *const a, IVec2 *const out);
    float (*raw_ivec2_distance)(const IVec2 *const a, const IVec2 *const b);
    void (*raw_ivec2_add_in)(IVec2 *const to, const IVec2 *const what);
    void (*raw_ivec2_sub_in)(IVec2 *const from, const IVec2 *const what);
    void (*raw_ivec2_scale_in)(IVec2 *const to, const float factor);
    void (*raw_ivec2_normalize_in)(IVec2 *const a);
    void (*raw_ivec3_init)(i32 x, i32 y, i32 z, IVec3 *const out);
    void (*raw_ivec3_add)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);
    void (*raw_ivec3_sub)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);
    void (*raw_ivec3_scale)(const IVec3 *const a, const float factor, IVec3 *const out);
    void (*raw_ivec3_cross)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);
    float (*raw_ivec3_dot)(const IVec3 *const a, const IVec3 *const b);
    float (*raw_ivec3_length)(const IVec3 *const a);
    void (*raw_ivec3_normalize)(const IVec3 *const a, IVec3 *const out);
    float (*raw_ivec3_distance)(const IVec3 *const a, const IVec3 *const b);
    void (*raw_ivec3_add_in)(IVec3 *const to, const IVec3 *const what);
    void (*raw_ivec3_sub_in)(IVec3 *const from, const IVec3 *const what);
    void (*raw_ivec3_scale_in)(IVec3 *const to, const float factor);
    void (*raw_ivec3_normalize_in)(IVec3 *const a);
    void (*raw_ivec4_init)(i32 x, i32 y, i32 z, i32 w, IVec4 *const out);
    void (*raw_ivec4_add)(const IVec4 *const a, const IVec4 *const b, IVec4 *const out);
    void (*raw_ivec4_sub)(const IVec4 *const a, const IVec4 *const b, IVec4 *const out);
    void (*raw_ivec4_scale)(const IVec4 *const a, const float factor, IVec4 *const out);
    float (*raw_ivec4_dot)(const IVec4 *const a, const IVec4 *const b);
    float (*raw_ivec4_length)(const IVec4 *const a);
    void (*raw_ivec4_normalize)(const IVec4 *const a, IVec4 *const out);
    float (*raw_ivec4_distance)(const IVec4 *const a, const IVec4 *const b);
    void (*raw_ivec4_add_in)(IVec4 *const to, const IVec4 *const what);
    void (*raw_ivec4_sub_in)(IVec4 *const from, const IVec4 *const what);
    void (*raw_ivec4_scale_in)(IVec4 *const to, const float factor);
    void (*raw_ivec4_normalize_in)(IVec4 *const a);
    void (*raw_uvec2_init)(const u32 x, const u32 y, UVec2 *const out);
    void (*raw_uvec2_add)(const UVec2 *const a, const UVec2 *const b, UVec2 *const out);
    void (*raw_uvec2_sub)(const UVec2 *const a, const UVec2 *const b, UVec2 *const out);
    void (*raw_uvec2_scale)(const UVec2 *const a, const float factor, UVec2 *const out);
    float (*raw_uvec2_dot)(const UVec2 *const a, const UVec2 *const b);
    float (*raw_uvec2_length)(const UVec2 *const a);
    void (*raw_uvec2_normalize)(const UVec2 *const a, UVec2 *const out);
    float (*raw_uvec2_distance)(const UVec2 *const a, const UVec2 *const b);
    void (*raw_uvec2_add_in)(UVec2 *const to, const UVec2 *const what);
    void (*raw_uvec2_sub_in)(UVec2 *const from, const UVec2 *const what);
    void (*raw_uvec2_scale_in)(UVec2 *const to, const float factor);
    void (*raw_uvec2_normalize_in)(UVec2 *const a);
    void (*raw_uvec3_init)(u32 x, u32 y, u32 z, UVec3 *const out);
    void (*raw_uvec3_add)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);
    void (*raw_uvec3_sub)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);
    void (*raw_uvec3_scale)(const UVec3 *const a, const float factor, UVec3 *const out);
    void (*raw_uvec3_cross)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);
    float (*raw_uvec3_dot)(const UVec3 *const a, const UVec3 *const b);
    float (*raw_uvec3_length)(const UVec3 *const a);
    void (*raw_uvec3_normalize)(const UVec3 *const a, UVec3 *const out);
    float (*raw_uvec3_distance)(const UVec3 *const a, const UVec3 *const b);
    void (*raw_uvec3_add_in)(UVec3 *const to, const UVec3 *const what);
    void (*raw_uvec3_sub_in)(UVec3 *const from, const UVec3 *const what);
    void (*raw_uvec3_scale_in)(UVec3 *const to, const float factor);
    void (*raw_uvec3_normalize_in)(UVec3 *const a);
    void (*raw_uvec4_init)(u32 x, u32 y, u32 z, u32 w, UVec4 *const out);
    void (*raw_uvec4_add)(const UVec4 *const a, const UVec4 *const b, UVec4 *const out);
    void (*raw_uvec4_sub)(const UVec4 *const a, const UVec4 *const b, UVec4 *const out);
    void (*raw_uvec4_scale)(const UVec4 *const a, const float factor, UVec4 *const out);
    float (*raw_uvec4_dot)(const UVec4 *const a, const UVec4 *const b);
    float (*raw_uvec4_length)(const UVec4 *const a);
    void (*raw_uvec4_normalize)(const UVec4 *const a, UVec4 *const out);
    float (*raw_uvec4_distance)(const UVec4 *const a, const UVec4 *const b);
    void (*raw_uvec4_add_in)(UVec4 *const to, const UVec4 *const what);
    void (*raw_uvec4_sub_in)(UVec4 *const from, const UVec4 *const what);
    void (*raw_uvec4_scale_in)(UVec4 *const to, const float factor);
    void (*raw_uvec4_normalize_in)(UVec4 *const a);
    void (*raw_vec2_init)(const float x, const float y, Vec2 *const out);
    void (*raw_vec2_add)(const Vec2 *const a, const Vec2 *const b, Vec2 *const out);
    void (*raw_vec2_sub)(const Vec2 *const a, const Vec2 *const b, Vec2 *const out);
    void (*raw_vec2_scale)(const Vec2 *const a, const float factor, Vec2 *const out);
    float (*raw_vec2_dot)(const Vec2 *const a, const Vec2 *const b);
    float (*raw_vec2_length)(const Vec2 *const a);
    void (*raw_vec2_normalize)(const Vec2 *const a, Vec2 *const out);
    float (*raw_vec2_distance)(const Vec2 *const a, const Vec2 *const b);
    void (*raw_vec2_add_in)(Vec2 *const to, const Vec2 *const what);
    void (*raw_vec2_sub_in)(Vec2 *const from, const Vec2 *const what);
    void (*raw_vec2_scale_in)(Vec2 *const to, const float factor);
    void (*raw_vec2_normalize_in)(Vec2 *const a);
    void (*raw_vec3_init)(float x, float y, float z, Vec3 *const out);
    void (*raw_vec3_add)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);
    void (*raw_vec3_sub)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);
    void (*raw_vec3_scale)(const Vec3 *const a, const float factor, Vec3 *const out);
    void (*raw_vec3_cross)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);
    float (*raw_vec3_dot)(const Vec3 *const a, const Vec3 *const b);
    float (*raw_vec3_length)(const Vec3 *const a);
    void (*raw_vec3_normalize)(const Vec3 *const a, Vec3 *const out);
    float (*raw_vec3_distance)(const Vec3 *const a, const Vec3 *const b);
    void (*raw_vec3_add_in)(Vec3 *const to, const Vec3 *const what);
    void (*raw_vec3_sub_in)(Vec3 *const from, const Vec3 *const what);
    void (*raw_vec3_scale_in)(Vec3 *const to, const float factor);
    void (*raw_vec3_normalize_in)(Vec3 *const a);
    void (*raw_vec4_init)(float x, float y, float z, float w, Vec4 *const out);
    void (*raw_vec4_add)(const Vec4 *const a, const Vec4 *const b, Vec4 *const out);
    void (*raw_vec4_sub)(const Vec4 *const a, const Vec4 *const b, Vec4 *const out);
    void (*raw_vec4_scale)(const Vec4 *const a, const float factor, Vec4 *const out);
    float (*raw_vec4_dot)(const Vec4 *const a, const Vec4 *const b);
    float (*raw_vec4_length)(const Vec4 *const a);
    void (*raw_vec4_normalize)(const Vec4 *const a, Vec4 *const out);
    float (*raw_vec4_distance)(const Vec4 *const a, const Vec4 *const b);
    void (*raw_vec4_add_in)(Vec4 *const to, const Vec4 *const what);
    void (*raw_vec4_sub_in)(Vec4 *const from, const Vec4 *const what);
    void (*raw_vec4_scale_in)(Vec4 *const to, const float factor);
    void (*raw_vec4_normalize_in)(Vec4 *const a);
    Error (*raw_string_new)(string ** str);
    string * (*raw_string_from)(const char * c_str);
    string * (*raw_string_clone)(const string * c_str);
    string * (*raw_string_set_cstr)(string * self, const char * c_cstr);
    string * (*raw_string_set)(string * self, const string * c_str);
    char * (*raw_string_cstr)(const string * c_str);
    usize (*raw_string_len)(const string * c_str);
    usize (*raw_string_size)(const string * c_str);
    string * (*raw_string_push_back)(string * dest, const string * src);
    string * (*raw_string_push_back_cstr)(string * dest, const char * src);
    string * (*raw_string_push_front)(string * dest, const string * src);
    string * (*raw_string_push_front_cstr)(string * dest, const char * src);
    string * (*raw_string_insert)(string * dest, const string * src, const usize i);
    string * (*raw_string_insert_cstr)(string * dest, const char * src, const usize i);
    string * (*raw_string_insert_cstr_ex)(string * dest, const char * src, const usize i);
    string * (*raw_string_insert_cstr_by_byte)(string * dest, const char * src, const usize b);
    string * (*raw_string_insert_by_byte)(string * dest, const string * src, const usize b);
    string * (*raw_string_remove)(string * self, const usize i);
    string * (*raw_string_remove_by_byte)(string * self, const usize b);
    string * (*raw_string_remove_n)(string * self, const usize i, const usize n);
    string * (*raw_string_remove_n_by_byte)(string * self, const usize b, const usize b_n);
    boolean (*raw_string_equals)(const string * str1, const string * str2);
    boolean (*raw_string_equals_cstr)(const string * str, const char * cstr);
    void (*raw_string_free)(string * str);
    void (*raw_string_itr_free)(string_itr * str_itr);
    string_itr * (*raw_string_get_itr)(const string * c_str);
    u8 (*raw_string_itr_next)(string_itr * str_itr);
    string_slice * (*raw_string_get_slice)(const string * str, const usize s, const usize e);
    string * (*raw_string_from_slice)(const string_slice * str_sl);
    boolean (*raw_string_equals_slice)(string_slice * str_sl_1, string_slice * str_sl_2);
    string_slice * (*raw_string_set_slice)(string_slice * self, const string_slice * str_sl);
    string * (*raw_string_push_back_slice)(string * dest, const string_slice * src);
    string * (*raw_string_push_front_slice)(string * dest, const string_slice * src);
    string * (*raw_string_insert_slice_ex)(string * dest, const string_slice * src, const usize i);
    string * (*raw_string_insert_slice)(string * dest, const string_slice * src, const usize i);
    string * (*raw_string_insert_slice_by_byte)(string * dest, const string_slice * src, const usize b);
    void (*raw_string_slice_free)(string_slice * str_sl);
    Error (*raw_string_utf8_new)(string_utf8 ** str);
    Error (*raw_string_utf8_to_string)(string ** dest, const string_utf8 * str);
    Error (*raw_string_utf8_from)(string_utf8 ** str, const char * c_str);
    Error (*raw_string_utf8_len)(usize * len, const string_utf8 * c_str);
    Error (*raw_string_utf8_size)(usize * size, const string_utf8 * c_str);
    Error (*raw_string_utf8_clone)(string_utf8 ** str, const string_utf8 * c_str);
    Error (*raw_string_utf8_push_back_cstr)(string_utf8 * dest, const char * src);
    Error (*raw_string_utf8_push_back)(string_utf8 * dest, const string_utf8 * src);
    Error (*raw_string_utf8_insert)(string_utf8 * dest, const string_utf8 * src, const usize i);
    Error (*raw_string_utf8_push_front)(string_utf8 * dest, const string_utf8 * src);
    void (*raw_string_utf8_free)(string_utf8 * str);
    UID (*raw_uid_new)(void);
    boolean (*raw_vfs_mount_res)(const char * path, const char * mount_point);
    boolean (*raw_vfs_unmount_res)(const char * mount_point);
    boolean (*raw_vfs_mount_rfs)(const char * mount_point);
    boolean (*raw_vfs_mount_rfs_whitelist)(const char ** whitelist, u64 count, const char * mount_point);
    boolean (*raw_vfs_unmount_rfs)(void);
    boolean (*raw_vfs_res_path_exists)(const char * path);
    boolean (*raw_vfs_usr_path_exists)(const char * path, boolean prefer_res);
    void * (*raw_vfs_res_read_file)(const char * path, u64 * size);
    FileStream * (*raw_vfs_res_stream_open)(const char * path);
    void * (*raw_vfs_usr_read_file)(const char * path, u64 * size, boolean prefer_res);
    boolean (*raw_vfs_usr_write_file)(const char * path, const void * data, u64 size);
    FileStream * (*raw_vfs_usr_stream_open)(const char * path, boolean prefer_res);
    u64 (*raw_vfs_stream_size)(FileStream * stream);
    u64 (*raw_vfs_stream_read_n)(FileStream * stream, void * buffer, u64 size);
    void * (*raw_vfs_stream_read_all)(FileStream * stream, u64 * size);
    u64 (*raw_vfs_stream_write)(FileStream * stream, void * buffer, u64 size);
    boolean (*raw_vfs_stream_seek)(FileStream * stream, FSSeekFrom whence, u64 offset);
    u64 (*raw_vfs_stream_tell)(FileStream * stream, boolean * success);
    boolean (*raw_vfs_stream_flush)(FileStream * stream);
    void (*raw_vfs_stream_close)(FileStream * stream);
    boolean (*raw_window_server_register_backend)(const char * name, WindowServerBackend * backend);
    boolean (*raw_window_server_load_backend)(const char * name);
    WindowServerBackend * (*raw_window_server_backend_new)(void);
    boolean (*raw_window_server_backend_free)(WindowServerBackend * backend);
    boolean (*raw_window_server_backend_set_function)(WindowServerBackend * backend, const char * name, fptr function);
    fptr (*raw_window_server_backend_get_function)(WindowServerBackend * backend, const char * name);
    boolean (*raw_render_context_register_backend)(const char * render_server_name, const char * window_server_name, RenderContextBackend * backend);
    boolean (*raw_render_context_load_backend)(const char * render_server_name, const char * window_server_name);
    RenderContextBackend * (*raw_render_context_backend_new)(void);
    boolean (*raw_render_context_backend_free)(RenderContextBackend * backend);
    boolean (*raw_render_context_backend_set_function)(RenderContextBackend * backend, const char * name, fptr function);
    fptr (*raw_render_context_backend_get_function)(RenderContextBackend * backend, const char * name);
    boolean (*raw_render_server_register_backend)(const char * name, RenderServerBackend * backend);
    boolean (*raw_render_server_load_backend)(const char * name);
    RenderServerBackend * (*raw_render_server_backend_new)(void);
    boolean (*raw_render_server_backend_free)(RenderServerBackend * backend);
    boolean (*raw_render_server_backend_set_function)(RenderServerBackend * backend, const char * name, fptr function);
    fptr (*raw_render_server_backend_get_function)(RenderServerBackend * backend, const char * name);
    WindowServerWindow * (*raw_window_server_create_window)(const char * title, IVec2 size, WindowServerWindow * parent);
    boolean (*raw_window_server_destroy_window)(WindowServerWindow * this);
    boolean (*raw_window_server_window_set_title)(WindowServerWindow * this, const char * title);
    c_str (*raw_window_server_window_get_title)(WindowServerWindow * this);
    boolean (*raw_window_server_window_set_mode)(WindowServerWindow * this, WindowServerWindowMode mode);
    WindowServerWindowMode (*raw_window_server_window_get_mode)(WindowServerWindow * this);
    boolean (*raw_window_server_window_set_size)(WindowServerWindow * this, IVec2 dimensions);
    boolean (*raw_window_server_window_get_size)(WindowServerWindow * this, IVec2 * out);
    boolean (*raw_window_server_window_set_position)(WindowServerWindow * this, IVec2 dimensions);
    boolean (*raw_window_server_window_get_position)(WindowServerWindow * this, IVec2 * out);
    RenderContextSurface * (*raw_render_context_create_surface)(WindowServerWindow * window);
    boolean (*raw_render_context_destroy_surface)(RenderContextSurface * surface);
    boolean (*raw_render_context_surface_make_current)(RenderContextSurface * surface);
    boolean (*raw_render_context_surface_present)(RenderContextSurface * surface);
    fptr (*raw_render_context_get_proc_addr)(const char * proc);


    #if !defined(HEAPI_FULL_TRACE)
            void (*set_error)(Error err);
    Error (*get_error)(void);
    void (*__he_update_full_trace_info)(const char * func, const char * file, i32 line);
    datetime_handle (*datetime_new)(void);
    void (*datetime_free)(datetime_handle handle);
    void (*datetime_update)(datetime_handle handle);
    u16 (*datetime_get_year)(datetime_handle handle);
    u8 (*datetime_get_month)(datetime_handle handle);
    u8 (*datetime_get_day)(datetime_handle handle);
    u8 (*datetime_get_hour)(datetime_handle handle);
    u8 (*datetime_get_minute)(datetime_handle handle);
    u8 (*datetime_get_second)(datetime_handle handle);
    u32 (*datetime_get_nanosecond)(datetime_handle handle);
    u16 (*datetime_get_utc_year)(datetime_handle handle);
    u8 (*datetime_get_utc_month)(datetime_handle handle);
    u8 (*datetime_get_utc_day)(datetime_handle handle);
    u8 (*datetime_get_utc_hour)(datetime_handle handle);
    u8 (*datetime_get_utc_minute)(datetime_handle handle);
    u8 (*datetime_get_utc_second)(datetime_handle handle);
    void * (*tmalloc)(u64 size);
    void * (*trealloc)(void * ptr, u64 size);
    void (*tfree)(void * ptr);
    u64 (*get_allocated_memory)(void);
    mutex_handle (*mutex_new)(void);
    mutex_handle (*mutex_new_recursive)(void);
    void (*mutex_free)(mutex_handle handle);
    void (*mutex_lock)(mutex_handle handle);
    boolean (*mutex_try_lock)(mutex_handle handle);
    void (*mutex_unlock)(mutex_handle handle);
    void (*auto_free)(Object * object);
    Node * (*node_new)(const char * name);
    Node * (*from_node)(Node * node);
    void (*node_set_name)(Node * node, const char * name);
    const char * (*node_get_name)(Node * node);
    void (*node_add_child)(Node * node, Node * child);
    boolean (*node_remove_child)(Node * node, Node * child);
    boolean (*node_remove_child_by_name)(Node * node, const char * name);
    boolean (*node_remove_child_by_uid)(Node * node, UID uid);
    boolean (*node_remove_all_children)(Node * node);
    Node * (*node_get_child_by_name)(Node * node, const char * name);
    Node * (*node_get_child_by_uid)(Node * node, UID uid);
    Node3D * (*node3d_new)(const char * name);
    Window * (*window_new)(const char * name, const char * title, i32 w, i32 h);
    void (*window_set_title)(Window * this, const char * title);
    const char * (*window_get_title)(const Window * this);
    CanvasItem * (*canvas_item_new)(const char * name);
    Control * (*control_new)(const char * name);
    Viewport * (*viewport_new)(const char * name);
    void (*mat4_init)(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33, Mat4 *const out);
    void (*mat4_from_array)(const float *const array, Mat4 *const out);
    void (*mat4_clone)(const Mat4 *const a, Mat4 *const out);
    void (*mat4_add)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);
    void (*mat4_sub)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);
    void (*mat4_mul)(const Mat4 *const a, const Mat4 *const b, Mat4 *const out);
    void (*mat4_scale)(const Mat4 *const a, const float factor, Mat4 *const out);
    void (*mat4_transpose)(const Mat4 *const a, Mat4 *const out);
    void (*mat4_inverse)(const Mat4 *const a, Mat4 *const out);
    void (*ivec2_init)(const i32 x, const i32 y, IVec2 *const out);
    void (*ivec2_add)(const IVec2 *const a, const IVec2 *const b, IVec2 *const out);
    void (*ivec2_sub)(const IVec2 *const a, const IVec2 *const b, IVec2 *const out);
    void (*ivec2_scale)(const IVec2 *const a, const float factor, IVec2 *const out);
    float (*ivec2_dot)(const IVec2 *const a, const IVec2 *const b);
    float (*ivec2_length)(const IVec2 *const a);
    void (*ivec2_normalize)(const IVec2 *const a, IVec2 *const out);
    float (*ivec2_distance)(const IVec2 *const a, const IVec2 *const b);
    void (*ivec2_add_in)(IVec2 *const to, const IVec2 *const what);
    void (*ivec2_sub_in)(IVec2 *const from, const IVec2 *const what);
    void (*ivec2_scale_in)(IVec2 *const to, const float factor);
    void (*ivec2_normalize_in)(IVec2 *const a);
    void (*ivec3_init)(i32 x, i32 y, i32 z, IVec3 *const out);
    void (*ivec3_add)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);
    void (*ivec3_sub)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);
    void (*ivec3_scale)(const IVec3 *const a, const float factor, IVec3 *const out);
    void (*ivec3_cross)(const IVec3 *const a, const IVec3 *const b, IVec3 *const out);
    float (*ivec3_dot)(const IVec3 *const a, const IVec3 *const b);
    float (*ivec3_length)(const IVec3 *const a);
    void (*ivec3_normalize)(const IVec3 *const a, IVec3 *const out);
    float (*ivec3_distance)(const IVec3 *const a, const IVec3 *const b);
    void (*ivec3_add_in)(IVec3 *const to, const IVec3 *const what);
    void (*ivec3_sub_in)(IVec3 *const from, const IVec3 *const what);
    void (*ivec3_scale_in)(IVec3 *const to, const float factor);
    void (*ivec3_normalize_in)(IVec3 *const a);
    void (*ivec4_init)(i32 x, i32 y, i32 z, i32 w, IVec4 *const out);
    void (*ivec4_add)(const IVec4 *const a, const IVec4 *const b, IVec4 *const out);
    void (*ivec4_sub)(const IVec4 *const a, const IVec4 *const b, IVec4 *const out);
    void (*ivec4_scale)(const IVec4 *const a, const float factor, IVec4 *const out);
    float (*ivec4_dot)(const IVec4 *const a, const IVec4 *const b);
    float (*ivec4_length)(const IVec4 *const a);
    void (*ivec4_normalize)(const IVec4 *const a, IVec4 *const out);
    float (*ivec4_distance)(const IVec4 *const a, const IVec4 *const b);
    void (*ivec4_add_in)(IVec4 *const to, const IVec4 *const what);
    void (*ivec4_sub_in)(IVec4 *const from, const IVec4 *const what);
    void (*ivec4_scale_in)(IVec4 *const to, const float factor);
    void (*ivec4_normalize_in)(IVec4 *const a);
    void (*uvec2_init)(const u32 x, const u32 y, UVec2 *const out);
    void (*uvec2_add)(const UVec2 *const a, const UVec2 *const b, UVec2 *const out);
    void (*uvec2_sub)(const UVec2 *const a, const UVec2 *const b, UVec2 *const out);
    void (*uvec2_scale)(const UVec2 *const a, const float factor, UVec2 *const out);
    float (*uvec2_dot)(const UVec2 *const a, const UVec2 *const b);
    float (*uvec2_length)(const UVec2 *const a);
    void (*uvec2_normalize)(const UVec2 *const a, UVec2 *const out);
    float (*uvec2_distance)(const UVec2 *const a, const UVec2 *const b);
    void (*uvec2_add_in)(UVec2 *const to, const UVec2 *const what);
    void (*uvec2_sub_in)(UVec2 *const from, const UVec2 *const what);
    void (*uvec2_scale_in)(UVec2 *const to, const float factor);
    void (*uvec2_normalize_in)(UVec2 *const a);
    void (*uvec3_init)(u32 x, u32 y, u32 z, UVec3 *const out);
    void (*uvec3_add)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);
    void (*uvec3_sub)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);
    void (*uvec3_scale)(const UVec3 *const a, const float factor, UVec3 *const out);
    void (*uvec3_cross)(const UVec3 *const a, const UVec3 *const b, UVec3 *const out);
    float (*uvec3_dot)(const UVec3 *const a, const UVec3 *const b);
    float (*uvec3_length)(const UVec3 *const a);
    void (*uvec3_normalize)(const UVec3 *const a, UVec3 *const out);
    float (*uvec3_distance)(const UVec3 *const a, const UVec3 *const b);
    void (*uvec3_add_in)(UVec3 *const to, const UVec3 *const what);
    void (*uvec3_sub_in)(UVec3 *const from, const UVec3 *const what);
    void (*uvec3_scale_in)(UVec3 *const to, const float factor);
    void (*uvec3_normalize_in)(UVec3 *const a);
    void (*uvec4_init)(u32 x, u32 y, u32 z, u32 w, UVec4 *const out);
    void (*uvec4_add)(const UVec4 *const a, const UVec4 *const b, UVec4 *const out);
    void (*uvec4_sub)(const UVec4 *const a, const UVec4 *const b, UVec4 *const out);
    void (*uvec4_scale)(const UVec4 *const a, const float factor, UVec4 *const out);
    float (*uvec4_dot)(const UVec4 *const a, const UVec4 *const b);
    float (*uvec4_length)(const UVec4 *const a);
    void (*uvec4_normalize)(const UVec4 *const a, UVec4 *const out);
    float (*uvec4_distance)(const UVec4 *const a, const UVec4 *const b);
    void (*uvec4_add_in)(UVec4 *const to, const UVec4 *const what);
    void (*uvec4_sub_in)(UVec4 *const from, const UVec4 *const what);
    void (*uvec4_scale_in)(UVec4 *const to, const float factor);
    void (*uvec4_normalize_in)(UVec4 *const a);
    void (*vec2_init)(const float x, const float y, Vec2 *const out);
    void (*vec2_add)(const Vec2 *const a, const Vec2 *const b, Vec2 *const out);
    void (*vec2_sub)(const Vec2 *const a, const Vec2 *const b, Vec2 *const out);
    void (*vec2_scale)(const Vec2 *const a, const float factor, Vec2 *const out);
    float (*vec2_dot)(const Vec2 *const a, const Vec2 *const b);
    float (*vec2_length)(const Vec2 *const a);
    void (*vec2_normalize)(const Vec2 *const a, Vec2 *const out);
    float (*vec2_distance)(const Vec2 *const a, const Vec2 *const b);
    void (*vec2_add_in)(Vec2 *const to, const Vec2 *const what);
    void (*vec2_sub_in)(Vec2 *const from, const Vec2 *const what);
    void (*vec2_scale_in)(Vec2 *const to, const float factor);
    void (*vec2_normalize_in)(Vec2 *const a);
    void (*vec3_init)(float x, float y, float z, Vec3 *const out);
    void (*vec3_add)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);
    void (*vec3_sub)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);
    void (*vec3_scale)(const Vec3 *const a, const float factor, Vec3 *const out);
    void (*vec3_cross)(const Vec3 *const a, const Vec3 *const b, Vec3 *const out);
    float (*vec3_dot)(const Vec3 *const a, const Vec3 *const b);
    float (*vec3_length)(const Vec3 *const a);
    void (*vec3_normalize)(const Vec3 *const a, Vec3 *const out);
    float (*vec3_distance)(const Vec3 *const a, const Vec3 *const b);
    void (*vec3_add_in)(Vec3 *const to, const Vec3 *const what);
    void (*vec3_sub_in)(Vec3 *const from, const Vec3 *const what);
    void (*vec3_scale_in)(Vec3 *const to, const float factor);
    void (*vec3_normalize_in)(Vec3 *const a);
    void (*vec4_init)(float x, float y, float z, float w, Vec4 *const out);
    void (*vec4_add)(const Vec4 *const a, const Vec4 *const b, Vec4 *const out);
    void (*vec4_sub)(const Vec4 *const a, const Vec4 *const b, Vec4 *const out);
    void (*vec4_scale)(const Vec4 *const a, const float factor, Vec4 *const out);
    float (*vec4_dot)(const Vec4 *const a, const Vec4 *const b);
    float (*vec4_length)(const Vec4 *const a);
    void (*vec4_normalize)(const Vec4 *const a, Vec4 *const out);
    float (*vec4_distance)(const Vec4 *const a, const Vec4 *const b);
    void (*vec4_add_in)(Vec4 *const to, const Vec4 *const what);
    void (*vec4_sub_in)(Vec4 *const from, const Vec4 *const what);
    void (*vec4_scale_in)(Vec4 *const to, const float factor);
    void (*vec4_normalize_in)(Vec4 *const a);
    Error (*string_new)(string ** str);
    string * (*string_from)(const char * c_str);
    string * (*string_clone)(const string * c_str);
    string * (*string_set_cstr)(string * self, const char * c_cstr);
    string * (*string_set)(string * self, const string * c_str);
    char * (*string_cstr)(const string * c_str);
    usize (*string_len)(const string * c_str);
    usize (*string_size)(const string * c_str);
    string * (*string_push_back)(string * dest, const string * src);
    string * (*string_push_back_cstr)(string * dest, const char * src);
    string * (*string_push_front)(string * dest, const string * src);
    string * (*string_push_front_cstr)(string * dest, const char * src);
    string * (*string_insert)(string * dest, const string * src, const usize i);
    string * (*string_insert_cstr)(string * dest, const char * src, const usize i);
    string * (*string_insert_cstr_ex)(string * dest, const char * src, const usize i);
    string * (*string_insert_cstr_by_byte)(string * dest, const char * src, const usize b);
    string * (*string_insert_by_byte)(string * dest, const string * src, const usize b);
    string * (*string_remove)(string * self, const usize i);
    string * (*string_remove_by_byte)(string * self, const usize b);
    string * (*string_remove_n)(string * self, const usize i, const usize n);
    string * (*string_remove_n_by_byte)(string * self, const usize b, const usize b_n);
    boolean (*string_equals)(const string * str1, const string * str2);
    boolean (*string_equals_cstr)(const string * str, const char * cstr);
    void (*string_free)(string * str);
    void (*string_itr_free)(string_itr * str_itr);
    string_itr * (*string_get_itr)(const string * c_str);
    u8 (*string_itr_next)(string_itr * str_itr);
    string_slice * (*string_get_slice)(const string * str, const usize s, const usize e);
    string * (*string_from_slice)(const string_slice * str_sl);
    boolean (*string_equals_slice)(string_slice * str_sl_1, string_slice * str_sl_2);
    string_slice * (*string_set_slice)(string_slice * self, const string_slice * str_sl);
    string * (*string_push_back_slice)(string * dest, const string_slice * src);
    string * (*string_push_front_slice)(string * dest, const string_slice * src);
    string * (*string_insert_slice_ex)(string * dest, const string_slice * src, const usize i);
    string * (*string_insert_slice)(string * dest, const string_slice * src, const usize i);
    string * (*string_insert_slice_by_byte)(string * dest, const string_slice * src, const usize b);
    void (*string_slice_free)(string_slice * str_sl);
    Error (*string_utf8_new)(string_utf8 ** str);
    Error (*string_utf8_to_string)(string ** dest, const string_utf8 * str);
    Error (*string_utf8_from)(string_utf8 ** str, const char * c_str);
    Error (*string_utf8_len)(usize * len, const string_utf8 * c_str);
    Error (*string_utf8_size)(usize * size, const string_utf8 * c_str);
    Error (*string_utf8_clone)(string_utf8 ** str, const string_utf8 * c_str);
    Error (*string_utf8_push_back_cstr)(string_utf8 * dest, const char * src);
    Error (*string_utf8_push_back)(string_utf8 * dest, const string_utf8 * src);
    Error (*string_utf8_insert)(string_utf8 * dest, const string_utf8 * src, const usize i);
    Error (*string_utf8_push_front)(string_utf8 * dest, const string_utf8 * src);
    void (*string_utf8_free)(string_utf8 * str);
    UID (*uid_new)(void);
    boolean (*vfs_mount_res)(const char * path, const char * mount_point);
    boolean (*vfs_unmount_res)(const char * mount_point);
    boolean (*vfs_mount_rfs)(const char * mount_point);
    boolean (*vfs_mount_rfs_whitelist)(const char ** whitelist, u64 count, const char * mount_point);
    boolean (*vfs_unmount_rfs)(void);
    boolean (*vfs_res_path_exists)(const char * path);
    boolean (*vfs_usr_path_exists)(const char * path, boolean prefer_res);
    void * (*vfs_res_read_file)(const char * path, u64 * size);
    FileStream * (*vfs_res_stream_open)(const char * path);
    void * (*vfs_usr_read_file)(const char * path, u64 * size, boolean prefer_res);
    boolean (*vfs_usr_write_file)(const char * path, const void * data, u64 size);
    FileStream * (*vfs_usr_stream_open)(const char * path, boolean prefer_res);
    u64 (*vfs_stream_size)(FileStream * stream);
    u64 (*vfs_stream_read_n)(FileStream * stream, void * buffer, u64 size);
    void * (*vfs_stream_read_all)(FileStream * stream, u64 * size);
    u64 (*vfs_stream_write)(FileStream * stream, void * buffer, u64 size);
    boolean (*vfs_stream_seek)(FileStream * stream, FSSeekFrom whence, u64 offset);
    u64 (*vfs_stream_tell)(FileStream * stream, boolean * success);
    boolean (*vfs_stream_flush)(FileStream * stream);
    void (*vfs_stream_close)(FileStream * stream);
    boolean (*window_server_register_backend)(const char * name, WindowServerBackend * backend);
    boolean (*window_server_load_backend)(const char * name);
    WindowServerBackend * (*window_server_backend_new)(void);
    boolean (*window_server_backend_free)(WindowServerBackend * backend);
    boolean (*window_server_backend_set_function)(WindowServerBackend * backend, const char * name, fptr function);
    fptr (*window_server_backend_get_function)(WindowServerBackend * backend, const char * name);
    boolean (*render_context_register_backend)(const char * render_server_name, const char * window_server_name, RenderContextBackend * backend);
    boolean (*render_context_load_backend)(const char * render_server_name, const char * window_server_name);
    RenderContextBackend * (*render_context_backend_new)(void);
    boolean (*render_context_backend_free)(RenderContextBackend * backend);
    boolean (*render_context_backend_set_function)(RenderContextBackend * backend, const char * name, fptr function);
    fptr (*render_context_backend_get_function)(RenderContextBackend * backend, const char * name);
    boolean (*render_server_register_backend)(const char * name, RenderServerBackend * backend);
    boolean (*render_server_load_backend)(const char * name);
    RenderServerBackend * (*render_server_backend_new)(void);
    boolean (*render_server_backend_free)(RenderServerBackend * backend);
    boolean (*render_server_backend_set_function)(RenderServerBackend * backend, const char * name, fptr function);
    fptr (*render_server_backend_get_function)(RenderServerBackend * backend, const char * name);
    WindowServerWindow * (*window_server_create_window)(const char * title, IVec2 size, WindowServerWindow * parent);
    boolean (*window_server_destroy_window)(WindowServerWindow * this);
    boolean (*window_server_window_set_title)(WindowServerWindow * this, const char * title);
    c_str (*window_server_window_get_title)(WindowServerWindow * this);
    boolean (*window_server_window_set_mode)(WindowServerWindow * this, WindowServerWindowMode mode);
    WindowServerWindowMode (*window_server_window_get_mode)(WindowServerWindow * this);
    boolean (*window_server_window_set_size)(WindowServerWindow * this, IVec2 dimensions);
    boolean (*window_server_window_get_size)(WindowServerWindow * this, IVec2 * out);
    boolean (*window_server_window_set_position)(WindowServerWindow * this, IVec2 dimensions);
    boolean (*window_server_window_get_position)(WindowServerWindow * this, IVec2 * out);
    RenderContextSurface * (*render_context_create_surface)(WindowServerWindow * window);
    boolean (*render_context_destroy_surface)(RenderContextSurface * surface);
    boolean (*render_context_surface_make_current)(RenderContextSurface * surface);
    boolean (*render_context_surface_present)(RenderContextSurface * surface);
    fptr (*render_context_get_proc_addr)(const char * proc);

    #endif

    void ___hate_engine_runtime_init(void* (*proc_addr)(const char* name)) {
                raw_set_error = (void (*)(Error))proc_addr("set_error");
        raw_get_error = (Error (*)(void))proc_addr("get_error");
        raw___he_update_full_trace_info = (void (*)(const char *, const char *, i32))proc_addr("__he_update_full_trace_info");
        raw_datetime_new = (datetime_handle (*)(void))proc_addr("datetime_new");
        raw_datetime_free = (void (*)(datetime_handle))proc_addr("datetime_free");
        raw_datetime_update = (void (*)(datetime_handle))proc_addr("datetime_update");
        raw_datetime_get_year = (u16 (*)(datetime_handle))proc_addr("datetime_get_year");
        raw_datetime_get_month = (u8 (*)(datetime_handle))proc_addr("datetime_get_month");
        raw_datetime_get_day = (u8 (*)(datetime_handle))proc_addr("datetime_get_day");
        raw_datetime_get_hour = (u8 (*)(datetime_handle))proc_addr("datetime_get_hour");
        raw_datetime_get_minute = (u8 (*)(datetime_handle))proc_addr("datetime_get_minute");
        raw_datetime_get_second = (u8 (*)(datetime_handle))proc_addr("datetime_get_second");
        raw_datetime_get_nanosecond = (u32 (*)(datetime_handle))proc_addr("datetime_get_nanosecond");
        raw_datetime_get_utc_year = (u16 (*)(datetime_handle))proc_addr("datetime_get_utc_year");
        raw_datetime_get_utc_month = (u8 (*)(datetime_handle))proc_addr("datetime_get_utc_month");
        raw_datetime_get_utc_day = (u8 (*)(datetime_handle))proc_addr("datetime_get_utc_day");
        raw_datetime_get_utc_hour = (u8 (*)(datetime_handle))proc_addr("datetime_get_utc_hour");
        raw_datetime_get_utc_minute = (u8 (*)(datetime_handle))proc_addr("datetime_get_utc_minute");
        raw_datetime_get_utc_second = (u8 (*)(datetime_handle))proc_addr("datetime_get_utc_second");
        raw_tmalloc = (void * (*)(u64))proc_addr("tmalloc");
        raw_trealloc = (void * (*)(void *, u64))proc_addr("trealloc");
        raw_tfree = (void (*)(void *))proc_addr("tfree");
        raw_get_allocated_memory = (u64 (*)(void))proc_addr("get_allocated_memory");
        raw_mutex_new = (mutex_handle (*)(void))proc_addr("mutex_new");
        raw_mutex_new_recursive = (mutex_handle (*)(void))proc_addr("mutex_new_recursive");
        raw_mutex_free = (void (*)(mutex_handle))proc_addr("mutex_free");
        raw_mutex_lock = (void (*)(mutex_handle))proc_addr("mutex_lock");
        raw_mutex_try_lock = (boolean (*)(mutex_handle))proc_addr("mutex_try_lock");
        raw_mutex_unlock = (void (*)(mutex_handle))proc_addr("mutex_unlock");
        raw_auto_free = (void (*)(Object *))proc_addr("auto_free");
        raw_node_new = (Node * (*)(const char *))proc_addr("node_new");
        raw_from_node = (Node * (*)(Node *))proc_addr("from_node");
        raw_node_set_name = (void (*)(Node *, const char *))proc_addr("node_set_name");
        raw_node_get_name = (const char * (*)(Node *))proc_addr("node_get_name");
        raw_node_add_child = (void (*)(Node *, Node *))proc_addr("node_add_child");
        raw_node_remove_child = (boolean (*)(Node *, Node *))proc_addr("node_remove_child");
        raw_node_remove_child_by_name = (boolean (*)(Node *, const char *))proc_addr("node_remove_child_by_name");
        raw_node_remove_child_by_uid = (boolean (*)(Node *, UID))proc_addr("node_remove_child_by_uid");
        raw_node_remove_all_children = (boolean (*)(Node *))proc_addr("node_remove_all_children");
        raw_node_get_child_by_name = (Node * (*)(Node *, const char *))proc_addr("node_get_child_by_name");
        raw_node_get_child_by_uid = (Node * (*)(Node *, UID))proc_addr("node_get_child_by_uid");
        raw_node3d_new = (Node3D * (*)(const char *))proc_addr("node3d_new");
        raw_window_new = (Window * (*)(const char *, const char *, i32, i32))proc_addr("window_new");
        raw_window_set_title = (void (*)(Window *, const char *))proc_addr("window_set_title");
        raw_window_get_title = (const char * (*)(const Window *))proc_addr("window_get_title");
        raw_canvas_item_new = (CanvasItem * (*)(const char *))proc_addr("canvas_item_new");
        raw_control_new = (Control * (*)(const char *))proc_addr("control_new");
        raw_viewport_new = (Viewport * (*)(const char *))proc_addr("viewport_new");
        raw_mat4_init = (void (*)(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, Mat4 *const))proc_addr("mat4_init");
        raw_mat4_from_array = (void (*)(const float *const, Mat4 *const))proc_addr("mat4_from_array");
        raw_mat4_clone = (void (*)(const Mat4 *const, Mat4 *const))proc_addr("mat4_clone");
        raw_mat4_add = (void (*)(const Mat4 *const, const Mat4 *const, Mat4 *const))proc_addr("mat4_add");
        raw_mat4_sub = (void (*)(const Mat4 *const, const Mat4 *const, Mat4 *const))proc_addr("mat4_sub");
        raw_mat4_mul = (void (*)(const Mat4 *const, const Mat4 *const, Mat4 *const))proc_addr("mat4_mul");
        raw_mat4_scale = (void (*)(const Mat4 *const, const float, Mat4 *const))proc_addr("mat4_scale");
        raw_mat4_transpose = (void (*)(const Mat4 *const, Mat4 *const))proc_addr("mat4_transpose");
        raw_mat4_inverse = (void (*)(const Mat4 *const, Mat4 *const))proc_addr("mat4_inverse");
        raw_ivec2_init = (void (*)(const i32, const i32, IVec2 *const))proc_addr("ivec2_init");
        raw_ivec2_add = (void (*)(const IVec2 *const, const IVec2 *const, IVec2 *const))proc_addr("ivec2_add");
        raw_ivec2_sub = (void (*)(const IVec2 *const, const IVec2 *const, IVec2 *const))proc_addr("ivec2_sub");
        raw_ivec2_scale = (void (*)(const IVec2 *const, const float, IVec2 *const))proc_addr("ivec2_scale");
        raw_ivec2_dot = (float (*)(const IVec2 *const, const IVec2 *const))proc_addr("ivec2_dot");
        raw_ivec2_length = (float (*)(const IVec2 *const))proc_addr("ivec2_length");
        raw_ivec2_normalize = (void (*)(const IVec2 *const, IVec2 *const))proc_addr("ivec2_normalize");
        raw_ivec2_distance = (float (*)(const IVec2 *const, const IVec2 *const))proc_addr("ivec2_distance");
        raw_ivec2_add_in = (void (*)(IVec2 *const, const IVec2 *const))proc_addr("ivec2_add_in");
        raw_ivec2_sub_in = (void (*)(IVec2 *const, const IVec2 *const))proc_addr("ivec2_sub_in");
        raw_ivec2_scale_in = (void (*)(IVec2 *const, const float))proc_addr("ivec2_scale_in");
        raw_ivec2_normalize_in = (void (*)(IVec2 *const))proc_addr("ivec2_normalize_in");
        raw_ivec3_init = (void (*)(i32, i32, i32, IVec3 *const))proc_addr("ivec3_init");
        raw_ivec3_add = (void (*)(const IVec3 *const, const IVec3 *const, IVec3 *const))proc_addr("ivec3_add");
        raw_ivec3_sub = (void (*)(const IVec3 *const, const IVec3 *const, IVec3 *const))proc_addr("ivec3_sub");
        raw_ivec3_scale = (void (*)(const IVec3 *const, const float, IVec3 *const))proc_addr("ivec3_scale");
        raw_ivec3_cross = (void (*)(const IVec3 *const, const IVec3 *const, IVec3 *const))proc_addr("ivec3_cross");
        raw_ivec3_dot = (float (*)(const IVec3 *const, const IVec3 *const))proc_addr("ivec3_dot");
        raw_ivec3_length = (float (*)(const IVec3 *const))proc_addr("ivec3_length");
        raw_ivec3_normalize = (void (*)(const IVec3 *const, IVec3 *const))proc_addr("ivec3_normalize");
        raw_ivec3_distance = (float (*)(const IVec3 *const, const IVec3 *const))proc_addr("ivec3_distance");
        raw_ivec3_add_in = (void (*)(IVec3 *const, const IVec3 *const))proc_addr("ivec3_add_in");
        raw_ivec3_sub_in = (void (*)(IVec3 *const, const IVec3 *const))proc_addr("ivec3_sub_in");
        raw_ivec3_scale_in = (void (*)(IVec3 *const, const float))proc_addr("ivec3_scale_in");
        raw_ivec3_normalize_in = (void (*)(IVec3 *const))proc_addr("ivec3_normalize_in");
        raw_ivec4_init = (void (*)(i32, i32, i32, i32, IVec4 *const))proc_addr("ivec4_init");
        raw_ivec4_add = (void (*)(const IVec4 *const, const IVec4 *const, IVec4 *const))proc_addr("ivec4_add");
        raw_ivec4_sub = (void (*)(const IVec4 *const, const IVec4 *const, IVec4 *const))proc_addr("ivec4_sub");
        raw_ivec4_scale = (void (*)(const IVec4 *const, const float, IVec4 *const))proc_addr("ivec4_scale");
        raw_ivec4_dot = (float (*)(const IVec4 *const, const IVec4 *const))proc_addr("ivec4_dot");
        raw_ivec4_length = (float (*)(const IVec4 *const))proc_addr("ivec4_length");
        raw_ivec4_normalize = (void (*)(const IVec4 *const, IVec4 *const))proc_addr("ivec4_normalize");
        raw_ivec4_distance = (float (*)(const IVec4 *const, const IVec4 *const))proc_addr("ivec4_distance");
        raw_ivec4_add_in = (void (*)(IVec4 *const, const IVec4 *const))proc_addr("ivec4_add_in");
        raw_ivec4_sub_in = (void (*)(IVec4 *const, const IVec4 *const))proc_addr("ivec4_sub_in");
        raw_ivec4_scale_in = (void (*)(IVec4 *const, const float))proc_addr("ivec4_scale_in");
        raw_ivec4_normalize_in = (void (*)(IVec4 *const))proc_addr("ivec4_normalize_in");
        raw_uvec2_init = (void (*)(const u32, const u32, UVec2 *const))proc_addr("uvec2_init");
        raw_uvec2_add = (void (*)(const UVec2 *const, const UVec2 *const, UVec2 *const))proc_addr("uvec2_add");
        raw_uvec2_sub = (void (*)(const UVec2 *const, const UVec2 *const, UVec2 *const))proc_addr("uvec2_sub");
        raw_uvec2_scale = (void (*)(const UVec2 *const, const float, UVec2 *const))proc_addr("uvec2_scale");
        raw_uvec2_dot = (float (*)(const UVec2 *const, const UVec2 *const))proc_addr("uvec2_dot");
        raw_uvec2_length = (float (*)(const UVec2 *const))proc_addr("uvec2_length");
        raw_uvec2_normalize = (void (*)(const UVec2 *const, UVec2 *const))proc_addr("uvec2_normalize");
        raw_uvec2_distance = (float (*)(const UVec2 *const, const UVec2 *const))proc_addr("uvec2_distance");
        raw_uvec2_add_in = (void (*)(UVec2 *const, const UVec2 *const))proc_addr("uvec2_add_in");
        raw_uvec2_sub_in = (void (*)(UVec2 *const, const UVec2 *const))proc_addr("uvec2_sub_in");
        raw_uvec2_scale_in = (void (*)(UVec2 *const, const float))proc_addr("uvec2_scale_in");
        raw_uvec2_normalize_in = (void (*)(UVec2 *const))proc_addr("uvec2_normalize_in");
        raw_uvec3_init = (void (*)(u32, u32, u32, UVec3 *const))proc_addr("uvec3_init");
        raw_uvec3_add = (void (*)(const UVec3 *const, const UVec3 *const, UVec3 *const))proc_addr("uvec3_add");
        raw_uvec3_sub = (void (*)(const UVec3 *const, const UVec3 *const, UVec3 *const))proc_addr("uvec3_sub");
        raw_uvec3_scale = (void (*)(const UVec3 *const, const float, UVec3 *const))proc_addr("uvec3_scale");
        raw_uvec3_cross = (void (*)(const UVec3 *const, const UVec3 *const, UVec3 *const))proc_addr("uvec3_cross");
        raw_uvec3_dot = (float (*)(const UVec3 *const, const UVec3 *const))proc_addr("uvec3_dot");
        raw_uvec3_length = (float (*)(const UVec3 *const))proc_addr("uvec3_length");
        raw_uvec3_normalize = (void (*)(const UVec3 *const, UVec3 *const))proc_addr("uvec3_normalize");
        raw_uvec3_distance = (float (*)(const UVec3 *const, const UVec3 *const))proc_addr("uvec3_distance");
        raw_uvec3_add_in = (void (*)(UVec3 *const, const UVec3 *const))proc_addr("uvec3_add_in");
        raw_uvec3_sub_in = (void (*)(UVec3 *const, const UVec3 *const))proc_addr("uvec3_sub_in");
        raw_uvec3_scale_in = (void (*)(UVec3 *const, const float))proc_addr("uvec3_scale_in");
        raw_uvec3_normalize_in = (void (*)(UVec3 *const))proc_addr("uvec3_normalize_in");
        raw_uvec4_init = (void (*)(u32, u32, u32, u32, UVec4 *const))proc_addr("uvec4_init");
        raw_uvec4_add = (void (*)(const UVec4 *const, const UVec4 *const, UVec4 *const))proc_addr("uvec4_add");
        raw_uvec4_sub = (void (*)(const UVec4 *const, const UVec4 *const, UVec4 *const))proc_addr("uvec4_sub");
        raw_uvec4_scale = (void (*)(const UVec4 *const, const float, UVec4 *const))proc_addr("uvec4_scale");
        raw_uvec4_dot = (float (*)(const UVec4 *const, const UVec4 *const))proc_addr("uvec4_dot");
        raw_uvec4_length = (float (*)(const UVec4 *const))proc_addr("uvec4_length");
        raw_uvec4_normalize = (void (*)(const UVec4 *const, UVec4 *const))proc_addr("uvec4_normalize");
        raw_uvec4_distance = (float (*)(const UVec4 *const, const UVec4 *const))proc_addr("uvec4_distance");
        raw_uvec4_add_in = (void (*)(UVec4 *const, const UVec4 *const))proc_addr("uvec4_add_in");
        raw_uvec4_sub_in = (void (*)(UVec4 *const, const UVec4 *const))proc_addr("uvec4_sub_in");
        raw_uvec4_scale_in = (void (*)(UVec4 *const, const float))proc_addr("uvec4_scale_in");
        raw_uvec4_normalize_in = (void (*)(UVec4 *const))proc_addr("uvec4_normalize_in");
        raw_vec2_init = (void (*)(const float, const float, Vec2 *const))proc_addr("vec2_init");
        raw_vec2_add = (void (*)(const Vec2 *const, const Vec2 *const, Vec2 *const))proc_addr("vec2_add");
        raw_vec2_sub = (void (*)(const Vec2 *const, const Vec2 *const, Vec2 *const))proc_addr("vec2_sub");
        raw_vec2_scale = (void (*)(const Vec2 *const, const float, Vec2 *const))proc_addr("vec2_scale");
        raw_vec2_dot = (float (*)(const Vec2 *const, const Vec2 *const))proc_addr("vec2_dot");
        raw_vec2_length = (float (*)(const Vec2 *const))proc_addr("vec2_length");
        raw_vec2_normalize = (void (*)(const Vec2 *const, Vec2 *const))proc_addr("vec2_normalize");
        raw_vec2_distance = (float (*)(const Vec2 *const, const Vec2 *const))proc_addr("vec2_distance");
        raw_vec2_add_in = (void (*)(Vec2 *const, const Vec2 *const))proc_addr("vec2_add_in");
        raw_vec2_sub_in = (void (*)(Vec2 *const, const Vec2 *const))proc_addr("vec2_sub_in");
        raw_vec2_scale_in = (void (*)(Vec2 *const, const float))proc_addr("vec2_scale_in");
        raw_vec2_normalize_in = (void (*)(Vec2 *const))proc_addr("vec2_normalize_in");
        raw_vec3_init = (void (*)(float, float, float, Vec3 *const))proc_addr("vec3_init");
        raw_vec3_add = (void (*)(const Vec3 *const, const Vec3 *const, Vec3 *const))proc_addr("vec3_add");
        raw_vec3_sub = (void (*)(const Vec3 *const, const Vec3 *const, Vec3 *const))proc_addr("vec3_sub");
        raw_vec3_scale = (void (*)(const Vec3 *const, const float, Vec3 *const))proc_addr("vec3_scale");
        raw_vec3_cross = (void (*)(const Vec3 *const, const Vec3 *const, Vec3 *const))proc_addr("vec3_cross");
        raw_vec3_dot = (float (*)(const Vec3 *const, const Vec3 *const))proc_addr("vec3_dot");
        raw_vec3_length = (float (*)(const Vec3 *const))proc_addr("vec3_length");
        raw_vec3_normalize = (void (*)(const Vec3 *const, Vec3 *const))proc_addr("vec3_normalize");
        raw_vec3_distance = (float (*)(const Vec3 *const, const Vec3 *const))proc_addr("vec3_distance");
        raw_vec3_add_in = (void (*)(Vec3 *const, const Vec3 *const))proc_addr("vec3_add_in");
        raw_vec3_sub_in = (void (*)(Vec3 *const, const Vec3 *const))proc_addr("vec3_sub_in");
        raw_vec3_scale_in = (void (*)(Vec3 *const, const float))proc_addr("vec3_scale_in");
        raw_vec3_normalize_in = (void (*)(Vec3 *const))proc_addr("vec3_normalize_in");
        raw_vec4_init = (void (*)(float, float, float, float, Vec4 *const))proc_addr("vec4_init");
        raw_vec4_add = (void (*)(const Vec4 *const, const Vec4 *const, Vec4 *const))proc_addr("vec4_add");
        raw_vec4_sub = (void (*)(const Vec4 *const, const Vec4 *const, Vec4 *const))proc_addr("vec4_sub");
        raw_vec4_scale = (void (*)(const Vec4 *const, const float, Vec4 *const))proc_addr("vec4_scale");
        raw_vec4_dot = (float (*)(const Vec4 *const, const Vec4 *const))proc_addr("vec4_dot");
        raw_vec4_length = (float (*)(const Vec4 *const))proc_addr("vec4_length");
        raw_vec4_normalize = (void (*)(const Vec4 *const, Vec4 *const))proc_addr("vec4_normalize");
        raw_vec4_distance = (float (*)(const Vec4 *const, const Vec4 *const))proc_addr("vec4_distance");
        raw_vec4_add_in = (void (*)(Vec4 *const, const Vec4 *const))proc_addr("vec4_add_in");
        raw_vec4_sub_in = (void (*)(Vec4 *const, const Vec4 *const))proc_addr("vec4_sub_in");
        raw_vec4_scale_in = (void (*)(Vec4 *const, const float))proc_addr("vec4_scale_in");
        raw_vec4_normalize_in = (void (*)(Vec4 *const))proc_addr("vec4_normalize_in");
        raw_string_new = (Error (*)(string **))proc_addr("string_new");
        raw_string_from = (string * (*)(const char *))proc_addr("string_from");
        raw_string_clone = (string * (*)(const string *))proc_addr("string_clone");
        raw_string_set_cstr = (string * (*)(string *, const char *))proc_addr("string_set_cstr");
        raw_string_set = (string * (*)(string *, const string *))proc_addr("string_set");
        raw_string_cstr = (char * (*)(const string *))proc_addr("string_cstr");
        raw_string_len = (usize (*)(const string *))proc_addr("string_len");
        raw_string_size = (usize (*)(const string *))proc_addr("string_size");
        raw_string_push_back = (string * (*)(string *, const string *))proc_addr("string_push_back");
        raw_string_push_back_cstr = (string * (*)(string *, const char *))proc_addr("string_push_back_cstr");
        raw_string_push_front = (string * (*)(string *, const string *))proc_addr("string_push_front");
        raw_string_push_front_cstr = (string * (*)(string *, const char *))proc_addr("string_push_front_cstr");
        raw_string_insert = (string * (*)(string *, const string *, const usize))proc_addr("string_insert");
        raw_string_insert_cstr = (string * (*)(string *, const char *, const usize))proc_addr("string_insert_cstr");
        raw_string_insert_cstr_ex = (string * (*)(string *, const char *, const usize))proc_addr("string_insert_cstr_ex");
        raw_string_insert_cstr_by_byte = (string * (*)(string *, const char *, const usize))proc_addr("string_insert_cstr_by_byte");
        raw_string_insert_by_byte = (string * (*)(string *, const string *, const usize))proc_addr("string_insert_by_byte");
        raw_string_remove = (string * (*)(string *, const usize))proc_addr("string_remove");
        raw_string_remove_by_byte = (string * (*)(string *, const usize))proc_addr("string_remove_by_byte");
        raw_string_remove_n = (string * (*)(string *, const usize, const usize))proc_addr("string_remove_n");
        raw_string_remove_n_by_byte = (string * (*)(string *, const usize, const usize))proc_addr("string_remove_n_by_byte");
        raw_string_equals = (boolean (*)(const string *, const string *))proc_addr("string_equals");
        raw_string_equals_cstr = (boolean (*)(const string *, const char *))proc_addr("string_equals_cstr");
        raw_string_free = (void (*)(string *))proc_addr("string_free");
        raw_string_itr_free = (void (*)(string_itr *))proc_addr("string_itr_free");
        raw_string_get_itr = (string_itr * (*)(const string *))proc_addr("string_get_itr");
        raw_string_itr_next = (u8 (*)(string_itr *))proc_addr("string_itr_next");
        raw_string_get_slice = (string_slice * (*)(const string *, const usize, const usize))proc_addr("string_get_slice");
        raw_string_from_slice = (string * (*)(const string_slice *))proc_addr("string_from_slice");
        raw_string_equals_slice = (boolean (*)(string_slice *, string_slice *))proc_addr("string_equals_slice");
        raw_string_set_slice = (string_slice * (*)(string_slice *, const string_slice *))proc_addr("string_set_slice");
        raw_string_push_back_slice = (string * (*)(string *, const string_slice *))proc_addr("string_push_back_slice");
        raw_string_push_front_slice = (string * (*)(string *, const string_slice *))proc_addr("string_push_front_slice");
        raw_string_insert_slice_ex = (string * (*)(string *, const string_slice *, const usize))proc_addr("string_insert_slice_ex");
        raw_string_insert_slice = (string * (*)(string *, const string_slice *, const usize))proc_addr("string_insert_slice");
        raw_string_insert_slice_by_byte = (string * (*)(string *, const string_slice *, const usize))proc_addr("string_insert_slice_by_byte");
        raw_string_slice_free = (void (*)(string_slice *))proc_addr("string_slice_free");
        raw_string_utf8_new = (Error (*)(string_utf8 **))proc_addr("string_utf8_new");
        raw_string_utf8_to_string = (Error (*)(string **, const string_utf8 *))proc_addr("string_utf8_to_string");
        raw_string_utf8_from = (Error (*)(string_utf8 **, const char *))proc_addr("string_utf8_from");
        raw_string_utf8_len = (Error (*)(usize *, const string_utf8 *))proc_addr("string_utf8_len");
        raw_string_utf8_size = (Error (*)(usize *, const string_utf8 *))proc_addr("string_utf8_size");
        raw_string_utf8_clone = (Error (*)(string_utf8 **, const string_utf8 *))proc_addr("string_utf8_clone");
        raw_string_utf8_push_back_cstr = (Error (*)(string_utf8 *, const char *))proc_addr("string_utf8_push_back_cstr");
        raw_string_utf8_push_back = (Error (*)(string_utf8 *, const string_utf8 *))proc_addr("string_utf8_push_back");
        raw_string_utf8_insert = (Error (*)(string_utf8 *, const string_utf8 *, const usize))proc_addr("string_utf8_insert");
        raw_string_utf8_push_front = (Error (*)(string_utf8 *, const string_utf8 *))proc_addr("string_utf8_push_front");
        raw_string_utf8_free = (void (*)(string_utf8 *))proc_addr("string_utf8_free");
        raw_uid_new = (UID (*)(void))proc_addr("uid_new");
        raw_vfs_mount_res = (boolean (*)(const char *, const char *))proc_addr("vfs_mount_res");
        raw_vfs_unmount_res = (boolean (*)(const char *))proc_addr("vfs_unmount_res");
        raw_vfs_mount_rfs = (boolean (*)(const char *))proc_addr("vfs_mount_rfs");
        raw_vfs_mount_rfs_whitelist = (boolean (*)(const char **, u64, const char *))proc_addr("vfs_mount_rfs_whitelist");
        raw_vfs_unmount_rfs = (boolean (*)(void))proc_addr("vfs_unmount_rfs");
        raw_vfs_res_path_exists = (boolean (*)(const char *))proc_addr("vfs_res_path_exists");
        raw_vfs_usr_path_exists = (boolean (*)(const char *, boolean))proc_addr("vfs_usr_path_exists");
        raw_vfs_res_read_file = (void * (*)(const char *, u64 *))proc_addr("vfs_res_read_file");
        raw_vfs_res_stream_open = (FileStream * (*)(const char *))proc_addr("vfs_res_stream_open");
        raw_vfs_usr_read_file = (void * (*)(const char *, u64 *, boolean))proc_addr("vfs_usr_read_file");
        raw_vfs_usr_write_file = (boolean (*)(const char *, const void *, u64))proc_addr("vfs_usr_write_file");
        raw_vfs_usr_stream_open = (FileStream * (*)(const char *, boolean))proc_addr("vfs_usr_stream_open");
        raw_vfs_stream_size = (u64 (*)(FileStream *))proc_addr("vfs_stream_size");
        raw_vfs_stream_read_n = (u64 (*)(FileStream *, void *, u64))proc_addr("vfs_stream_read_n");
        raw_vfs_stream_read_all = (void * (*)(FileStream *, u64 *))proc_addr("vfs_stream_read_all");
        raw_vfs_stream_write = (u64 (*)(FileStream *, void *, u64))proc_addr("vfs_stream_write");
        raw_vfs_stream_seek = (boolean (*)(FileStream *, FSSeekFrom, u64))proc_addr("vfs_stream_seek");
        raw_vfs_stream_tell = (u64 (*)(FileStream *, boolean *))proc_addr("vfs_stream_tell");
        raw_vfs_stream_flush = (boolean (*)(FileStream *))proc_addr("vfs_stream_flush");
        raw_vfs_stream_close = (void (*)(FileStream *))proc_addr("vfs_stream_close");
        raw_window_server_register_backend = (boolean (*)(const char *, WindowServerBackend *))proc_addr("window_server_register_backend");
        raw_window_server_load_backend = (boolean (*)(const char *))proc_addr("window_server_load_backend");
        raw_window_server_backend_new = (WindowServerBackend * (*)(void))proc_addr("window_server_backend_new");
        raw_window_server_backend_free = (boolean (*)(WindowServerBackend *))proc_addr("window_server_backend_free");
        raw_window_server_backend_set_function = (boolean (*)(WindowServerBackend *, const char *, fptr))proc_addr("window_server_backend_set_function");
        raw_window_server_backend_get_function = (fptr (*)(WindowServerBackend *, const char *))proc_addr("window_server_backend_get_function");
        raw_render_context_register_backend = (boolean (*)(const char *, const char *, RenderContextBackend *))proc_addr("render_context_register_backend");
        raw_render_context_load_backend = (boolean (*)(const char *, const char *))proc_addr("render_context_load_backend");
        raw_render_context_backend_new = (RenderContextBackend * (*)(void))proc_addr("render_context_backend_new");
        raw_render_context_backend_free = (boolean (*)(RenderContextBackend *))proc_addr("render_context_backend_free");
        raw_render_context_backend_set_function = (boolean (*)(RenderContextBackend *, const char *, fptr))proc_addr("render_context_backend_set_function");
        raw_render_context_backend_get_function = (fptr (*)(RenderContextBackend *, const char *))proc_addr("render_context_backend_get_function");
        raw_render_server_register_backend = (boolean (*)(const char *, RenderServerBackend *))proc_addr("render_server_register_backend");
        raw_render_server_load_backend = (boolean (*)(const char *))proc_addr("render_server_load_backend");
        raw_render_server_backend_new = (RenderServerBackend * (*)(void))proc_addr("render_server_backend_new");
        raw_render_server_backend_free = (boolean (*)(RenderServerBackend *))proc_addr("render_server_backend_free");
        raw_render_server_backend_set_function = (boolean (*)(RenderServerBackend *, const char *, fptr))proc_addr("render_server_backend_set_function");
        raw_render_server_backend_get_function = (fptr (*)(RenderServerBackend *, const char *))proc_addr("render_server_backend_get_function");


        #if !defined(HEAPI_FULL_TRACE)
                        set_error = raw_set_error;
            get_error = raw_get_error;
            __he_update_full_trace_info = raw___he_update_full_trace_info;
            datetime_new = raw_datetime_new;
            datetime_free = raw_datetime_free;
            datetime_update = raw_datetime_update;
            datetime_get_year = raw_datetime_get_year;
            datetime_get_month = raw_datetime_get_month;
            datetime_get_day = raw_datetime_get_day;
            datetime_get_hour = raw_datetime_get_hour;
            datetime_get_minute = raw_datetime_get_minute;
            datetime_get_second = raw_datetime_get_second;
            datetime_get_nanosecond = raw_datetime_get_nanosecond;
            datetime_get_utc_year = raw_datetime_get_utc_year;
            datetime_get_utc_month = raw_datetime_get_utc_month;
            datetime_get_utc_day = raw_datetime_get_utc_day;
            datetime_get_utc_hour = raw_datetime_get_utc_hour;
            datetime_get_utc_minute = raw_datetime_get_utc_minute;
            datetime_get_utc_second = raw_datetime_get_utc_second;
            tmalloc = raw_tmalloc;
            trealloc = raw_trealloc;
            tfree = raw_tfree;
            get_allocated_memory = raw_get_allocated_memory;
            mutex_new = raw_mutex_new;
            mutex_new_recursive = raw_mutex_new_recursive;
            mutex_free = raw_mutex_free;
            mutex_lock = raw_mutex_lock;
            mutex_try_lock = raw_mutex_try_lock;
            mutex_unlock = raw_mutex_unlock;
            auto_free = raw_auto_free;
            node_new = raw_node_new;
            from_node = raw_from_node;
            node_set_name = raw_node_set_name;
            node_get_name = raw_node_get_name;
            node_add_child = raw_node_add_child;
            node_remove_child = raw_node_remove_child;
            node_remove_child_by_name = raw_node_remove_child_by_name;
            node_remove_child_by_uid = raw_node_remove_child_by_uid;
            node_remove_all_children = raw_node_remove_all_children;
            node_get_child_by_name = raw_node_get_child_by_name;
            node_get_child_by_uid = raw_node_get_child_by_uid;
            node3d_new = raw_node3d_new;
            window_new = raw_window_new;
            window_set_title = raw_window_set_title;
            window_get_title = raw_window_get_title;
            canvas_item_new = raw_canvas_item_new;
            control_new = raw_control_new;
            viewport_new = raw_viewport_new;
            mat4_init = raw_mat4_init;
            mat4_from_array = raw_mat4_from_array;
            mat4_clone = raw_mat4_clone;
            mat4_add = raw_mat4_add;
            mat4_sub = raw_mat4_sub;
            mat4_mul = raw_mat4_mul;
            mat4_scale = raw_mat4_scale;
            mat4_transpose = raw_mat4_transpose;
            mat4_inverse = raw_mat4_inverse;
            ivec2_init = raw_ivec2_init;
            ivec2_add = raw_ivec2_add;
            ivec2_sub = raw_ivec2_sub;
            ivec2_scale = raw_ivec2_scale;
            ivec2_dot = raw_ivec2_dot;
            ivec2_length = raw_ivec2_length;
            ivec2_normalize = raw_ivec2_normalize;
            ivec2_distance = raw_ivec2_distance;
            ivec2_add_in = raw_ivec2_add_in;
            ivec2_sub_in = raw_ivec2_sub_in;
            ivec2_scale_in = raw_ivec2_scale_in;
            ivec2_normalize_in = raw_ivec2_normalize_in;
            ivec3_init = raw_ivec3_init;
            ivec3_add = raw_ivec3_add;
            ivec3_sub = raw_ivec3_sub;
            ivec3_scale = raw_ivec3_scale;
            ivec3_cross = raw_ivec3_cross;
            ivec3_dot = raw_ivec3_dot;
            ivec3_length = raw_ivec3_length;
            ivec3_normalize = raw_ivec3_normalize;
            ivec3_distance = raw_ivec3_distance;
            ivec3_add_in = raw_ivec3_add_in;
            ivec3_sub_in = raw_ivec3_sub_in;
            ivec3_scale_in = raw_ivec3_scale_in;
            ivec3_normalize_in = raw_ivec3_normalize_in;
            ivec4_init = raw_ivec4_init;
            ivec4_add = raw_ivec4_add;
            ivec4_sub = raw_ivec4_sub;
            ivec4_scale = raw_ivec4_scale;
            ivec4_dot = raw_ivec4_dot;
            ivec4_length = raw_ivec4_length;
            ivec4_normalize = raw_ivec4_normalize;
            ivec4_distance = raw_ivec4_distance;
            ivec4_add_in = raw_ivec4_add_in;
            ivec4_sub_in = raw_ivec4_sub_in;
            ivec4_scale_in = raw_ivec4_scale_in;
            ivec4_normalize_in = raw_ivec4_normalize_in;
            uvec2_init = raw_uvec2_init;
            uvec2_add = raw_uvec2_add;
            uvec2_sub = raw_uvec2_sub;
            uvec2_scale = raw_uvec2_scale;
            uvec2_dot = raw_uvec2_dot;
            uvec2_length = raw_uvec2_length;
            uvec2_normalize = raw_uvec2_normalize;
            uvec2_distance = raw_uvec2_distance;
            uvec2_add_in = raw_uvec2_add_in;
            uvec2_sub_in = raw_uvec2_sub_in;
            uvec2_scale_in = raw_uvec2_scale_in;
            uvec2_normalize_in = raw_uvec2_normalize_in;
            uvec3_init = raw_uvec3_init;
            uvec3_add = raw_uvec3_add;
            uvec3_sub = raw_uvec3_sub;
            uvec3_scale = raw_uvec3_scale;
            uvec3_cross = raw_uvec3_cross;
            uvec3_dot = raw_uvec3_dot;
            uvec3_length = raw_uvec3_length;
            uvec3_normalize = raw_uvec3_normalize;
            uvec3_distance = raw_uvec3_distance;
            uvec3_add_in = raw_uvec3_add_in;
            uvec3_sub_in = raw_uvec3_sub_in;
            uvec3_scale_in = raw_uvec3_scale_in;
            uvec3_normalize_in = raw_uvec3_normalize_in;
            uvec4_init = raw_uvec4_init;
            uvec4_add = raw_uvec4_add;
            uvec4_sub = raw_uvec4_sub;
            uvec4_scale = raw_uvec4_scale;
            uvec4_dot = raw_uvec4_dot;
            uvec4_length = raw_uvec4_length;
            uvec4_normalize = raw_uvec4_normalize;
            uvec4_distance = raw_uvec4_distance;
            uvec4_add_in = raw_uvec4_add_in;
            uvec4_sub_in = raw_uvec4_sub_in;
            uvec4_scale_in = raw_uvec4_scale_in;
            uvec4_normalize_in = raw_uvec4_normalize_in;
            vec2_init = raw_vec2_init;
            vec2_add = raw_vec2_add;
            vec2_sub = raw_vec2_sub;
            vec2_scale = raw_vec2_scale;
            vec2_dot = raw_vec2_dot;
            vec2_length = raw_vec2_length;
            vec2_normalize = raw_vec2_normalize;
            vec2_distance = raw_vec2_distance;
            vec2_add_in = raw_vec2_add_in;
            vec2_sub_in = raw_vec2_sub_in;
            vec2_scale_in = raw_vec2_scale_in;
            vec2_normalize_in = raw_vec2_normalize_in;
            vec3_init = raw_vec3_init;
            vec3_add = raw_vec3_add;
            vec3_sub = raw_vec3_sub;
            vec3_scale = raw_vec3_scale;
            vec3_cross = raw_vec3_cross;
            vec3_dot = raw_vec3_dot;
            vec3_length = raw_vec3_length;
            vec3_normalize = raw_vec3_normalize;
            vec3_distance = raw_vec3_distance;
            vec3_add_in = raw_vec3_add_in;
            vec3_sub_in = raw_vec3_sub_in;
            vec3_scale_in = raw_vec3_scale_in;
            vec3_normalize_in = raw_vec3_normalize_in;
            vec4_init = raw_vec4_init;
            vec4_add = raw_vec4_add;
            vec4_sub = raw_vec4_sub;
            vec4_scale = raw_vec4_scale;
            vec4_dot = raw_vec4_dot;
            vec4_length = raw_vec4_length;
            vec4_normalize = raw_vec4_normalize;
            vec4_distance = raw_vec4_distance;
            vec4_add_in = raw_vec4_add_in;
            vec4_sub_in = raw_vec4_sub_in;
            vec4_scale_in = raw_vec4_scale_in;
            vec4_normalize_in = raw_vec4_normalize_in;
            string_new = raw_string_new;
            string_from = raw_string_from;
            string_clone = raw_string_clone;
            string_set_cstr = raw_string_set_cstr;
            string_set = raw_string_set;
            string_cstr = raw_string_cstr;
            string_len = raw_string_len;
            string_size = raw_string_size;
            string_push_back = raw_string_push_back;
            string_push_back_cstr = raw_string_push_back_cstr;
            string_push_front = raw_string_push_front;
            string_push_front_cstr = raw_string_push_front_cstr;
            string_insert = raw_string_insert;
            string_insert_cstr = raw_string_insert_cstr;
            string_insert_cstr_ex = raw_string_insert_cstr_ex;
            string_insert_cstr_by_byte = raw_string_insert_cstr_by_byte;
            string_insert_by_byte = raw_string_insert_by_byte;
            string_remove = raw_string_remove;
            string_remove_by_byte = raw_string_remove_by_byte;
            string_remove_n = raw_string_remove_n;
            string_remove_n_by_byte = raw_string_remove_n_by_byte;
            string_equals = raw_string_equals;
            string_equals_cstr = raw_string_equals_cstr;
            string_free = raw_string_free;
            string_itr_free = raw_string_itr_free;
            string_get_itr = raw_string_get_itr;
            string_itr_next = raw_string_itr_next;
            string_get_slice = raw_string_get_slice;
            string_from_slice = raw_string_from_slice;
            string_equals_slice = raw_string_equals_slice;
            string_set_slice = raw_string_set_slice;
            string_push_back_slice = raw_string_push_back_slice;
            string_push_front_slice = raw_string_push_front_slice;
            string_insert_slice_ex = raw_string_insert_slice_ex;
            string_insert_slice = raw_string_insert_slice;
            string_insert_slice_by_byte = raw_string_insert_slice_by_byte;
            string_slice_free = raw_string_slice_free;
            string_utf8_new = raw_string_utf8_new;
            string_utf8_to_string = raw_string_utf8_to_string;
            string_utf8_from = raw_string_utf8_from;
            string_utf8_len = raw_string_utf8_len;
            string_utf8_size = raw_string_utf8_size;
            string_utf8_clone = raw_string_utf8_clone;
            string_utf8_push_back_cstr = raw_string_utf8_push_back_cstr;
            string_utf8_push_back = raw_string_utf8_push_back;
            string_utf8_insert = raw_string_utf8_insert;
            string_utf8_push_front = raw_string_utf8_push_front;
            string_utf8_free = raw_string_utf8_free;
            uid_new = raw_uid_new;
            vfs_mount_res = raw_vfs_mount_res;
            vfs_unmount_res = raw_vfs_unmount_res;
            vfs_mount_rfs = raw_vfs_mount_rfs;
            vfs_mount_rfs_whitelist = raw_vfs_mount_rfs_whitelist;
            vfs_unmount_rfs = raw_vfs_unmount_rfs;
            vfs_res_path_exists = raw_vfs_res_path_exists;
            vfs_usr_path_exists = raw_vfs_usr_path_exists;
            vfs_res_read_file = raw_vfs_res_read_file;
            vfs_res_stream_open = raw_vfs_res_stream_open;
            vfs_usr_read_file = raw_vfs_usr_read_file;
            vfs_usr_write_file = raw_vfs_usr_write_file;
            vfs_usr_stream_open = raw_vfs_usr_stream_open;
            vfs_stream_size = raw_vfs_stream_size;
            vfs_stream_read_n = raw_vfs_stream_read_n;
            vfs_stream_read_all = raw_vfs_stream_read_all;
            vfs_stream_write = raw_vfs_stream_write;
            vfs_stream_seek = raw_vfs_stream_seek;
            vfs_stream_tell = raw_vfs_stream_tell;
            vfs_stream_flush = raw_vfs_stream_flush;
            vfs_stream_close = raw_vfs_stream_close;
            window_server_register_backend = raw_window_server_register_backend;
            window_server_load_backend = raw_window_server_load_backend;
            window_server_backend_new = raw_window_server_backend_new;
            window_server_backend_free = raw_window_server_backend_free;
            window_server_backend_set_function = raw_window_server_backend_set_function;
            window_server_backend_get_function = raw_window_server_backend_get_function;
            render_context_register_backend = raw_render_context_register_backend;
            render_context_load_backend = raw_render_context_load_backend;
            render_context_backend_new = raw_render_context_backend_new;
            render_context_backend_free = raw_render_context_backend_free;
            render_context_backend_set_function = raw_render_context_backend_set_function;
            render_context_backend_get_function = raw_render_context_backend_get_function;
            render_server_register_backend = raw_render_server_register_backend;
            render_server_load_backend = raw_render_server_load_backend;
            render_server_backend_new = raw_render_server_backend_new;
            render_server_backend_free = raw_render_server_backend_free;
            render_server_backend_set_function = raw_render_server_backend_set_function;
            render_server_backend_get_function = raw_render_server_backend_get_function;

        #endif
    }

    void ___hate_engine_runtime_init_window_server(WindowServerBackend* backend) {
        raw_window_server_create_window = (WindowServerWindow * (*)(const char *, IVec2, WindowServerWindow *))raw_window_server_backend_get_function(backend, "create_window");
        raw_window_server_destroy_window = (boolean (*)(WindowServerWindow *))raw_window_server_backend_get_function(backend, "destroy_window");
        raw_window_server_window_set_title = (boolean (*)(WindowServerWindow *, const char *))raw_window_server_backend_get_function(backend, "window_set_title");
        raw_window_server_window_get_title = (c_str (*)(WindowServerWindow *))raw_window_server_backend_get_function(backend, "window_get_title");
        raw_window_server_window_set_mode = (boolean (*)(WindowServerWindow *, WindowServerWindowMode))raw_window_server_backend_get_function(backend, "window_set_mode");
        raw_window_server_window_get_mode = (WindowServerWindowMode (*)(WindowServerWindow *))raw_window_server_backend_get_function(backend, "window_get_mode");
        raw_window_server_window_set_size = (boolean (*)(WindowServerWindow *, IVec2))raw_window_server_backend_get_function(backend, "window_set_size");
        raw_window_server_window_get_size = (boolean (*)(WindowServerWindow *, IVec2 *))raw_window_server_backend_get_function(backend, "window_get_size");
        raw_window_server_window_set_position = (boolean (*)(WindowServerWindow *, IVec2))raw_window_server_backend_get_function(backend, "window_set_position");
        raw_window_server_window_get_position = (boolean (*)(WindowServerWindow *, IVec2 *))raw_window_server_backend_get_function(backend, "window_get_position");
        #if !defined(HEAPI_FULL_TRACE)
            window_server_create_window = raw_window_server_create_window;
            window_server_destroy_window = raw_window_server_destroy_window;
            window_server_window_set_title = raw_window_server_window_set_title;
            window_server_window_get_title = raw_window_server_window_get_title;
            window_server_window_set_mode = raw_window_server_window_set_mode;
            window_server_window_get_mode = raw_window_server_window_get_mode;
            window_server_window_set_size = raw_window_server_window_set_size;
            window_server_window_get_size = raw_window_server_window_get_size;
            window_server_window_set_position = raw_window_server_window_set_position;
            window_server_window_get_position = raw_window_server_window_get_position;
        #endif
    }

    void ___hate_engine_runtime_init_render_context(RenderContextBackend* backend) {
        raw_render_context_create_surface = (RenderContextSurface * (*)(WindowServerWindow *))raw_render_context_backend_get_function(backend, "create_surface");
        raw_render_context_destroy_surface = (boolean (*)(RenderContextSurface *))raw_render_context_backend_get_function(backend, "destroy_surface");
        raw_render_context_surface_make_current = (boolean (*)(RenderContextSurface *))raw_render_context_backend_get_function(backend, "surface_make_current");
        raw_render_context_surface_present = (boolean (*)(RenderContextSurface *))raw_render_context_backend_get_function(backend, "surface_present");
        raw_render_context_get_proc_addr = (fptr (*)(const char *))raw_render_context_backend_get_function(backend, "get_proc_addr");
        #if !defined(HEAPI_FULL_TRACE)
            render_context_create_surface = raw_render_context_create_surface;
            render_context_destroy_surface = raw_render_context_destroy_surface;
            render_context_surface_make_current = raw_render_context_surface_make_current;
            render_context_surface_present = raw_render_context_surface_present;
            render_context_get_proc_addr = raw_render_context_get_proc_addr;
        #endif
    }

    void ___hate_engine_runtime_init_render_server(RenderServerBackend* backend) {
        #if !defined(HEAPI_FULL_TRACE)
        #endif
    }


#endif
#endif

#if defined(HEAPI_FULL_TRACE)
void full_trace_set_error(const char* ___file___, uint32_t ___line___, Error);
Error full_trace_get_error(const char* ___file___, uint32_t ___line___);
datetime_handle full_trace_datetime_new(const char* ___file___, uint32_t ___line___);
void full_trace_datetime_free(const char* ___file___, uint32_t ___line___, datetime_handle);
void full_trace_datetime_update(const char* ___file___, uint32_t ___line___, datetime_handle);
u16 full_trace_datetime_get_year(const char* ___file___, uint32_t ___line___, datetime_handle);
u8 full_trace_datetime_get_month(const char* ___file___, uint32_t ___line___, datetime_handle);
u8 full_trace_datetime_get_day(const char* ___file___, uint32_t ___line___, datetime_handle);
u8 full_trace_datetime_get_hour(const char* ___file___, uint32_t ___line___, datetime_handle);
u8 full_trace_datetime_get_minute(const char* ___file___, uint32_t ___line___, datetime_handle);
u8 full_trace_datetime_get_second(const char* ___file___, uint32_t ___line___, datetime_handle);
u32 full_trace_datetime_get_nanosecond(const char* ___file___, uint32_t ___line___, datetime_handle);
u16 full_trace_datetime_get_utc_year(const char* ___file___, uint32_t ___line___, datetime_handle);
u8 full_trace_datetime_get_utc_month(const char* ___file___, uint32_t ___line___, datetime_handle);
u8 full_trace_datetime_get_utc_day(const char* ___file___, uint32_t ___line___, datetime_handle);
u8 full_trace_datetime_get_utc_hour(const char* ___file___, uint32_t ___line___, datetime_handle);
u8 full_trace_datetime_get_utc_minute(const char* ___file___, uint32_t ___line___, datetime_handle);
u8 full_trace_datetime_get_utc_second(const char* ___file___, uint32_t ___line___, datetime_handle);
void * full_trace_tmalloc(const char* ___file___, uint32_t ___line___, u64);
void * full_trace_trealloc(const char* ___file___, uint32_t ___line___, void *, u64);
void full_trace_tfree(const char* ___file___, uint32_t ___line___, void *);
u64 full_trace_get_allocated_memory(const char* ___file___, uint32_t ___line___);
mutex_handle full_trace_mutex_new(const char* ___file___, uint32_t ___line___);
mutex_handle full_trace_mutex_new_recursive(const char* ___file___, uint32_t ___line___);
void full_trace_mutex_free(const char* ___file___, uint32_t ___line___, mutex_handle);
void full_trace_mutex_lock(const char* ___file___, uint32_t ___line___, mutex_handle);
boolean full_trace_mutex_try_lock(const char* ___file___, uint32_t ___line___, mutex_handle);
void full_trace_mutex_unlock(const char* ___file___, uint32_t ___line___, mutex_handle);
void full_trace_auto_free(const char* ___file___, uint32_t ___line___, Object *);
Node * full_trace_node_new(const char* ___file___, uint32_t ___line___, const char *);
Node * full_trace_from_node(const char* ___file___, uint32_t ___line___, Node *);
void full_trace_node_set_name(const char* ___file___, uint32_t ___line___, Node *, const char *);
const char * full_trace_node_get_name(const char* ___file___, uint32_t ___line___, Node *);
void full_trace_node_add_child(const char* ___file___, uint32_t ___line___, Node *, Node *);
boolean full_trace_node_remove_child(const char* ___file___, uint32_t ___line___, Node *, Node *);
boolean full_trace_node_remove_child_by_name(const char* ___file___, uint32_t ___line___, Node *, const char *);
boolean full_trace_node_remove_child_by_uid(const char* ___file___, uint32_t ___line___, Node *, UID);
boolean full_trace_node_remove_all_children(const char* ___file___, uint32_t ___line___, Node *);
Node * full_trace_node_get_child_by_name(const char* ___file___, uint32_t ___line___, Node *, const char *);
Node * full_trace_node_get_child_by_uid(const char* ___file___, uint32_t ___line___, Node *, UID);
Node3D * full_trace_node3d_new(const char* ___file___, uint32_t ___line___, const char *);
Window * full_trace_window_new(const char* ___file___, uint32_t ___line___, const char *, const char *, i32, i32);
void full_trace_window_set_title(const char* ___file___, uint32_t ___line___, Window *, const char *);
const char * full_trace_window_get_title(const char* ___file___, uint32_t ___line___, const Window *);
CanvasItem * full_trace_canvas_item_new(const char* ___file___, uint32_t ___line___, const char *);
Control * full_trace_control_new(const char* ___file___, uint32_t ___line___, const char *);
Viewport * full_trace_viewport_new(const char* ___file___, uint32_t ___line___, const char *);
void full_trace_mat4_init(const char* ___file___, uint32_t ___line___, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, Mat4 *const);
void full_trace_mat4_from_array(const char* ___file___, uint32_t ___line___, const float *const, Mat4 *const);
void full_trace_mat4_clone(const char* ___file___, uint32_t ___line___, const Mat4 *const, Mat4 *const);
void full_trace_mat4_add(const char* ___file___, uint32_t ___line___, const Mat4 *const, const Mat4 *const, Mat4 *const);
void full_trace_mat4_sub(const char* ___file___, uint32_t ___line___, const Mat4 *const, const Mat4 *const, Mat4 *const);
void full_trace_mat4_mul(const char* ___file___, uint32_t ___line___, const Mat4 *const, const Mat4 *const, Mat4 *const);
void full_trace_mat4_scale(const char* ___file___, uint32_t ___line___, const Mat4 *const, const float, Mat4 *const);
void full_trace_mat4_transpose(const char* ___file___, uint32_t ___line___, const Mat4 *const, Mat4 *const);
void full_trace_mat4_inverse(const char* ___file___, uint32_t ___line___, const Mat4 *const, Mat4 *const);
void full_trace_ivec2_init(const char* ___file___, uint32_t ___line___, const i32, const i32, IVec2 *const);
void full_trace_ivec2_add(const char* ___file___, uint32_t ___line___, const IVec2 *const, const IVec2 *const, IVec2 *const);
void full_trace_ivec2_sub(const char* ___file___, uint32_t ___line___, const IVec2 *const, const IVec2 *const, IVec2 *const);
void full_trace_ivec2_scale(const char* ___file___, uint32_t ___line___, const IVec2 *const, const float, IVec2 *const);
float full_trace_ivec2_dot(const char* ___file___, uint32_t ___line___, const IVec2 *const, const IVec2 *const);
float full_trace_ivec2_length(const char* ___file___, uint32_t ___line___, const IVec2 *const);
void full_trace_ivec2_normalize(const char* ___file___, uint32_t ___line___, const IVec2 *const, IVec2 *const);
float full_trace_ivec2_distance(const char* ___file___, uint32_t ___line___, const IVec2 *const, const IVec2 *const);
void full_trace_ivec2_add_in(const char* ___file___, uint32_t ___line___, IVec2 *const, const IVec2 *const);
void full_trace_ivec2_sub_in(const char* ___file___, uint32_t ___line___, IVec2 *const, const IVec2 *const);
void full_trace_ivec2_scale_in(const char* ___file___, uint32_t ___line___, IVec2 *const, const float);
void full_trace_ivec2_normalize_in(const char* ___file___, uint32_t ___line___, IVec2 *const);
void full_trace_ivec3_init(const char* ___file___, uint32_t ___line___, i32, i32, i32, IVec3 *const);
void full_trace_ivec3_add(const char* ___file___, uint32_t ___line___, const IVec3 *const, const IVec3 *const, IVec3 *const);
void full_trace_ivec3_sub(const char* ___file___, uint32_t ___line___, const IVec3 *const, const IVec3 *const, IVec3 *const);
void full_trace_ivec3_scale(const char* ___file___, uint32_t ___line___, const IVec3 *const, const float, IVec3 *const);
void full_trace_ivec3_cross(const char* ___file___, uint32_t ___line___, const IVec3 *const, const IVec3 *const, IVec3 *const);
float full_trace_ivec3_dot(const char* ___file___, uint32_t ___line___, const IVec3 *const, const IVec3 *const);
float full_trace_ivec3_length(const char* ___file___, uint32_t ___line___, const IVec3 *const);
void full_trace_ivec3_normalize(const char* ___file___, uint32_t ___line___, const IVec3 *const, IVec3 *const);
float full_trace_ivec3_distance(const char* ___file___, uint32_t ___line___, const IVec3 *const, const IVec3 *const);
void full_trace_ivec3_add_in(const char* ___file___, uint32_t ___line___, IVec3 *const, const IVec3 *const);
void full_trace_ivec3_sub_in(const char* ___file___, uint32_t ___line___, IVec3 *const, const IVec3 *const);
void full_trace_ivec3_scale_in(const char* ___file___, uint32_t ___line___, IVec3 *const, const float);
void full_trace_ivec3_normalize_in(const char* ___file___, uint32_t ___line___, IVec3 *const);
void full_trace_ivec4_init(const char* ___file___, uint32_t ___line___, i32, i32, i32, i32, IVec4 *const);
void full_trace_ivec4_add(const char* ___file___, uint32_t ___line___, const IVec4 *const, const IVec4 *const, IVec4 *const);
void full_trace_ivec4_sub(const char* ___file___, uint32_t ___line___, const IVec4 *const, const IVec4 *const, IVec4 *const);
void full_trace_ivec4_scale(const char* ___file___, uint32_t ___line___, const IVec4 *const, const float, IVec4 *const);
float full_trace_ivec4_dot(const char* ___file___, uint32_t ___line___, const IVec4 *const, const IVec4 *const);
float full_trace_ivec4_length(const char* ___file___, uint32_t ___line___, const IVec4 *const);
void full_trace_ivec4_normalize(const char* ___file___, uint32_t ___line___, const IVec4 *const, IVec4 *const);
float full_trace_ivec4_distance(const char* ___file___, uint32_t ___line___, const IVec4 *const, const IVec4 *const);
void full_trace_ivec4_add_in(const char* ___file___, uint32_t ___line___, IVec4 *const, const IVec4 *const);
void full_trace_ivec4_sub_in(const char* ___file___, uint32_t ___line___, IVec4 *const, const IVec4 *const);
void full_trace_ivec4_scale_in(const char* ___file___, uint32_t ___line___, IVec4 *const, const float);
void full_trace_ivec4_normalize_in(const char* ___file___, uint32_t ___line___, IVec4 *const);
void full_trace_uvec2_init(const char* ___file___, uint32_t ___line___, const u32, const u32, UVec2 *const);
void full_trace_uvec2_add(const char* ___file___, uint32_t ___line___, const UVec2 *const, const UVec2 *const, UVec2 *const);
void full_trace_uvec2_sub(const char* ___file___, uint32_t ___line___, const UVec2 *const, const UVec2 *const, UVec2 *const);
void full_trace_uvec2_scale(const char* ___file___, uint32_t ___line___, const UVec2 *const, const float, UVec2 *const);
float full_trace_uvec2_dot(const char* ___file___, uint32_t ___line___, const UVec2 *const, const UVec2 *const);
float full_trace_uvec2_length(const char* ___file___, uint32_t ___line___, const UVec2 *const);
void full_trace_uvec2_normalize(const char* ___file___, uint32_t ___line___, const UVec2 *const, UVec2 *const);
float full_trace_uvec2_distance(const char* ___file___, uint32_t ___line___, const UVec2 *const, const UVec2 *const);
void full_trace_uvec2_add_in(const char* ___file___, uint32_t ___line___, UVec2 *const, const UVec2 *const);
void full_trace_uvec2_sub_in(const char* ___file___, uint32_t ___line___, UVec2 *const, const UVec2 *const);
void full_trace_uvec2_scale_in(const char* ___file___, uint32_t ___line___, UVec2 *const, const float);
void full_trace_uvec2_normalize_in(const char* ___file___, uint32_t ___line___, UVec2 *const);
void full_trace_uvec3_init(const char* ___file___, uint32_t ___line___, u32, u32, u32, UVec3 *const);
void full_trace_uvec3_add(const char* ___file___, uint32_t ___line___, const UVec3 *const, const UVec3 *const, UVec3 *const);
void full_trace_uvec3_sub(const char* ___file___, uint32_t ___line___, const UVec3 *const, const UVec3 *const, UVec3 *const);
void full_trace_uvec3_scale(const char* ___file___, uint32_t ___line___, const UVec3 *const, const float, UVec3 *const);
void full_trace_uvec3_cross(const char* ___file___, uint32_t ___line___, const UVec3 *const, const UVec3 *const, UVec3 *const);
float full_trace_uvec3_dot(const char* ___file___, uint32_t ___line___, const UVec3 *const, const UVec3 *const);
float full_trace_uvec3_length(const char* ___file___, uint32_t ___line___, const UVec3 *const);
void full_trace_uvec3_normalize(const char* ___file___, uint32_t ___line___, const UVec3 *const, UVec3 *const);
float full_trace_uvec3_distance(const char* ___file___, uint32_t ___line___, const UVec3 *const, const UVec3 *const);
void full_trace_uvec3_add_in(const char* ___file___, uint32_t ___line___, UVec3 *const, const UVec3 *const);
void full_trace_uvec3_sub_in(const char* ___file___, uint32_t ___line___, UVec3 *const, const UVec3 *const);
void full_trace_uvec3_scale_in(const char* ___file___, uint32_t ___line___, UVec3 *const, const float);
void full_trace_uvec3_normalize_in(const char* ___file___, uint32_t ___line___, UVec3 *const);
void full_trace_uvec4_init(const char* ___file___, uint32_t ___line___, u32, u32, u32, u32, UVec4 *const);
void full_trace_uvec4_add(const char* ___file___, uint32_t ___line___, const UVec4 *const, const UVec4 *const, UVec4 *const);
void full_trace_uvec4_sub(const char* ___file___, uint32_t ___line___, const UVec4 *const, const UVec4 *const, UVec4 *const);
void full_trace_uvec4_scale(const char* ___file___, uint32_t ___line___, const UVec4 *const, const float, UVec4 *const);
float full_trace_uvec4_dot(const char* ___file___, uint32_t ___line___, const UVec4 *const, const UVec4 *const);
float full_trace_uvec4_length(const char* ___file___, uint32_t ___line___, const UVec4 *const);
void full_trace_uvec4_normalize(const char* ___file___, uint32_t ___line___, const UVec4 *const, UVec4 *const);
float full_trace_uvec4_distance(const char* ___file___, uint32_t ___line___, const UVec4 *const, const UVec4 *const);
void full_trace_uvec4_add_in(const char* ___file___, uint32_t ___line___, UVec4 *const, const UVec4 *const);
void full_trace_uvec4_sub_in(const char* ___file___, uint32_t ___line___, UVec4 *const, const UVec4 *const);
void full_trace_uvec4_scale_in(const char* ___file___, uint32_t ___line___, UVec4 *const, const float);
void full_trace_uvec4_normalize_in(const char* ___file___, uint32_t ___line___, UVec4 *const);
void full_trace_vec2_init(const char* ___file___, uint32_t ___line___, const float, const float, Vec2 *const);
void full_trace_vec2_add(const char* ___file___, uint32_t ___line___, const Vec2 *const, const Vec2 *const, Vec2 *const);
void full_trace_vec2_sub(const char* ___file___, uint32_t ___line___, const Vec2 *const, const Vec2 *const, Vec2 *const);
void full_trace_vec2_scale(const char* ___file___, uint32_t ___line___, const Vec2 *const, const float, Vec2 *const);
float full_trace_vec2_dot(const char* ___file___, uint32_t ___line___, const Vec2 *const, const Vec2 *const);
float full_trace_vec2_length(const char* ___file___, uint32_t ___line___, const Vec2 *const);
void full_trace_vec2_normalize(const char* ___file___, uint32_t ___line___, const Vec2 *const, Vec2 *const);
float full_trace_vec2_distance(const char* ___file___, uint32_t ___line___, const Vec2 *const, const Vec2 *const);
void full_trace_vec2_add_in(const char* ___file___, uint32_t ___line___, Vec2 *const, const Vec2 *const);
void full_trace_vec2_sub_in(const char* ___file___, uint32_t ___line___, Vec2 *const, const Vec2 *const);
void full_trace_vec2_scale_in(const char* ___file___, uint32_t ___line___, Vec2 *const, const float);
void full_trace_vec2_normalize_in(const char* ___file___, uint32_t ___line___, Vec2 *const);
void full_trace_vec3_init(const char* ___file___, uint32_t ___line___, float, float, float, Vec3 *const);
void full_trace_vec3_add(const char* ___file___, uint32_t ___line___, const Vec3 *const, const Vec3 *const, Vec3 *const);
void full_trace_vec3_sub(const char* ___file___, uint32_t ___line___, const Vec3 *const, const Vec3 *const, Vec3 *const);
void full_trace_vec3_scale(const char* ___file___, uint32_t ___line___, const Vec3 *const, const float, Vec3 *const);
void full_trace_vec3_cross(const char* ___file___, uint32_t ___line___, const Vec3 *const, const Vec3 *const, Vec3 *const);
float full_trace_vec3_dot(const char* ___file___, uint32_t ___line___, const Vec3 *const, const Vec3 *const);
float full_trace_vec3_length(const char* ___file___, uint32_t ___line___, const Vec3 *const);
void full_trace_vec3_normalize(const char* ___file___, uint32_t ___line___, const Vec3 *const, Vec3 *const);
float full_trace_vec3_distance(const char* ___file___, uint32_t ___line___, const Vec3 *const, const Vec3 *const);
void full_trace_vec3_add_in(const char* ___file___, uint32_t ___line___, Vec3 *const, const Vec3 *const);
void full_trace_vec3_sub_in(const char* ___file___, uint32_t ___line___, Vec3 *const, const Vec3 *const);
void full_trace_vec3_scale_in(const char* ___file___, uint32_t ___line___, Vec3 *const, const float);
void full_trace_vec3_normalize_in(const char* ___file___, uint32_t ___line___, Vec3 *const);
void full_trace_vec4_init(const char* ___file___, uint32_t ___line___, float, float, float, float, Vec4 *const);
void full_trace_vec4_add(const char* ___file___, uint32_t ___line___, const Vec4 *const, const Vec4 *const, Vec4 *const);
void full_trace_vec4_sub(const char* ___file___, uint32_t ___line___, const Vec4 *const, const Vec4 *const, Vec4 *const);
void full_trace_vec4_scale(const char* ___file___, uint32_t ___line___, const Vec4 *const, const float, Vec4 *const);
float full_trace_vec4_dot(const char* ___file___, uint32_t ___line___, const Vec4 *const, const Vec4 *const);
float full_trace_vec4_length(const char* ___file___, uint32_t ___line___, const Vec4 *const);
void full_trace_vec4_normalize(const char* ___file___, uint32_t ___line___, const Vec4 *const, Vec4 *const);
float full_trace_vec4_distance(const char* ___file___, uint32_t ___line___, const Vec4 *const, const Vec4 *const);
void full_trace_vec4_add_in(const char* ___file___, uint32_t ___line___, Vec4 *const, const Vec4 *const);
void full_trace_vec4_sub_in(const char* ___file___, uint32_t ___line___, Vec4 *const, const Vec4 *const);
void full_trace_vec4_scale_in(const char* ___file___, uint32_t ___line___, Vec4 *const, const float);
void full_trace_vec4_normalize_in(const char* ___file___, uint32_t ___line___, Vec4 *const);
Error full_trace_string_new(const char* ___file___, uint32_t ___line___, string **);
string * full_trace_string_from(const char* ___file___, uint32_t ___line___, const char *);
string * full_trace_string_clone(const char* ___file___, uint32_t ___line___, const string *);
string * full_trace_string_set_cstr(const char* ___file___, uint32_t ___line___, string *, const char *);
string * full_trace_string_set(const char* ___file___, uint32_t ___line___, string *, const string *);
char * full_trace_string_cstr(const char* ___file___, uint32_t ___line___, const string *);
usize full_trace_string_len(const char* ___file___, uint32_t ___line___, const string *);
usize full_trace_string_size(const char* ___file___, uint32_t ___line___, const string *);
string * full_trace_string_push_back(const char* ___file___, uint32_t ___line___, string *, const string *);
string * full_trace_string_push_back_cstr(const char* ___file___, uint32_t ___line___, string *, const char *);
string * full_trace_string_push_front(const char* ___file___, uint32_t ___line___, string *, const string *);
string * full_trace_string_push_front_cstr(const char* ___file___, uint32_t ___line___, string *, const char *);
string * full_trace_string_insert(const char* ___file___, uint32_t ___line___, string *, const string *, const usize);
string * full_trace_string_insert_cstr(const char* ___file___, uint32_t ___line___, string *, const char *, const usize);
string * full_trace_string_insert_cstr_ex(const char* ___file___, uint32_t ___line___, string *, const char *, const usize);
string * full_trace_string_insert_cstr_by_byte(const char* ___file___, uint32_t ___line___, string *, const char *, const usize);
string * full_trace_string_insert_by_byte(const char* ___file___, uint32_t ___line___, string *, const string *, const usize);
string * full_trace_string_remove(const char* ___file___, uint32_t ___line___, string *, const usize);
string * full_trace_string_remove_by_byte(const char* ___file___, uint32_t ___line___, string *, const usize);
string * full_trace_string_remove_n(const char* ___file___, uint32_t ___line___, string *, const usize, const usize);
string * full_trace_string_remove_n_by_byte(const char* ___file___, uint32_t ___line___, string *, const usize, const usize);
boolean full_trace_string_equals(const char* ___file___, uint32_t ___line___, const string *, const string *);
boolean full_trace_string_equals_cstr(const char* ___file___, uint32_t ___line___, const string *, const char *);
void full_trace_string_free(const char* ___file___, uint32_t ___line___, string *);
void full_trace_string_itr_free(const char* ___file___, uint32_t ___line___, string_itr *);
string_itr * full_trace_string_get_itr(const char* ___file___, uint32_t ___line___, const string *);
u8 full_trace_string_itr_next(const char* ___file___, uint32_t ___line___, string_itr *);
string_slice * full_trace_string_get_slice(const char* ___file___, uint32_t ___line___, const string *, const usize, const usize);
string * full_trace_string_from_slice(const char* ___file___, uint32_t ___line___, const string_slice *);
boolean full_trace_string_equals_slice(const char* ___file___, uint32_t ___line___, string_slice *, string_slice *);
string_slice * full_trace_string_set_slice(const char* ___file___, uint32_t ___line___, string_slice *, const string_slice *);
string * full_trace_string_push_back_slice(const char* ___file___, uint32_t ___line___, string *, const string_slice *);
string * full_trace_string_push_front_slice(const char* ___file___, uint32_t ___line___, string *, const string_slice *);
string * full_trace_string_insert_slice_ex(const char* ___file___, uint32_t ___line___, string *, const string_slice *, const usize);
string * full_trace_string_insert_slice(const char* ___file___, uint32_t ___line___, string *, const string_slice *, const usize);
string * full_trace_string_insert_slice_by_byte(const char* ___file___, uint32_t ___line___, string *, const string_slice *, const usize);
void full_trace_string_slice_free(const char* ___file___, uint32_t ___line___, string_slice *);
Error full_trace_string_utf8_new(const char* ___file___, uint32_t ___line___, string_utf8 **);
Error full_trace_string_utf8_to_string(const char* ___file___, uint32_t ___line___, string **, const string_utf8 *);
Error full_trace_string_utf8_from(const char* ___file___, uint32_t ___line___, string_utf8 **, const char *);
Error full_trace_string_utf8_len(const char* ___file___, uint32_t ___line___, usize *, const string_utf8 *);
Error full_trace_string_utf8_size(const char* ___file___, uint32_t ___line___, usize *, const string_utf8 *);
Error full_trace_string_utf8_clone(const char* ___file___, uint32_t ___line___, string_utf8 **, const string_utf8 *);
Error full_trace_string_utf8_push_back_cstr(const char* ___file___, uint32_t ___line___, string_utf8 *, const char *);
Error full_trace_string_utf8_push_back(const char* ___file___, uint32_t ___line___, string_utf8 *, const string_utf8 *);
Error full_trace_string_utf8_insert(const char* ___file___, uint32_t ___line___, string_utf8 *, const string_utf8 *, const usize);
Error full_trace_string_utf8_push_front(const char* ___file___, uint32_t ___line___, string_utf8 *, const string_utf8 *);
void full_trace_string_utf8_free(const char* ___file___, uint32_t ___line___, string_utf8 *);
UID full_trace_uid_new(const char* ___file___, uint32_t ___line___);
boolean full_trace_vfs_mount_res(const char* ___file___, uint32_t ___line___, const char *, const char *);
boolean full_trace_vfs_unmount_res(const char* ___file___, uint32_t ___line___, const char *);
boolean full_trace_vfs_mount_rfs(const char* ___file___, uint32_t ___line___, const char *);
boolean full_trace_vfs_mount_rfs_whitelist(const char* ___file___, uint32_t ___line___, const char **, u64, const char *);
boolean full_trace_vfs_unmount_rfs(const char* ___file___, uint32_t ___line___);
boolean full_trace_vfs_res_path_exists(const char* ___file___, uint32_t ___line___, const char *);
boolean full_trace_vfs_usr_path_exists(const char* ___file___, uint32_t ___line___, const char *, boolean);
void * full_trace_vfs_res_read_file(const char* ___file___, uint32_t ___line___, const char *, u64 *);
FileStream * full_trace_vfs_res_stream_open(const char* ___file___, uint32_t ___line___, const char *);
void * full_trace_vfs_usr_read_file(const char* ___file___, uint32_t ___line___, const char *, u64 *, boolean);
boolean full_trace_vfs_usr_write_file(const char* ___file___, uint32_t ___line___, const char *, const void *, u64);
FileStream * full_trace_vfs_usr_stream_open(const char* ___file___, uint32_t ___line___, const char *, boolean);
u64 full_trace_vfs_stream_size(const char* ___file___, uint32_t ___line___, FileStream *);
u64 full_trace_vfs_stream_read_n(const char* ___file___, uint32_t ___line___, FileStream *, void *, u64);
void * full_trace_vfs_stream_read_all(const char* ___file___, uint32_t ___line___, FileStream *, u64 *);
u64 full_trace_vfs_stream_write(const char* ___file___, uint32_t ___line___, FileStream *, void *, u64);
boolean full_trace_vfs_stream_seek(const char* ___file___, uint32_t ___line___, FileStream *, FSSeekFrom, u64);
u64 full_trace_vfs_stream_tell(const char* ___file___, uint32_t ___line___, FileStream *, boolean *);
boolean full_trace_vfs_stream_flush(const char* ___file___, uint32_t ___line___, FileStream *);
void full_trace_vfs_stream_close(const char* ___file___, uint32_t ___line___, FileStream *);
boolean full_trace_window_server_register_backend(const char* ___file___, uint32_t ___line___, const char *, WindowServerBackend *);
boolean full_trace_window_server_load_backend(const char* ___file___, uint32_t ___line___, const char *);
WindowServerBackend * full_trace_window_server_backend_new(const char* ___file___, uint32_t ___line___);
boolean full_trace_window_server_backend_free(const char* ___file___, uint32_t ___line___, WindowServerBackend *);
boolean full_trace_window_server_backend_set_function(const char* ___file___, uint32_t ___line___, WindowServerBackend *, const char *, fptr);
fptr full_trace_window_server_backend_get_function(const char* ___file___, uint32_t ___line___, WindowServerBackend *, const char *);
boolean full_trace_render_context_register_backend(const char* ___file___, uint32_t ___line___, const char *, const char *, RenderContextBackend *);
boolean full_trace_render_context_load_backend(const char* ___file___, uint32_t ___line___, const char *, const char *);
RenderContextBackend * full_trace_render_context_backend_new(const char* ___file___, uint32_t ___line___);
boolean full_trace_render_context_backend_free(const char* ___file___, uint32_t ___line___, RenderContextBackend *);
boolean full_trace_render_context_backend_set_function(const char* ___file___, uint32_t ___line___, RenderContextBackend *, const char *, fptr);
fptr full_trace_render_context_backend_get_function(const char* ___file___, uint32_t ___line___, RenderContextBackend *, const char *);
boolean full_trace_render_server_register_backend(const char* ___file___, uint32_t ___line___, const char *, RenderServerBackend *);
boolean full_trace_render_server_load_backend(const char* ___file___, uint32_t ___line___, const char *);
RenderServerBackend * full_trace_render_server_backend_new(const char* ___file___, uint32_t ___line___);
boolean full_trace_render_server_backend_free(const char* ___file___, uint32_t ___line___, RenderServerBackend *);
boolean full_trace_render_server_backend_set_function(const char* ___file___, uint32_t ___line___, RenderServerBackend *, const char *, fptr);
fptr full_trace_render_server_backend_get_function(const char* ___file___, uint32_t ___line___, RenderServerBackend *, const char *);
WindowServerWindow * full_trace_window_server_create_window(const char* ___file___, uint32_t ___line___, const char *, IVec2, WindowServerWindow *);
boolean full_trace_window_server_destroy_window(const char* ___file___, uint32_t ___line___, WindowServerWindow *);
boolean full_trace_window_server_window_set_title(const char* ___file___, uint32_t ___line___, WindowServerWindow *, const char *);
c_str full_trace_window_server_window_get_title(const char* ___file___, uint32_t ___line___, WindowServerWindow *);
boolean full_trace_window_server_window_set_mode(const char* ___file___, uint32_t ___line___, WindowServerWindow *, WindowServerWindowMode);
WindowServerWindowMode full_trace_window_server_window_get_mode(const char* ___file___, uint32_t ___line___, WindowServerWindow *);
boolean full_trace_window_server_window_set_size(const char* ___file___, uint32_t ___line___, WindowServerWindow *, IVec2);
boolean full_trace_window_server_window_get_size(const char* ___file___, uint32_t ___line___, WindowServerWindow *, IVec2 *);
boolean full_trace_window_server_window_set_position(const char* ___file___, uint32_t ___line___, WindowServerWindow *, IVec2);
boolean full_trace_window_server_window_get_position(const char* ___file___, uint32_t ___line___, WindowServerWindow *, IVec2 *);
RenderContextSurface * full_trace_render_context_create_surface(const char* ___file___, uint32_t ___line___, WindowServerWindow *);
boolean full_trace_render_context_destroy_surface(const char* ___file___, uint32_t ___line___, RenderContextSurface *);
boolean full_trace_render_context_surface_make_current(const char* ___file___, uint32_t ___line___, RenderContextSurface *);
boolean full_trace_render_context_surface_present(const char* ___file___, uint32_t ___line___, RenderContextSurface *);
fptr full_trace_render_context_get_proc_addr(const char* ___file___, uint32_t ___line___, const char *);


#if defined(HEAPI_LOAD_IMPL)
    inline void full_trace_set_error(const char* ___file___, uint32_t ___line___, Error err) {
    raw___he_update_full_trace_info("set_error", ___file___, ___line___);
    raw_set_error(err);
    raw___he_update_full_trace_info("", "", -1);
}

inline Error full_trace_get_error(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("get_error", ___file___, ___line___);
    Error result = raw_get_error();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline datetime_handle full_trace_datetime_new(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("datetime_new", ___file___, ___line___);
    datetime_handle result = raw_datetime_new();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_datetime_free(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_free", ___file___, ___line___);
    raw_datetime_free(handle);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_datetime_update(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_update", ___file___, ___line___);
    raw_datetime_update(handle);
    raw___he_update_full_trace_info("", "", -1);
}

inline u16 full_trace_datetime_get_year(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_year", ___file___, ___line___);
    u16 result = raw_datetime_get_year(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u8 full_trace_datetime_get_month(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_month", ___file___, ___line___);
    u8 result = raw_datetime_get_month(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u8 full_trace_datetime_get_day(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_day", ___file___, ___line___);
    u8 result = raw_datetime_get_day(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u8 full_trace_datetime_get_hour(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_hour", ___file___, ___line___);
    u8 result = raw_datetime_get_hour(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u8 full_trace_datetime_get_minute(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_minute", ___file___, ___line___);
    u8 result = raw_datetime_get_minute(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u8 full_trace_datetime_get_second(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_second", ___file___, ___line___);
    u8 result = raw_datetime_get_second(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u32 full_trace_datetime_get_nanosecond(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_nanosecond", ___file___, ___line___);
    u32 result = raw_datetime_get_nanosecond(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u16 full_trace_datetime_get_utc_year(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_utc_year", ___file___, ___line___);
    u16 result = raw_datetime_get_utc_year(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u8 full_trace_datetime_get_utc_month(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_utc_month", ___file___, ___line___);
    u8 result = raw_datetime_get_utc_month(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u8 full_trace_datetime_get_utc_day(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_utc_day", ___file___, ___line___);
    u8 result = raw_datetime_get_utc_day(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u8 full_trace_datetime_get_utc_hour(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_utc_hour", ___file___, ___line___);
    u8 result = raw_datetime_get_utc_hour(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u8 full_trace_datetime_get_utc_minute(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_utc_minute", ___file___, ___line___);
    u8 result = raw_datetime_get_utc_minute(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u8 full_trace_datetime_get_utc_second(const char* ___file___, uint32_t ___line___, datetime_handle handle) {
    raw___he_update_full_trace_info("datetime_get_utc_second", ___file___, ___line___);
    u8 result = raw_datetime_get_utc_second(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void * full_trace_tmalloc(const char* ___file___, uint32_t ___line___, u64 size) {
    raw___he_update_full_trace_info("tmalloc", ___file___, ___line___);
    void * result = raw_tmalloc(size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void * full_trace_trealloc(const char* ___file___, uint32_t ___line___, void * ptr, u64 size) {
    raw___he_update_full_trace_info("trealloc", ___file___, ___line___);
    void * result = raw_trealloc(ptr, size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_tfree(const char* ___file___, uint32_t ___line___, void * ptr) {
    raw___he_update_full_trace_info("tfree", ___file___, ___line___);
    raw_tfree(ptr);
    raw___he_update_full_trace_info("", "", -1);
}

inline u64 full_trace_get_allocated_memory(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("get_allocated_memory", ___file___, ___line___);
    u64 result = raw_get_allocated_memory();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline mutex_handle full_trace_mutex_new(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("mutex_new", ___file___, ___line___);
    mutex_handle result = raw_mutex_new();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline mutex_handle full_trace_mutex_new_recursive(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("mutex_new_recursive", ___file___, ___line___);
    mutex_handle result = raw_mutex_new_recursive();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_mutex_free(const char* ___file___, uint32_t ___line___, mutex_handle handle) {
    raw___he_update_full_trace_info("mutex_free", ___file___, ___line___);
    raw_mutex_free(handle);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_mutex_lock(const char* ___file___, uint32_t ___line___, mutex_handle handle) {
    raw___he_update_full_trace_info("mutex_lock", ___file___, ___line___);
    raw_mutex_lock(handle);
    raw___he_update_full_trace_info("", "", -1);
}

inline boolean full_trace_mutex_try_lock(const char* ___file___, uint32_t ___line___, mutex_handle handle) {
    raw___he_update_full_trace_info("mutex_try_lock", ___file___, ___line___);
    boolean result = raw_mutex_try_lock(handle);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_mutex_unlock(const char* ___file___, uint32_t ___line___, mutex_handle handle) {
    raw___he_update_full_trace_info("mutex_unlock", ___file___, ___line___);
    raw_mutex_unlock(handle);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_auto_free(const char* ___file___, uint32_t ___line___, Object * object) {
    raw___he_update_full_trace_info("auto_free", ___file___, ___line___);
    raw_auto_free(object);
    raw___he_update_full_trace_info("", "", -1);
}

inline Node * full_trace_node_new(const char* ___file___, uint32_t ___line___, const char * name) {
    raw___he_update_full_trace_info("node_new", ___file___, ___line___);
    Node * result = raw_node_new(name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Node * full_trace_from_node(const char* ___file___, uint32_t ___line___, Node * node) {
    raw___he_update_full_trace_info("from_node", ___file___, ___line___);
    Node * result = raw_from_node(node);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_node_set_name(const char* ___file___, uint32_t ___line___, Node * node, const char * name) {
    raw___he_update_full_trace_info("node_set_name", ___file___, ___line___);
    raw_node_set_name(node, name);
    raw___he_update_full_trace_info("", "", -1);
}

inline const char * full_trace_node_get_name(const char* ___file___, uint32_t ___line___, Node * node) {
    raw___he_update_full_trace_info("node_get_name", ___file___, ___line___);
    const char * result = raw_node_get_name(node);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_node_add_child(const char* ___file___, uint32_t ___line___, Node * node, Node * child) {
    raw___he_update_full_trace_info("node_add_child", ___file___, ___line___);
    raw_node_add_child(node, child);
    raw___he_update_full_trace_info("", "", -1);
}

inline boolean full_trace_node_remove_child(const char* ___file___, uint32_t ___line___, Node * node, Node * child) {
    raw___he_update_full_trace_info("node_remove_child", ___file___, ___line___);
    boolean result = raw_node_remove_child(node, child);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_node_remove_child_by_name(const char* ___file___, uint32_t ___line___, Node * node, const char * name) {
    raw___he_update_full_trace_info("node_remove_child_by_name", ___file___, ___line___);
    boolean result = raw_node_remove_child_by_name(node, name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_node_remove_child_by_uid(const char* ___file___, uint32_t ___line___, Node * node, UID uid) {
    raw___he_update_full_trace_info("node_remove_child_by_uid", ___file___, ___line___);
    boolean result = raw_node_remove_child_by_uid(node, uid);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_node_remove_all_children(const char* ___file___, uint32_t ___line___, Node * node) {
    raw___he_update_full_trace_info("node_remove_all_children", ___file___, ___line___);
    boolean result = raw_node_remove_all_children(node);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Node * full_trace_node_get_child_by_name(const char* ___file___, uint32_t ___line___, Node * node, const char * name) {
    raw___he_update_full_trace_info("node_get_child_by_name", ___file___, ___line___);
    Node * result = raw_node_get_child_by_name(node, name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Node * full_trace_node_get_child_by_uid(const char* ___file___, uint32_t ___line___, Node * node, UID uid) {
    raw___he_update_full_trace_info("node_get_child_by_uid", ___file___, ___line___);
    Node * result = raw_node_get_child_by_uid(node, uid);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Node3D * full_trace_node3d_new(const char* ___file___, uint32_t ___line___, const char * name) {
    raw___he_update_full_trace_info("node3d_new", ___file___, ___line___);
    Node3D * result = raw_node3d_new(name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Window * full_trace_window_new(const char* ___file___, uint32_t ___line___, const char * name, const char * title, i32 w, i32 h) {
    raw___he_update_full_trace_info("window_new", ___file___, ___line___);
    Window * result = raw_window_new(name, title, w, h);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_window_set_title(const char* ___file___, uint32_t ___line___, Window * this, const char * title) {
    raw___he_update_full_trace_info("window_set_title", ___file___, ___line___);
    raw_window_set_title(this, title);
    raw___he_update_full_trace_info("", "", -1);
}

inline const char * full_trace_window_get_title(const char* ___file___, uint32_t ___line___, const Window * this) {
    raw___he_update_full_trace_info("window_get_title", ___file___, ___line___);
    const char * result = raw_window_get_title(this);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline CanvasItem * full_trace_canvas_item_new(const char* ___file___, uint32_t ___line___, const char * name) {
    raw___he_update_full_trace_info("canvas_item_new", ___file___, ___line___);
    CanvasItem * result = raw_canvas_item_new(name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Control * full_trace_control_new(const char* ___file___, uint32_t ___line___, const char * name) {
    raw___he_update_full_trace_info("control_new", ___file___, ___line___);
    Control * result = raw_control_new(name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Viewport * full_trace_viewport_new(const char* ___file___, uint32_t ___line___, const char * name) {
    raw___he_update_full_trace_info("viewport_new", ___file___, ___line___);
    Viewport * result = raw_viewport_new(name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_mat4_init(const char* ___file___, uint32_t ___line___, float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33, Mat4 *const out) {
    raw___he_update_full_trace_info("mat4_init", ___file___, ___line___);
    raw_mat4_init(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_mat4_from_array(const char* ___file___, uint32_t ___line___, const float *const array, Mat4 *const out) {
    raw___he_update_full_trace_info("mat4_from_array", ___file___, ___line___);
    raw_mat4_from_array(array, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_mat4_clone(const char* ___file___, uint32_t ___line___, const Mat4 *const a, Mat4 *const out) {
    raw___he_update_full_trace_info("mat4_clone", ___file___, ___line___);
    raw_mat4_clone(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_mat4_add(const char* ___file___, uint32_t ___line___, const Mat4 *const a, const Mat4 *const b, Mat4 *const out) {
    raw___he_update_full_trace_info("mat4_add", ___file___, ___line___);
    raw_mat4_add(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_mat4_sub(const char* ___file___, uint32_t ___line___, const Mat4 *const a, const Mat4 *const b, Mat4 *const out) {
    raw___he_update_full_trace_info("mat4_sub", ___file___, ___line___);
    raw_mat4_sub(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_mat4_mul(const char* ___file___, uint32_t ___line___, const Mat4 *const a, const Mat4 *const b, Mat4 *const out) {
    raw___he_update_full_trace_info("mat4_mul", ___file___, ___line___);
    raw_mat4_mul(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_mat4_scale(const char* ___file___, uint32_t ___line___, const Mat4 *const a, const float factor, Mat4 *const out) {
    raw___he_update_full_trace_info("mat4_scale", ___file___, ___line___);
    raw_mat4_scale(a, factor, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_mat4_transpose(const char* ___file___, uint32_t ___line___, const Mat4 *const a, Mat4 *const out) {
    raw___he_update_full_trace_info("mat4_transpose", ___file___, ___line___);
    raw_mat4_transpose(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_mat4_inverse(const char* ___file___, uint32_t ___line___, const Mat4 *const a, Mat4 *const out) {
    raw___he_update_full_trace_info("mat4_inverse", ___file___, ___line___);
    raw_mat4_inverse(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec2_init(const char* ___file___, uint32_t ___line___, const i32 x, const i32 y, IVec2 *const out) {
    raw___he_update_full_trace_info("ivec2_init", ___file___, ___line___);
    raw_ivec2_init(x, y, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec2_add(const char* ___file___, uint32_t ___line___, const IVec2 *const a, const IVec2 *const b, IVec2 *const out) {
    raw___he_update_full_trace_info("ivec2_add", ___file___, ___line___);
    raw_ivec2_add(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec2_sub(const char* ___file___, uint32_t ___line___, const IVec2 *const a, const IVec2 *const b, IVec2 *const out) {
    raw___he_update_full_trace_info("ivec2_sub", ___file___, ___line___);
    raw_ivec2_sub(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec2_scale(const char* ___file___, uint32_t ___line___, const IVec2 *const a, const float factor, IVec2 *const out) {
    raw___he_update_full_trace_info("ivec2_scale", ___file___, ___line___);
    raw_ivec2_scale(a, factor, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_ivec2_dot(const char* ___file___, uint32_t ___line___, const IVec2 *const a, const IVec2 *const b) {
    raw___he_update_full_trace_info("ivec2_dot", ___file___, ___line___);
    float result = raw_ivec2_dot(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_ivec2_length(const char* ___file___, uint32_t ___line___, const IVec2 *const a) {
    raw___he_update_full_trace_info("ivec2_length", ___file___, ___line___);
    float result = raw_ivec2_length(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_ivec2_normalize(const char* ___file___, uint32_t ___line___, const IVec2 *const a, IVec2 *const out) {
    raw___he_update_full_trace_info("ivec2_normalize", ___file___, ___line___);
    raw_ivec2_normalize(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_ivec2_distance(const char* ___file___, uint32_t ___line___, const IVec2 *const a, const IVec2 *const b) {
    raw___he_update_full_trace_info("ivec2_distance", ___file___, ___line___);
    float result = raw_ivec2_distance(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_ivec2_add_in(const char* ___file___, uint32_t ___line___, IVec2 *const to, const IVec2 *const what) {
    raw___he_update_full_trace_info("ivec2_add_in", ___file___, ___line___);
    raw_ivec2_add_in(to, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec2_sub_in(const char* ___file___, uint32_t ___line___, IVec2 *const from, const IVec2 *const what) {
    raw___he_update_full_trace_info("ivec2_sub_in", ___file___, ___line___);
    raw_ivec2_sub_in(from, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec2_scale_in(const char* ___file___, uint32_t ___line___, IVec2 *const to, const float factor) {
    raw___he_update_full_trace_info("ivec2_scale_in", ___file___, ___line___);
    raw_ivec2_scale_in(to, factor);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec2_normalize_in(const char* ___file___, uint32_t ___line___, IVec2 *const a) {
    raw___he_update_full_trace_info("ivec2_normalize_in", ___file___, ___line___);
    raw_ivec2_normalize_in(a);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec3_init(const char* ___file___, uint32_t ___line___, i32 x, i32 y, i32 z, IVec3 *const out) {
    raw___he_update_full_trace_info("ivec3_init", ___file___, ___line___);
    raw_ivec3_init(x, y, z, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec3_add(const char* ___file___, uint32_t ___line___, const IVec3 *const a, const IVec3 *const b, IVec3 *const out) {
    raw___he_update_full_trace_info("ivec3_add", ___file___, ___line___);
    raw_ivec3_add(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec3_sub(const char* ___file___, uint32_t ___line___, const IVec3 *const a, const IVec3 *const b, IVec3 *const out) {
    raw___he_update_full_trace_info("ivec3_sub", ___file___, ___line___);
    raw_ivec3_sub(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec3_scale(const char* ___file___, uint32_t ___line___, const IVec3 *const a, const float factor, IVec3 *const out) {
    raw___he_update_full_trace_info("ivec3_scale", ___file___, ___line___);
    raw_ivec3_scale(a, factor, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec3_cross(const char* ___file___, uint32_t ___line___, const IVec3 *const a, const IVec3 *const b, IVec3 *const out) {
    raw___he_update_full_trace_info("ivec3_cross", ___file___, ___line___);
    raw_ivec3_cross(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_ivec3_dot(const char* ___file___, uint32_t ___line___, const IVec3 *const a, const IVec3 *const b) {
    raw___he_update_full_trace_info("ivec3_dot", ___file___, ___line___);
    float result = raw_ivec3_dot(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_ivec3_length(const char* ___file___, uint32_t ___line___, const IVec3 *const a) {
    raw___he_update_full_trace_info("ivec3_length", ___file___, ___line___);
    float result = raw_ivec3_length(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_ivec3_normalize(const char* ___file___, uint32_t ___line___, const IVec3 *const a, IVec3 *const out) {
    raw___he_update_full_trace_info("ivec3_normalize", ___file___, ___line___);
    raw_ivec3_normalize(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_ivec3_distance(const char* ___file___, uint32_t ___line___, const IVec3 *const a, const IVec3 *const b) {
    raw___he_update_full_trace_info("ivec3_distance", ___file___, ___line___);
    float result = raw_ivec3_distance(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_ivec3_add_in(const char* ___file___, uint32_t ___line___, IVec3 *const to, const IVec3 *const what) {
    raw___he_update_full_trace_info("ivec3_add_in", ___file___, ___line___);
    raw_ivec3_add_in(to, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec3_sub_in(const char* ___file___, uint32_t ___line___, IVec3 *const from, const IVec3 *const what) {
    raw___he_update_full_trace_info("ivec3_sub_in", ___file___, ___line___);
    raw_ivec3_sub_in(from, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec3_scale_in(const char* ___file___, uint32_t ___line___, IVec3 *const to, const float factor) {
    raw___he_update_full_trace_info("ivec3_scale_in", ___file___, ___line___);
    raw_ivec3_scale_in(to, factor);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec3_normalize_in(const char* ___file___, uint32_t ___line___, IVec3 *const a) {
    raw___he_update_full_trace_info("ivec3_normalize_in", ___file___, ___line___);
    raw_ivec3_normalize_in(a);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec4_init(const char* ___file___, uint32_t ___line___, i32 x, i32 y, i32 z, i32 w, IVec4 *const out) {
    raw___he_update_full_trace_info("ivec4_init", ___file___, ___line___);
    raw_ivec4_init(x, y, z, w, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec4_add(const char* ___file___, uint32_t ___line___, const IVec4 *const a, const IVec4 *const b, IVec4 *const out) {
    raw___he_update_full_trace_info("ivec4_add", ___file___, ___line___);
    raw_ivec4_add(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec4_sub(const char* ___file___, uint32_t ___line___, const IVec4 *const a, const IVec4 *const b, IVec4 *const out) {
    raw___he_update_full_trace_info("ivec4_sub", ___file___, ___line___);
    raw_ivec4_sub(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec4_scale(const char* ___file___, uint32_t ___line___, const IVec4 *const a, const float factor, IVec4 *const out) {
    raw___he_update_full_trace_info("ivec4_scale", ___file___, ___line___);
    raw_ivec4_scale(a, factor, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_ivec4_dot(const char* ___file___, uint32_t ___line___, const IVec4 *const a, const IVec4 *const b) {
    raw___he_update_full_trace_info("ivec4_dot", ___file___, ___line___);
    float result = raw_ivec4_dot(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_ivec4_length(const char* ___file___, uint32_t ___line___, const IVec4 *const a) {
    raw___he_update_full_trace_info("ivec4_length", ___file___, ___line___);
    float result = raw_ivec4_length(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_ivec4_normalize(const char* ___file___, uint32_t ___line___, const IVec4 *const a, IVec4 *const out) {
    raw___he_update_full_trace_info("ivec4_normalize", ___file___, ___line___);
    raw_ivec4_normalize(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_ivec4_distance(const char* ___file___, uint32_t ___line___, const IVec4 *const a, const IVec4 *const b) {
    raw___he_update_full_trace_info("ivec4_distance", ___file___, ___line___);
    float result = raw_ivec4_distance(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_ivec4_add_in(const char* ___file___, uint32_t ___line___, IVec4 *const to, const IVec4 *const what) {
    raw___he_update_full_trace_info("ivec4_add_in", ___file___, ___line___);
    raw_ivec4_add_in(to, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec4_sub_in(const char* ___file___, uint32_t ___line___, IVec4 *const from, const IVec4 *const what) {
    raw___he_update_full_trace_info("ivec4_sub_in", ___file___, ___line___);
    raw_ivec4_sub_in(from, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec4_scale_in(const char* ___file___, uint32_t ___line___, IVec4 *const to, const float factor) {
    raw___he_update_full_trace_info("ivec4_scale_in", ___file___, ___line___);
    raw_ivec4_scale_in(to, factor);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_ivec4_normalize_in(const char* ___file___, uint32_t ___line___, IVec4 *const a) {
    raw___he_update_full_trace_info("ivec4_normalize_in", ___file___, ___line___);
    raw_ivec4_normalize_in(a);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec2_init(const char* ___file___, uint32_t ___line___, const u32 x, const u32 y, UVec2 *const out) {
    raw___he_update_full_trace_info("uvec2_init", ___file___, ___line___);
    raw_uvec2_init(x, y, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec2_add(const char* ___file___, uint32_t ___line___, const UVec2 *const a, const UVec2 *const b, UVec2 *const out) {
    raw___he_update_full_trace_info("uvec2_add", ___file___, ___line___);
    raw_uvec2_add(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec2_sub(const char* ___file___, uint32_t ___line___, const UVec2 *const a, const UVec2 *const b, UVec2 *const out) {
    raw___he_update_full_trace_info("uvec2_sub", ___file___, ___line___);
    raw_uvec2_sub(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec2_scale(const char* ___file___, uint32_t ___line___, const UVec2 *const a, const float factor, UVec2 *const out) {
    raw___he_update_full_trace_info("uvec2_scale", ___file___, ___line___);
    raw_uvec2_scale(a, factor, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_uvec2_dot(const char* ___file___, uint32_t ___line___, const UVec2 *const a, const UVec2 *const b) {
    raw___he_update_full_trace_info("uvec2_dot", ___file___, ___line___);
    float result = raw_uvec2_dot(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_uvec2_length(const char* ___file___, uint32_t ___line___, const UVec2 *const a) {
    raw___he_update_full_trace_info("uvec2_length", ___file___, ___line___);
    float result = raw_uvec2_length(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_uvec2_normalize(const char* ___file___, uint32_t ___line___, const UVec2 *const a, UVec2 *const out) {
    raw___he_update_full_trace_info("uvec2_normalize", ___file___, ___line___);
    raw_uvec2_normalize(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_uvec2_distance(const char* ___file___, uint32_t ___line___, const UVec2 *const a, const UVec2 *const b) {
    raw___he_update_full_trace_info("uvec2_distance", ___file___, ___line___);
    float result = raw_uvec2_distance(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_uvec2_add_in(const char* ___file___, uint32_t ___line___, UVec2 *const to, const UVec2 *const what) {
    raw___he_update_full_trace_info("uvec2_add_in", ___file___, ___line___);
    raw_uvec2_add_in(to, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec2_sub_in(const char* ___file___, uint32_t ___line___, UVec2 *const from, const UVec2 *const what) {
    raw___he_update_full_trace_info("uvec2_sub_in", ___file___, ___line___);
    raw_uvec2_sub_in(from, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec2_scale_in(const char* ___file___, uint32_t ___line___, UVec2 *const to, const float factor) {
    raw___he_update_full_trace_info("uvec2_scale_in", ___file___, ___line___);
    raw_uvec2_scale_in(to, factor);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec2_normalize_in(const char* ___file___, uint32_t ___line___, UVec2 *const a) {
    raw___he_update_full_trace_info("uvec2_normalize_in", ___file___, ___line___);
    raw_uvec2_normalize_in(a);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec3_init(const char* ___file___, uint32_t ___line___, u32 x, u32 y, u32 z, UVec3 *const out) {
    raw___he_update_full_trace_info("uvec3_init", ___file___, ___line___);
    raw_uvec3_init(x, y, z, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec3_add(const char* ___file___, uint32_t ___line___, const UVec3 *const a, const UVec3 *const b, UVec3 *const out) {
    raw___he_update_full_trace_info("uvec3_add", ___file___, ___line___);
    raw_uvec3_add(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec3_sub(const char* ___file___, uint32_t ___line___, const UVec3 *const a, const UVec3 *const b, UVec3 *const out) {
    raw___he_update_full_trace_info("uvec3_sub", ___file___, ___line___);
    raw_uvec3_sub(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec3_scale(const char* ___file___, uint32_t ___line___, const UVec3 *const a, const float factor, UVec3 *const out) {
    raw___he_update_full_trace_info("uvec3_scale", ___file___, ___line___);
    raw_uvec3_scale(a, factor, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec3_cross(const char* ___file___, uint32_t ___line___, const UVec3 *const a, const UVec3 *const b, UVec3 *const out) {
    raw___he_update_full_trace_info("uvec3_cross", ___file___, ___line___);
    raw_uvec3_cross(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_uvec3_dot(const char* ___file___, uint32_t ___line___, const UVec3 *const a, const UVec3 *const b) {
    raw___he_update_full_trace_info("uvec3_dot", ___file___, ___line___);
    float result = raw_uvec3_dot(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_uvec3_length(const char* ___file___, uint32_t ___line___, const UVec3 *const a) {
    raw___he_update_full_trace_info("uvec3_length", ___file___, ___line___);
    float result = raw_uvec3_length(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_uvec3_normalize(const char* ___file___, uint32_t ___line___, const UVec3 *const a, UVec3 *const out) {
    raw___he_update_full_trace_info("uvec3_normalize", ___file___, ___line___);
    raw_uvec3_normalize(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_uvec3_distance(const char* ___file___, uint32_t ___line___, const UVec3 *const a, const UVec3 *const b) {
    raw___he_update_full_trace_info("uvec3_distance", ___file___, ___line___);
    float result = raw_uvec3_distance(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_uvec3_add_in(const char* ___file___, uint32_t ___line___, UVec3 *const to, const UVec3 *const what) {
    raw___he_update_full_trace_info("uvec3_add_in", ___file___, ___line___);
    raw_uvec3_add_in(to, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec3_sub_in(const char* ___file___, uint32_t ___line___, UVec3 *const from, const UVec3 *const what) {
    raw___he_update_full_trace_info("uvec3_sub_in", ___file___, ___line___);
    raw_uvec3_sub_in(from, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec3_scale_in(const char* ___file___, uint32_t ___line___, UVec3 *const to, const float factor) {
    raw___he_update_full_trace_info("uvec3_scale_in", ___file___, ___line___);
    raw_uvec3_scale_in(to, factor);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec3_normalize_in(const char* ___file___, uint32_t ___line___, UVec3 *const a) {
    raw___he_update_full_trace_info("uvec3_normalize_in", ___file___, ___line___);
    raw_uvec3_normalize_in(a);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec4_init(const char* ___file___, uint32_t ___line___, u32 x, u32 y, u32 z, u32 w, UVec4 *const out) {
    raw___he_update_full_trace_info("uvec4_init", ___file___, ___line___);
    raw_uvec4_init(x, y, z, w, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec4_add(const char* ___file___, uint32_t ___line___, const UVec4 *const a, const UVec4 *const b, UVec4 *const out) {
    raw___he_update_full_trace_info("uvec4_add", ___file___, ___line___);
    raw_uvec4_add(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec4_sub(const char* ___file___, uint32_t ___line___, const UVec4 *const a, const UVec4 *const b, UVec4 *const out) {
    raw___he_update_full_trace_info("uvec4_sub", ___file___, ___line___);
    raw_uvec4_sub(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec4_scale(const char* ___file___, uint32_t ___line___, const UVec4 *const a, const float factor, UVec4 *const out) {
    raw___he_update_full_trace_info("uvec4_scale", ___file___, ___line___);
    raw_uvec4_scale(a, factor, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_uvec4_dot(const char* ___file___, uint32_t ___line___, const UVec4 *const a, const UVec4 *const b) {
    raw___he_update_full_trace_info("uvec4_dot", ___file___, ___line___);
    float result = raw_uvec4_dot(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_uvec4_length(const char* ___file___, uint32_t ___line___, const UVec4 *const a) {
    raw___he_update_full_trace_info("uvec4_length", ___file___, ___line___);
    float result = raw_uvec4_length(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_uvec4_normalize(const char* ___file___, uint32_t ___line___, const UVec4 *const a, UVec4 *const out) {
    raw___he_update_full_trace_info("uvec4_normalize", ___file___, ___line___);
    raw_uvec4_normalize(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_uvec4_distance(const char* ___file___, uint32_t ___line___, const UVec4 *const a, const UVec4 *const b) {
    raw___he_update_full_trace_info("uvec4_distance", ___file___, ___line___);
    float result = raw_uvec4_distance(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_uvec4_add_in(const char* ___file___, uint32_t ___line___, UVec4 *const to, const UVec4 *const what) {
    raw___he_update_full_trace_info("uvec4_add_in", ___file___, ___line___);
    raw_uvec4_add_in(to, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec4_sub_in(const char* ___file___, uint32_t ___line___, UVec4 *const from, const UVec4 *const what) {
    raw___he_update_full_trace_info("uvec4_sub_in", ___file___, ___line___);
    raw_uvec4_sub_in(from, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec4_scale_in(const char* ___file___, uint32_t ___line___, UVec4 *const to, const float factor) {
    raw___he_update_full_trace_info("uvec4_scale_in", ___file___, ___line___);
    raw_uvec4_scale_in(to, factor);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_uvec4_normalize_in(const char* ___file___, uint32_t ___line___, UVec4 *const a) {
    raw___he_update_full_trace_info("uvec4_normalize_in", ___file___, ___line___);
    raw_uvec4_normalize_in(a);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec2_init(const char* ___file___, uint32_t ___line___, const float x, const float y, Vec2 *const out) {
    raw___he_update_full_trace_info("vec2_init", ___file___, ___line___);
    raw_vec2_init(x, y, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec2_add(const char* ___file___, uint32_t ___line___, const Vec2 *const a, const Vec2 *const b, Vec2 *const out) {
    raw___he_update_full_trace_info("vec2_add", ___file___, ___line___);
    raw_vec2_add(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec2_sub(const char* ___file___, uint32_t ___line___, const Vec2 *const a, const Vec2 *const b, Vec2 *const out) {
    raw___he_update_full_trace_info("vec2_sub", ___file___, ___line___);
    raw_vec2_sub(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec2_scale(const char* ___file___, uint32_t ___line___, const Vec2 *const a, const float factor, Vec2 *const out) {
    raw___he_update_full_trace_info("vec2_scale", ___file___, ___line___);
    raw_vec2_scale(a, factor, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_vec2_dot(const char* ___file___, uint32_t ___line___, const Vec2 *const a, const Vec2 *const b) {
    raw___he_update_full_trace_info("vec2_dot", ___file___, ___line___);
    float result = raw_vec2_dot(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_vec2_length(const char* ___file___, uint32_t ___line___, const Vec2 *const a) {
    raw___he_update_full_trace_info("vec2_length", ___file___, ___line___);
    float result = raw_vec2_length(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_vec2_normalize(const char* ___file___, uint32_t ___line___, const Vec2 *const a, Vec2 *const out) {
    raw___he_update_full_trace_info("vec2_normalize", ___file___, ___line___);
    raw_vec2_normalize(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_vec2_distance(const char* ___file___, uint32_t ___line___, const Vec2 *const a, const Vec2 *const b) {
    raw___he_update_full_trace_info("vec2_distance", ___file___, ___line___);
    float result = raw_vec2_distance(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_vec2_add_in(const char* ___file___, uint32_t ___line___, Vec2 *const to, const Vec2 *const what) {
    raw___he_update_full_trace_info("vec2_add_in", ___file___, ___line___);
    raw_vec2_add_in(to, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec2_sub_in(const char* ___file___, uint32_t ___line___, Vec2 *const from, const Vec2 *const what) {
    raw___he_update_full_trace_info("vec2_sub_in", ___file___, ___line___);
    raw_vec2_sub_in(from, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec2_scale_in(const char* ___file___, uint32_t ___line___, Vec2 *const to, const float factor) {
    raw___he_update_full_trace_info("vec2_scale_in", ___file___, ___line___);
    raw_vec2_scale_in(to, factor);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec2_normalize_in(const char* ___file___, uint32_t ___line___, Vec2 *const a) {
    raw___he_update_full_trace_info("vec2_normalize_in", ___file___, ___line___);
    raw_vec2_normalize_in(a);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec3_init(const char* ___file___, uint32_t ___line___, float x, float y, float z, Vec3 *const out) {
    raw___he_update_full_trace_info("vec3_init", ___file___, ___line___);
    raw_vec3_init(x, y, z, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec3_add(const char* ___file___, uint32_t ___line___, const Vec3 *const a, const Vec3 *const b, Vec3 *const out) {
    raw___he_update_full_trace_info("vec3_add", ___file___, ___line___);
    raw_vec3_add(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec3_sub(const char* ___file___, uint32_t ___line___, const Vec3 *const a, const Vec3 *const b, Vec3 *const out) {
    raw___he_update_full_trace_info("vec3_sub", ___file___, ___line___);
    raw_vec3_sub(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec3_scale(const char* ___file___, uint32_t ___line___, const Vec3 *const a, const float factor, Vec3 *const out) {
    raw___he_update_full_trace_info("vec3_scale", ___file___, ___line___);
    raw_vec3_scale(a, factor, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec3_cross(const char* ___file___, uint32_t ___line___, const Vec3 *const a, const Vec3 *const b, Vec3 *const out) {
    raw___he_update_full_trace_info("vec3_cross", ___file___, ___line___);
    raw_vec3_cross(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_vec3_dot(const char* ___file___, uint32_t ___line___, const Vec3 *const a, const Vec3 *const b) {
    raw___he_update_full_trace_info("vec3_dot", ___file___, ___line___);
    float result = raw_vec3_dot(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_vec3_length(const char* ___file___, uint32_t ___line___, const Vec3 *const a) {
    raw___he_update_full_trace_info("vec3_length", ___file___, ___line___);
    float result = raw_vec3_length(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_vec3_normalize(const char* ___file___, uint32_t ___line___, const Vec3 *const a, Vec3 *const out) {
    raw___he_update_full_trace_info("vec3_normalize", ___file___, ___line___);
    raw_vec3_normalize(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_vec3_distance(const char* ___file___, uint32_t ___line___, const Vec3 *const a, const Vec3 *const b) {
    raw___he_update_full_trace_info("vec3_distance", ___file___, ___line___);
    float result = raw_vec3_distance(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_vec3_add_in(const char* ___file___, uint32_t ___line___, Vec3 *const to, const Vec3 *const what) {
    raw___he_update_full_trace_info("vec3_add_in", ___file___, ___line___);
    raw_vec3_add_in(to, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec3_sub_in(const char* ___file___, uint32_t ___line___, Vec3 *const from, const Vec3 *const what) {
    raw___he_update_full_trace_info("vec3_sub_in", ___file___, ___line___);
    raw_vec3_sub_in(from, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec3_scale_in(const char* ___file___, uint32_t ___line___, Vec3 *const to, const float factor) {
    raw___he_update_full_trace_info("vec3_scale_in", ___file___, ___line___);
    raw_vec3_scale_in(to, factor);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec3_normalize_in(const char* ___file___, uint32_t ___line___, Vec3 *const a) {
    raw___he_update_full_trace_info("vec3_normalize_in", ___file___, ___line___);
    raw_vec3_normalize_in(a);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec4_init(const char* ___file___, uint32_t ___line___, float x, float y, float z, float w, Vec4 *const out) {
    raw___he_update_full_trace_info("vec4_init", ___file___, ___line___);
    raw_vec4_init(x, y, z, w, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec4_add(const char* ___file___, uint32_t ___line___, const Vec4 *const a, const Vec4 *const b, Vec4 *const out) {
    raw___he_update_full_trace_info("vec4_add", ___file___, ___line___);
    raw_vec4_add(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec4_sub(const char* ___file___, uint32_t ___line___, const Vec4 *const a, const Vec4 *const b, Vec4 *const out) {
    raw___he_update_full_trace_info("vec4_sub", ___file___, ___line___);
    raw_vec4_sub(a, b, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec4_scale(const char* ___file___, uint32_t ___line___, const Vec4 *const a, const float factor, Vec4 *const out) {
    raw___he_update_full_trace_info("vec4_scale", ___file___, ___line___);
    raw_vec4_scale(a, factor, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_vec4_dot(const char* ___file___, uint32_t ___line___, const Vec4 *const a, const Vec4 *const b) {
    raw___he_update_full_trace_info("vec4_dot", ___file___, ___line___);
    float result = raw_vec4_dot(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_vec4_length(const char* ___file___, uint32_t ___line___, const Vec4 *const a) {
    raw___he_update_full_trace_info("vec4_length", ___file___, ___line___);
    float result = raw_vec4_length(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_vec4_normalize(const char* ___file___, uint32_t ___line___, const Vec4 *const a, Vec4 *const out) {
    raw___he_update_full_trace_info("vec4_normalize", ___file___, ___line___);
    raw_vec4_normalize(a, out);
    raw___he_update_full_trace_info("", "", -1);
}

inline float full_trace_vec4_distance(const char* ___file___, uint32_t ___line___, const Vec4 *const a, const Vec4 *const b) {
    raw___he_update_full_trace_info("vec4_distance", ___file___, ___line___);
    float result = raw_vec4_distance(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_vec4_add_in(const char* ___file___, uint32_t ___line___, Vec4 *const to, const Vec4 *const what) {
    raw___he_update_full_trace_info("vec4_add_in", ___file___, ___line___);
    raw_vec4_add_in(to, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec4_sub_in(const char* ___file___, uint32_t ___line___, Vec4 *const from, const Vec4 *const what) {
    raw___he_update_full_trace_info("vec4_sub_in", ___file___, ___line___);
    raw_vec4_sub_in(from, what);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec4_scale_in(const char* ___file___, uint32_t ___line___, Vec4 *const to, const float factor) {
    raw___he_update_full_trace_info("vec4_scale_in", ___file___, ___line___);
    raw_vec4_scale_in(to, factor);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec4_normalize_in(const char* ___file___, uint32_t ___line___, Vec4 *const a) {
    raw___he_update_full_trace_info("vec4_normalize_in", ___file___, ___line___);
    raw_vec4_normalize_in(a);
    raw___he_update_full_trace_info("", "", -1);
}

inline Error full_trace_string_new(const char* ___file___, uint32_t ___line___, string ** str) {
    raw___he_update_full_trace_info("string_new", ___file___, ___line___);
    Error result = raw_string_new(str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_from(const char* ___file___, uint32_t ___line___, const char * c_str) {
    raw___he_update_full_trace_info("string_from", ___file___, ___line___);
    string * result = raw_string_from(c_str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_clone(const char* ___file___, uint32_t ___line___, const string * c_str) {
    raw___he_update_full_trace_info("string_clone", ___file___, ___line___);
    string * result = raw_string_clone(c_str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_set_cstr(const char* ___file___, uint32_t ___line___, string * self, const char * c_cstr) {
    raw___he_update_full_trace_info("string_set_cstr", ___file___, ___line___);
    string * result = raw_string_set_cstr(self, c_cstr);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_set(const char* ___file___, uint32_t ___line___, string * self, const string * c_str) {
    raw___he_update_full_trace_info("string_set", ___file___, ___line___);
    string * result = raw_string_set(self, c_str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline char * full_trace_string_cstr(const char* ___file___, uint32_t ___line___, const string * c_str) {
    raw___he_update_full_trace_info("string_cstr", ___file___, ___line___);
    char * result = raw_string_cstr(c_str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline usize full_trace_string_len(const char* ___file___, uint32_t ___line___, const string * c_str) {
    raw___he_update_full_trace_info("string_len", ___file___, ___line___);
    usize result = raw_string_len(c_str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline usize full_trace_string_size(const char* ___file___, uint32_t ___line___, const string * c_str) {
    raw___he_update_full_trace_info("string_size", ___file___, ___line___);
    usize result = raw_string_size(c_str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_push_back(const char* ___file___, uint32_t ___line___, string * dest, const string * src) {
    raw___he_update_full_trace_info("string_push_back", ___file___, ___line___);
    string * result = raw_string_push_back(dest, src);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_push_back_cstr(const char* ___file___, uint32_t ___line___, string * dest, const char * src) {
    raw___he_update_full_trace_info("string_push_back_cstr", ___file___, ___line___);
    string * result = raw_string_push_back_cstr(dest, src);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_push_front(const char* ___file___, uint32_t ___line___, string * dest, const string * src) {
    raw___he_update_full_trace_info("string_push_front", ___file___, ___line___);
    string * result = raw_string_push_front(dest, src);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_push_front_cstr(const char* ___file___, uint32_t ___line___, string * dest, const char * src) {
    raw___he_update_full_trace_info("string_push_front_cstr", ___file___, ___line___);
    string * result = raw_string_push_front_cstr(dest, src);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_insert(const char* ___file___, uint32_t ___line___, string * dest, const string * src, const usize i) {
    raw___he_update_full_trace_info("string_insert", ___file___, ___line___);
    string * result = raw_string_insert(dest, src, i);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_insert_cstr(const char* ___file___, uint32_t ___line___, string * dest, const char * src, const usize i) {
    raw___he_update_full_trace_info("string_insert_cstr", ___file___, ___line___);
    string * result = raw_string_insert_cstr(dest, src, i);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_insert_cstr_ex(const char* ___file___, uint32_t ___line___, string * dest, const char * src, const usize i) {
    raw___he_update_full_trace_info("string_insert_cstr_ex", ___file___, ___line___);
    string * result = raw_string_insert_cstr_ex(dest, src, i);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_insert_cstr_by_byte(const char* ___file___, uint32_t ___line___, string * dest, const char * src, const usize b) {
    raw___he_update_full_trace_info("string_insert_cstr_by_byte", ___file___, ___line___);
    string * result = raw_string_insert_cstr_by_byte(dest, src, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_insert_by_byte(const char* ___file___, uint32_t ___line___, string * dest, const string * src, const usize b) {
    raw___he_update_full_trace_info("string_insert_by_byte", ___file___, ___line___);
    string * result = raw_string_insert_by_byte(dest, src, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_remove(const char* ___file___, uint32_t ___line___, string * self, const usize i) {
    raw___he_update_full_trace_info("string_remove", ___file___, ___line___);
    string * result = raw_string_remove(self, i);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_remove_by_byte(const char* ___file___, uint32_t ___line___, string * self, const usize b) {
    raw___he_update_full_trace_info("string_remove_by_byte", ___file___, ___line___);
    string * result = raw_string_remove_by_byte(self, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_remove_n(const char* ___file___, uint32_t ___line___, string * self, const usize i, const usize n) {
    raw___he_update_full_trace_info("string_remove_n", ___file___, ___line___);
    string * result = raw_string_remove_n(self, i, n);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_remove_n_by_byte(const char* ___file___, uint32_t ___line___, string * self, const usize b, const usize b_n) {
    raw___he_update_full_trace_info("string_remove_n_by_byte", ___file___, ___line___);
    string * result = raw_string_remove_n_by_byte(self, b, b_n);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_string_equals(const char* ___file___, uint32_t ___line___, const string * str1, const string * str2) {
    raw___he_update_full_trace_info("string_equals", ___file___, ___line___);
    boolean result = raw_string_equals(str1, str2);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_string_equals_cstr(const char* ___file___, uint32_t ___line___, const string * str, const char * cstr) {
    raw___he_update_full_trace_info("string_equals_cstr", ___file___, ___line___);
    boolean result = raw_string_equals_cstr(str, cstr);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_string_free(const char* ___file___, uint32_t ___line___, string * str) {
    raw___he_update_full_trace_info("string_free", ___file___, ___line___);
    raw_string_free(str);
    raw___he_update_full_trace_info("", "", -1);
}

inline void full_trace_string_itr_free(const char* ___file___, uint32_t ___line___, string_itr * str_itr) {
    raw___he_update_full_trace_info("string_itr_free", ___file___, ___line___);
    raw_string_itr_free(str_itr);
    raw___he_update_full_trace_info("", "", -1);
}

inline string_itr * full_trace_string_get_itr(const char* ___file___, uint32_t ___line___, const string * c_str) {
    raw___he_update_full_trace_info("string_get_itr", ___file___, ___line___);
    string_itr * result = raw_string_get_itr(c_str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u8 full_trace_string_itr_next(const char* ___file___, uint32_t ___line___, string_itr * str_itr) {
    raw___he_update_full_trace_info("string_itr_next", ___file___, ___line___);
    u8 result = raw_string_itr_next(str_itr);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string_slice * full_trace_string_get_slice(const char* ___file___, uint32_t ___line___, const string * str, const usize s, const usize e) {
    raw___he_update_full_trace_info("string_get_slice", ___file___, ___line___);
    string_slice * result = raw_string_get_slice(str, s, e);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_from_slice(const char* ___file___, uint32_t ___line___, const string_slice * str_sl) {
    raw___he_update_full_trace_info("string_from_slice", ___file___, ___line___);
    string * result = raw_string_from_slice(str_sl);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_string_equals_slice(const char* ___file___, uint32_t ___line___, string_slice * str_sl_1, string_slice * str_sl_2) {
    raw___he_update_full_trace_info("string_equals_slice", ___file___, ___line___);
    boolean result = raw_string_equals_slice(str_sl_1, str_sl_2);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string_slice * full_trace_string_set_slice(const char* ___file___, uint32_t ___line___, string_slice * self, const string_slice * str_sl) {
    raw___he_update_full_trace_info("string_set_slice", ___file___, ___line___);
    string_slice * result = raw_string_set_slice(self, str_sl);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_push_back_slice(const char* ___file___, uint32_t ___line___, string * dest, const string_slice * src) {
    raw___he_update_full_trace_info("string_push_back_slice", ___file___, ___line___);
    string * result = raw_string_push_back_slice(dest, src);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_push_front_slice(const char* ___file___, uint32_t ___line___, string * dest, const string_slice * src) {
    raw___he_update_full_trace_info("string_push_front_slice", ___file___, ___line___);
    string * result = raw_string_push_front_slice(dest, src);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_insert_slice_ex(const char* ___file___, uint32_t ___line___, string * dest, const string_slice * src, const usize i) {
    raw___he_update_full_trace_info("string_insert_slice_ex", ___file___, ___line___);
    string * result = raw_string_insert_slice_ex(dest, src, i);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_insert_slice(const char* ___file___, uint32_t ___line___, string * dest, const string_slice * src, const usize i) {
    raw___he_update_full_trace_info("string_insert_slice", ___file___, ___line___);
    string * result = raw_string_insert_slice(dest, src, i);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline string * full_trace_string_insert_slice_by_byte(const char* ___file___, uint32_t ___line___, string * dest, const string_slice * src, const usize b) {
    raw___he_update_full_trace_info("string_insert_slice_by_byte", ___file___, ___line___);
    string * result = raw_string_insert_slice_by_byte(dest, src, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_string_slice_free(const char* ___file___, uint32_t ___line___, string_slice * str_sl) {
    raw___he_update_full_trace_info("string_slice_free", ___file___, ___line___);
    raw_string_slice_free(str_sl);
    raw___he_update_full_trace_info("", "", -1);
}

inline Error full_trace_string_utf8_new(const char* ___file___, uint32_t ___line___, string_utf8 ** str) {
    raw___he_update_full_trace_info("string_utf8_new", ___file___, ___line___);
    Error result = raw_string_utf8_new(str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_string_utf8_to_string(const char* ___file___, uint32_t ___line___, string ** dest, const string_utf8 * str) {
    raw___he_update_full_trace_info("string_utf8_to_string", ___file___, ___line___);
    Error result = raw_string_utf8_to_string(dest, str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_string_utf8_from(const char* ___file___, uint32_t ___line___, string_utf8 ** str, const char * c_str) {
    raw___he_update_full_trace_info("string_utf8_from", ___file___, ___line___);
    Error result = raw_string_utf8_from(str, c_str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_string_utf8_len(const char* ___file___, uint32_t ___line___, usize * len, const string_utf8 * c_str) {
    raw___he_update_full_trace_info("string_utf8_len", ___file___, ___line___);
    Error result = raw_string_utf8_len(len, c_str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_string_utf8_size(const char* ___file___, uint32_t ___line___, usize * size, const string_utf8 * c_str) {
    raw___he_update_full_trace_info("string_utf8_size", ___file___, ___line___);
    Error result = raw_string_utf8_size(size, c_str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_string_utf8_clone(const char* ___file___, uint32_t ___line___, string_utf8 ** str, const string_utf8 * c_str) {
    raw___he_update_full_trace_info("string_utf8_clone", ___file___, ___line___);
    Error result = raw_string_utf8_clone(str, c_str);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_string_utf8_push_back_cstr(const char* ___file___, uint32_t ___line___, string_utf8 * dest, const char * src) {
    raw___he_update_full_trace_info("string_utf8_push_back_cstr", ___file___, ___line___);
    Error result = raw_string_utf8_push_back_cstr(dest, src);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_string_utf8_push_back(const char* ___file___, uint32_t ___line___, string_utf8 * dest, const string_utf8 * src) {
    raw___he_update_full_trace_info("string_utf8_push_back", ___file___, ___line___);
    Error result = raw_string_utf8_push_back(dest, src);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_string_utf8_insert(const char* ___file___, uint32_t ___line___, string_utf8 * dest, const string_utf8 * src, const usize i) {
    raw___he_update_full_trace_info("string_utf8_insert", ___file___, ___line___);
    Error result = raw_string_utf8_insert(dest, src, i);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_string_utf8_push_front(const char* ___file___, uint32_t ___line___, string_utf8 * dest, const string_utf8 * src) {
    raw___he_update_full_trace_info("string_utf8_push_front", ___file___, ___line___);
    Error result = raw_string_utf8_push_front(dest, src);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_string_utf8_free(const char* ___file___, uint32_t ___line___, string_utf8 * str) {
    raw___he_update_full_trace_info("string_utf8_free", ___file___, ___line___);
    raw_string_utf8_free(str);
    raw___he_update_full_trace_info("", "", -1);
}

inline UID full_trace_uid_new(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("uid_new", ___file___, ___line___);
    UID result = raw_uid_new();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_vfs_mount_res(const char* ___file___, uint32_t ___line___, const char * path, const char * mount_point) {
    raw___he_update_full_trace_info("vfs_mount_res", ___file___, ___line___);
    boolean result = raw_vfs_mount_res(path, mount_point);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_vfs_unmount_res(const char* ___file___, uint32_t ___line___, const char * mount_point) {
    raw___he_update_full_trace_info("vfs_unmount_res", ___file___, ___line___);
    boolean result = raw_vfs_unmount_res(mount_point);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_vfs_mount_rfs(const char* ___file___, uint32_t ___line___, const char * mount_point) {
    raw___he_update_full_trace_info("vfs_mount_rfs", ___file___, ___line___);
    boolean result = raw_vfs_mount_rfs(mount_point);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_vfs_mount_rfs_whitelist(const char* ___file___, uint32_t ___line___, const char ** whitelist, u64 count, const char * mount_point) {
    raw___he_update_full_trace_info("vfs_mount_rfs_whitelist", ___file___, ___line___);
    boolean result = raw_vfs_mount_rfs_whitelist(whitelist, count, mount_point);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_vfs_unmount_rfs(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("vfs_unmount_rfs", ___file___, ___line___);
    boolean result = raw_vfs_unmount_rfs();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_vfs_res_path_exists(const char* ___file___, uint32_t ___line___, const char * path) {
    raw___he_update_full_trace_info("vfs_res_path_exists", ___file___, ___line___);
    boolean result = raw_vfs_res_path_exists(path);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_vfs_usr_path_exists(const char* ___file___, uint32_t ___line___, const char * path, boolean prefer_res) {
    raw___he_update_full_trace_info("vfs_usr_path_exists", ___file___, ___line___);
    boolean result = raw_vfs_usr_path_exists(path, prefer_res);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void * full_trace_vfs_res_read_file(const char* ___file___, uint32_t ___line___, const char * path, u64 * size) {
    raw___he_update_full_trace_info("vfs_res_read_file", ___file___, ___line___);
    void * result = raw_vfs_res_read_file(path, size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline FileStream * full_trace_vfs_res_stream_open(const char* ___file___, uint32_t ___line___, const char * path) {
    raw___he_update_full_trace_info("vfs_res_stream_open", ___file___, ___line___);
    FileStream * result = raw_vfs_res_stream_open(path);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void * full_trace_vfs_usr_read_file(const char* ___file___, uint32_t ___line___, const char * path, u64 * size, boolean prefer_res) {
    raw___he_update_full_trace_info("vfs_usr_read_file", ___file___, ___line___);
    void * result = raw_vfs_usr_read_file(path, size, prefer_res);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_vfs_usr_write_file(const char* ___file___, uint32_t ___line___, const char * path, const void * data, u64 size) {
    raw___he_update_full_trace_info("vfs_usr_write_file", ___file___, ___line___);
    boolean result = raw_vfs_usr_write_file(path, data, size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline FileStream * full_trace_vfs_usr_stream_open(const char* ___file___, uint32_t ___line___, const char * path, boolean prefer_res) {
    raw___he_update_full_trace_info("vfs_usr_stream_open", ___file___, ___line___);
    FileStream * result = raw_vfs_usr_stream_open(path, prefer_res);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u64 full_trace_vfs_stream_size(const char* ___file___, uint32_t ___line___, FileStream * stream) {
    raw___he_update_full_trace_info("vfs_stream_size", ___file___, ___line___);
    u64 result = raw_vfs_stream_size(stream);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u64 full_trace_vfs_stream_read_n(const char* ___file___, uint32_t ___line___, FileStream * stream, void * buffer, u64 size) {
    raw___he_update_full_trace_info("vfs_stream_read_n", ___file___, ___line___);
    u64 result = raw_vfs_stream_read_n(stream, buffer, size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void * full_trace_vfs_stream_read_all(const char* ___file___, uint32_t ___line___, FileStream * stream, u64 * size) {
    raw___he_update_full_trace_info("vfs_stream_read_all", ___file___, ___line___);
    void * result = raw_vfs_stream_read_all(stream, size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u64 full_trace_vfs_stream_write(const char* ___file___, uint32_t ___line___, FileStream * stream, void * buffer, u64 size) {
    raw___he_update_full_trace_info("vfs_stream_write", ___file___, ___line___);
    u64 result = raw_vfs_stream_write(stream, buffer, size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_vfs_stream_seek(const char* ___file___, uint32_t ___line___, FileStream * stream, FSSeekFrom whence, u64 offset) {
    raw___he_update_full_trace_info("vfs_stream_seek", ___file___, ___line___);
    boolean result = raw_vfs_stream_seek(stream, whence, offset);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline u64 full_trace_vfs_stream_tell(const char* ___file___, uint32_t ___line___, FileStream * stream, boolean * success) {
    raw___he_update_full_trace_info("vfs_stream_tell", ___file___, ___line___);
    u64 result = raw_vfs_stream_tell(stream, success);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_vfs_stream_flush(const char* ___file___, uint32_t ___line___, FileStream * stream) {
    raw___he_update_full_trace_info("vfs_stream_flush", ___file___, ___line___);
    boolean result = raw_vfs_stream_flush(stream);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_vfs_stream_close(const char* ___file___, uint32_t ___line___, FileStream * stream) {
    raw___he_update_full_trace_info("vfs_stream_close", ___file___, ___line___);
    raw_vfs_stream_close(stream);
    raw___he_update_full_trace_info("", "", -1);
}

inline boolean full_trace_window_server_register_backend(const char* ___file___, uint32_t ___line___, const char * name, WindowServerBackend * backend) {
    raw___he_update_full_trace_info("window_server_register_backend", ___file___, ___line___);
    boolean result = raw_window_server_register_backend(name, backend);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_window_server_load_backend(const char* ___file___, uint32_t ___line___, const char * name) {
    raw___he_update_full_trace_info("window_server_load_backend", ___file___, ___line___);
    boolean result = raw_window_server_load_backend(name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline WindowServerBackend * full_trace_window_server_backend_new(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("window_server_backend_new", ___file___, ___line___);
    WindowServerBackend * result = raw_window_server_backend_new();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_window_server_backend_free(const char* ___file___, uint32_t ___line___, WindowServerBackend * backend) {
    raw___he_update_full_trace_info("window_server_backend_free", ___file___, ___line___);
    boolean result = raw_window_server_backend_free(backend);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_window_server_backend_set_function(const char* ___file___, uint32_t ___line___, WindowServerBackend * backend, const char * name, fptr function) {
    raw___he_update_full_trace_info("window_server_backend_set_function", ___file___, ___line___);
    boolean result = raw_window_server_backend_set_function(backend, name, function);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline fptr full_trace_window_server_backend_get_function(const char* ___file___, uint32_t ___line___, WindowServerBackend * backend, const char * name) {
    raw___he_update_full_trace_info("window_server_backend_get_function", ___file___, ___line___);
    fptr result = raw_window_server_backend_get_function(backend, name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_render_context_register_backend(const char* ___file___, uint32_t ___line___, const char * render_server_name, const char * window_server_name, RenderContextBackend * backend) {
    raw___he_update_full_trace_info("render_context_register_backend", ___file___, ___line___);
    boolean result = raw_render_context_register_backend(render_server_name, window_server_name, backend);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_render_context_load_backend(const char* ___file___, uint32_t ___line___, const char * render_server_name, const char * window_server_name) {
    raw___he_update_full_trace_info("render_context_load_backend", ___file___, ___line___);
    boolean result = raw_render_context_load_backend(render_server_name, window_server_name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline RenderContextBackend * full_trace_render_context_backend_new(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("render_context_backend_new", ___file___, ___line___);
    RenderContextBackend * result = raw_render_context_backend_new();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_render_context_backend_free(const char* ___file___, uint32_t ___line___, RenderContextBackend * backend) {
    raw___he_update_full_trace_info("render_context_backend_free", ___file___, ___line___);
    boolean result = raw_render_context_backend_free(backend);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_render_context_backend_set_function(const char* ___file___, uint32_t ___line___, RenderContextBackend * backend, const char * name, fptr function) {
    raw___he_update_full_trace_info("render_context_backend_set_function", ___file___, ___line___);
    boolean result = raw_render_context_backend_set_function(backend, name, function);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline fptr full_trace_render_context_backend_get_function(const char* ___file___, uint32_t ___line___, RenderContextBackend * backend, const char * name) {
    raw___he_update_full_trace_info("render_context_backend_get_function", ___file___, ___line___);
    fptr result = raw_render_context_backend_get_function(backend, name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_render_server_register_backend(const char* ___file___, uint32_t ___line___, const char * name, RenderServerBackend * backend) {
    raw___he_update_full_trace_info("render_server_register_backend", ___file___, ___line___);
    boolean result = raw_render_server_register_backend(name, backend);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_render_server_load_backend(const char* ___file___, uint32_t ___line___, const char * name) {
    raw___he_update_full_trace_info("render_server_load_backend", ___file___, ___line___);
    boolean result = raw_render_server_load_backend(name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline RenderServerBackend * full_trace_render_server_backend_new(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("render_server_backend_new", ___file___, ___line___);
    RenderServerBackend * result = raw_render_server_backend_new();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_render_server_backend_free(const char* ___file___, uint32_t ___line___, RenderServerBackend * backend) {
    raw___he_update_full_trace_info("render_server_backend_free", ___file___, ___line___);
    boolean result = raw_render_server_backend_free(backend);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_render_server_backend_set_function(const char* ___file___, uint32_t ___line___, RenderServerBackend * backend, const char * name, fptr function) {
    raw___he_update_full_trace_info("render_server_backend_set_function", ___file___, ___line___);
    boolean result = raw_render_server_backend_set_function(backend, name, function);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline fptr full_trace_render_server_backend_get_function(const char* ___file___, uint32_t ___line___, RenderServerBackend * backend, const char * name) {
    raw___he_update_full_trace_info("render_server_backend_get_function", ___file___, ___line___);
    fptr result = raw_render_server_backend_get_function(backend, name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline WindowServerWindow * full_trace_window_server_create_window(const char* ___file___, uint32_t ___line___, const char * title, IVec2 size, WindowServerWindow * parent) {
    raw___he_update_full_trace_info("window_server_create_window", ___file___, ___line___);
    WindowServerWindow * result = raw_window_server_create_window(title, size, parent);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_window_server_destroy_window(const char* ___file___, uint32_t ___line___, WindowServerWindow * this) {
    raw___he_update_full_trace_info("window_server_destroy_window", ___file___, ___line___);
    boolean result = raw_window_server_destroy_window(this);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_window_server_window_set_title(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, const char * title) {
    raw___he_update_full_trace_info("window_server_window_set_title", ___file___, ___line___);
    boolean result = raw_window_server_window_set_title(this, title);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline c_str full_trace_window_server_window_get_title(const char* ___file___, uint32_t ___line___, WindowServerWindow * this) {
    raw___he_update_full_trace_info("window_server_window_get_title", ___file___, ___line___);
    c_str result = raw_window_server_window_get_title(this);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_window_server_window_set_mode(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, WindowServerWindowMode mode) {
    raw___he_update_full_trace_info("window_server_window_set_mode", ___file___, ___line___);
    boolean result = raw_window_server_window_set_mode(this, mode);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline WindowServerWindowMode full_trace_window_server_window_get_mode(const char* ___file___, uint32_t ___line___, WindowServerWindow * this) {
    raw___he_update_full_trace_info("window_server_window_get_mode", ___file___, ___line___);
    WindowServerWindowMode result = raw_window_server_window_get_mode(this);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_window_server_window_set_size(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, IVec2 dimensions) {
    raw___he_update_full_trace_info("window_server_window_set_size", ___file___, ___line___);
    boolean result = raw_window_server_window_set_size(this, dimensions);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_window_server_window_get_size(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, IVec2 * out) {
    raw___he_update_full_trace_info("window_server_window_get_size", ___file___, ___line___);
    boolean result = raw_window_server_window_get_size(this, out);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_window_server_window_set_position(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, IVec2 dimensions) {
    raw___he_update_full_trace_info("window_server_window_set_position", ___file___, ___line___);
    boolean result = raw_window_server_window_set_position(this, dimensions);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_window_server_window_get_position(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, IVec2 * out) {
    raw___he_update_full_trace_info("window_server_window_get_position", ___file___, ___line___);
    boolean result = raw_window_server_window_get_position(this, out);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline RenderContextSurface * full_trace_render_context_create_surface(const char* ___file___, uint32_t ___line___, WindowServerWindow * window) {
    raw___he_update_full_trace_info("render_context_create_surface", ___file___, ___line___);
    RenderContextSurface * result = raw_render_context_create_surface(window);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_render_context_destroy_surface(const char* ___file___, uint32_t ___line___, RenderContextSurface * surface) {
    raw___he_update_full_trace_info("render_context_destroy_surface", ___file___, ___line___);
    boolean result = raw_render_context_destroy_surface(surface);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_render_context_surface_make_current(const char* ___file___, uint32_t ___line___, RenderContextSurface * surface) {
    raw___he_update_full_trace_info("render_context_surface_make_current", ___file___, ___line___);
    boolean result = raw_render_context_surface_make_current(surface);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline boolean full_trace_render_context_surface_present(const char* ___file___, uint32_t ___line___, RenderContextSurface * surface) {
    raw___he_update_full_trace_info("render_context_surface_present", ___file___, ___line___);
    boolean result = raw_render_context_surface_present(surface);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline fptr full_trace_render_context_get_proc_addr(const char* ___file___, uint32_t ___line___, const char * proc) {
    raw___he_update_full_trace_info("render_context_get_proc_addr", ___file___, ___line___);
    fptr result = raw_render_context_get_proc_addr(proc);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}


#endif

#define set_error(err) full_trace_set_error(__FILE__, __LINE__, err)
#define get_error() full_trace_get_error(__FILE__, __LINE__)
#define datetime_new() full_trace_datetime_new(__FILE__, __LINE__)
#define datetime_free(handle) full_trace_datetime_free(__FILE__, __LINE__, handle)
#define datetime_update(handle) full_trace_datetime_update(__FILE__, __LINE__, handle)
#define datetime_get_year(handle) full_trace_datetime_get_year(__FILE__, __LINE__, handle)
#define datetime_get_month(handle) full_trace_datetime_get_month(__FILE__, __LINE__, handle)
#define datetime_get_day(handle) full_trace_datetime_get_day(__FILE__, __LINE__, handle)
#define datetime_get_hour(handle) full_trace_datetime_get_hour(__FILE__, __LINE__, handle)
#define datetime_get_minute(handle) full_trace_datetime_get_minute(__FILE__, __LINE__, handle)
#define datetime_get_second(handle) full_trace_datetime_get_second(__FILE__, __LINE__, handle)
#define datetime_get_nanosecond(handle) full_trace_datetime_get_nanosecond(__FILE__, __LINE__, handle)
#define datetime_get_utc_year(handle) full_trace_datetime_get_utc_year(__FILE__, __LINE__, handle)
#define datetime_get_utc_month(handle) full_trace_datetime_get_utc_month(__FILE__, __LINE__, handle)
#define datetime_get_utc_day(handle) full_trace_datetime_get_utc_day(__FILE__, __LINE__, handle)
#define datetime_get_utc_hour(handle) full_trace_datetime_get_utc_hour(__FILE__, __LINE__, handle)
#define datetime_get_utc_minute(handle) full_trace_datetime_get_utc_minute(__FILE__, __LINE__, handle)
#define datetime_get_utc_second(handle) full_trace_datetime_get_utc_second(__FILE__, __LINE__, handle)
#define tmalloc(size) full_trace_tmalloc(__FILE__, __LINE__, size)
#define trealloc(ptr, size) full_trace_trealloc(__FILE__, __LINE__, ptr, size)
#define tfree(ptr) full_trace_tfree(__FILE__, __LINE__, ptr)
#define get_allocated_memory() full_trace_get_allocated_memory(__FILE__, __LINE__)
#define mutex_new() full_trace_mutex_new(__FILE__, __LINE__)
#define mutex_new_recursive() full_trace_mutex_new_recursive(__FILE__, __LINE__)
#define mutex_free(handle) full_trace_mutex_free(__FILE__, __LINE__, handle)
#define mutex_lock(handle) full_trace_mutex_lock(__FILE__, __LINE__, handle)
#define mutex_try_lock(handle) full_trace_mutex_try_lock(__FILE__, __LINE__, handle)
#define mutex_unlock(handle) full_trace_mutex_unlock(__FILE__, __LINE__, handle)
#define auto_free(object) full_trace_auto_free(__FILE__, __LINE__, object)
#define node_new(name) full_trace_node_new(__FILE__, __LINE__, name)
#define from_node(node) full_trace_from_node(__FILE__, __LINE__, node)
#define node_set_name(node, name) full_trace_node_set_name(__FILE__, __LINE__, node, name)
#define node_get_name(node) full_trace_node_get_name(__FILE__, __LINE__, node)
#define node_add_child(node, child) full_trace_node_add_child(__FILE__, __LINE__, node, child)
#define node_remove_child(node, child) full_trace_node_remove_child(__FILE__, __LINE__, node, child)
#define node_remove_child_by_name(node, name) full_trace_node_remove_child_by_name(__FILE__, __LINE__, node, name)
#define node_remove_child_by_uid(node, uid) full_trace_node_remove_child_by_uid(__FILE__, __LINE__, node, uid)
#define node_remove_all_children(node) full_trace_node_remove_all_children(__FILE__, __LINE__, node)
#define node_get_child_by_name(node, name) full_trace_node_get_child_by_name(__FILE__, __LINE__, node, name)
#define node_get_child_by_uid(node, uid) full_trace_node_get_child_by_uid(__FILE__, __LINE__, node, uid)
#define node3d_new(name) full_trace_node3d_new(__FILE__, __LINE__, name)
#define window_new(name, title, w, h) full_trace_window_new(__FILE__, __LINE__, name, title, w, h)
#define window_set_title(this, title) full_trace_window_set_title(__FILE__, __LINE__, this, title)
#define window_get_title(this) full_trace_window_get_title(__FILE__, __LINE__, this)
#define canvas_item_new(name) full_trace_canvas_item_new(__FILE__, __LINE__, name)
#define control_new(name) full_trace_control_new(__FILE__, __LINE__, name)
#define viewport_new(name) full_trace_viewport_new(__FILE__, __LINE__, name)
#define mat4_init(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33, out) full_trace_mat4_init(__FILE__, __LINE__, m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33, out)
#define mat4_from_array(array, out) full_trace_mat4_from_array(__FILE__, __LINE__, array, out)
#define mat4_clone(a, out) full_trace_mat4_clone(__FILE__, __LINE__, a, out)
#define mat4_add(a, b, out) full_trace_mat4_add(__FILE__, __LINE__, a, b, out)
#define mat4_sub(a, b, out) full_trace_mat4_sub(__FILE__, __LINE__, a, b, out)
#define mat4_mul(a, b, out) full_trace_mat4_mul(__FILE__, __LINE__, a, b, out)
#define mat4_scale(a, factor, out) full_trace_mat4_scale(__FILE__, __LINE__, a, factor, out)
#define mat4_transpose(a, out) full_trace_mat4_transpose(__FILE__, __LINE__, a, out)
#define mat4_inverse(a, out) full_trace_mat4_inverse(__FILE__, __LINE__, a, out)
#define ivec2_init(x, y, out) full_trace_ivec2_init(__FILE__, __LINE__, x, y, out)
#define ivec2_add(a, b, out) full_trace_ivec2_add(__FILE__, __LINE__, a, b, out)
#define ivec2_sub(a, b, out) full_trace_ivec2_sub(__FILE__, __LINE__, a, b, out)
#define ivec2_scale(a, factor, out) full_trace_ivec2_scale(__FILE__, __LINE__, a, factor, out)
#define ivec2_dot(a, b) full_trace_ivec2_dot(__FILE__, __LINE__, a, b)
#define ivec2_length(a) full_trace_ivec2_length(__FILE__, __LINE__, a)
#define ivec2_normalize(a, out) full_trace_ivec2_normalize(__FILE__, __LINE__, a, out)
#define ivec2_distance(a, b) full_trace_ivec2_distance(__FILE__, __LINE__, a, b)
#define ivec2_add_in(to, what) full_trace_ivec2_add_in(__FILE__, __LINE__, to, what)
#define ivec2_sub_in(from, what) full_trace_ivec2_sub_in(__FILE__, __LINE__, from, what)
#define ivec2_scale_in(to, factor) full_trace_ivec2_scale_in(__FILE__, __LINE__, to, factor)
#define ivec2_normalize_in(a) full_trace_ivec2_normalize_in(__FILE__, __LINE__, a)
#define ivec3_init(x, y, z, out) full_trace_ivec3_init(__FILE__, __LINE__, x, y, z, out)
#define ivec3_add(a, b, out) full_trace_ivec3_add(__FILE__, __LINE__, a, b, out)
#define ivec3_sub(a, b, out) full_trace_ivec3_sub(__FILE__, __LINE__, a, b, out)
#define ivec3_scale(a, factor, out) full_trace_ivec3_scale(__FILE__, __LINE__, a, factor, out)
#define ivec3_cross(a, b, out) full_trace_ivec3_cross(__FILE__, __LINE__, a, b, out)
#define ivec3_dot(a, b) full_trace_ivec3_dot(__FILE__, __LINE__, a, b)
#define ivec3_length(a) full_trace_ivec3_length(__FILE__, __LINE__, a)
#define ivec3_normalize(a, out) full_trace_ivec3_normalize(__FILE__, __LINE__, a, out)
#define ivec3_distance(a, b) full_trace_ivec3_distance(__FILE__, __LINE__, a, b)
#define ivec3_add_in(to, what) full_trace_ivec3_add_in(__FILE__, __LINE__, to, what)
#define ivec3_sub_in(from, what) full_trace_ivec3_sub_in(__FILE__, __LINE__, from, what)
#define ivec3_scale_in(to, factor) full_trace_ivec3_scale_in(__FILE__, __LINE__, to, factor)
#define ivec3_normalize_in(a) full_trace_ivec3_normalize_in(__FILE__, __LINE__, a)
#define ivec4_init(x, y, z, w, out) full_trace_ivec4_init(__FILE__, __LINE__, x, y, z, w, out)
#define ivec4_add(a, b, out) full_trace_ivec4_add(__FILE__, __LINE__, a, b, out)
#define ivec4_sub(a, b, out) full_trace_ivec4_sub(__FILE__, __LINE__, a, b, out)
#define ivec4_scale(a, factor, out) full_trace_ivec4_scale(__FILE__, __LINE__, a, factor, out)
#define ivec4_dot(a, b) full_trace_ivec4_dot(__FILE__, __LINE__, a, b)
#define ivec4_length(a) full_trace_ivec4_length(__FILE__, __LINE__, a)
#define ivec4_normalize(a, out) full_trace_ivec4_normalize(__FILE__, __LINE__, a, out)
#define ivec4_distance(a, b) full_trace_ivec4_distance(__FILE__, __LINE__, a, b)
#define ivec4_add_in(to, what) full_trace_ivec4_add_in(__FILE__, __LINE__, to, what)
#define ivec4_sub_in(from, what) full_trace_ivec4_sub_in(__FILE__, __LINE__, from, what)
#define ivec4_scale_in(to, factor) full_trace_ivec4_scale_in(__FILE__, __LINE__, to, factor)
#define ivec4_normalize_in(a) full_trace_ivec4_normalize_in(__FILE__, __LINE__, a)
#define uvec2_init(x, y, out) full_trace_uvec2_init(__FILE__, __LINE__, x, y, out)
#define uvec2_add(a, b, out) full_trace_uvec2_add(__FILE__, __LINE__, a, b, out)
#define uvec2_sub(a, b, out) full_trace_uvec2_sub(__FILE__, __LINE__, a, b, out)
#define uvec2_scale(a, factor, out) full_trace_uvec2_scale(__FILE__, __LINE__, a, factor, out)
#define uvec2_dot(a, b) full_trace_uvec2_dot(__FILE__, __LINE__, a, b)
#define uvec2_length(a) full_trace_uvec2_length(__FILE__, __LINE__, a)
#define uvec2_normalize(a, out) full_trace_uvec2_normalize(__FILE__, __LINE__, a, out)
#define uvec2_distance(a, b) full_trace_uvec2_distance(__FILE__, __LINE__, a, b)
#define uvec2_add_in(to, what) full_trace_uvec2_add_in(__FILE__, __LINE__, to, what)
#define uvec2_sub_in(from, what) full_trace_uvec2_sub_in(__FILE__, __LINE__, from, what)
#define uvec2_scale_in(to, factor) full_trace_uvec2_scale_in(__FILE__, __LINE__, to, factor)
#define uvec2_normalize_in(a) full_trace_uvec2_normalize_in(__FILE__, __LINE__, a)
#define uvec3_init(x, y, z, out) full_trace_uvec3_init(__FILE__, __LINE__, x, y, z, out)
#define uvec3_add(a, b, out) full_trace_uvec3_add(__FILE__, __LINE__, a, b, out)
#define uvec3_sub(a, b, out) full_trace_uvec3_sub(__FILE__, __LINE__, a, b, out)
#define uvec3_scale(a, factor, out) full_trace_uvec3_scale(__FILE__, __LINE__, a, factor, out)
#define uvec3_cross(a, b, out) full_trace_uvec3_cross(__FILE__, __LINE__, a, b, out)
#define uvec3_dot(a, b) full_trace_uvec3_dot(__FILE__, __LINE__, a, b)
#define uvec3_length(a) full_trace_uvec3_length(__FILE__, __LINE__, a)
#define uvec3_normalize(a, out) full_trace_uvec3_normalize(__FILE__, __LINE__, a, out)
#define uvec3_distance(a, b) full_trace_uvec3_distance(__FILE__, __LINE__, a, b)
#define uvec3_add_in(to, what) full_trace_uvec3_add_in(__FILE__, __LINE__, to, what)
#define uvec3_sub_in(from, what) full_trace_uvec3_sub_in(__FILE__, __LINE__, from, what)
#define uvec3_scale_in(to, factor) full_trace_uvec3_scale_in(__FILE__, __LINE__, to, factor)
#define uvec3_normalize_in(a) full_trace_uvec3_normalize_in(__FILE__, __LINE__, a)
#define uvec4_init(x, y, z, w, out) full_trace_uvec4_init(__FILE__, __LINE__, x, y, z, w, out)
#define uvec4_add(a, b, out) full_trace_uvec4_add(__FILE__, __LINE__, a, b, out)
#define uvec4_sub(a, b, out) full_trace_uvec4_sub(__FILE__, __LINE__, a, b, out)
#define uvec4_scale(a, factor, out) full_trace_uvec4_scale(__FILE__, __LINE__, a, factor, out)
#define uvec4_dot(a, b) full_trace_uvec4_dot(__FILE__, __LINE__, a, b)
#define uvec4_length(a) full_trace_uvec4_length(__FILE__, __LINE__, a)
#define uvec4_normalize(a, out) full_trace_uvec4_normalize(__FILE__, __LINE__, a, out)
#define uvec4_distance(a, b) full_trace_uvec4_distance(__FILE__, __LINE__, a, b)
#define uvec4_add_in(to, what) full_trace_uvec4_add_in(__FILE__, __LINE__, to, what)
#define uvec4_sub_in(from, what) full_trace_uvec4_sub_in(__FILE__, __LINE__, from, what)
#define uvec4_scale_in(to, factor) full_trace_uvec4_scale_in(__FILE__, __LINE__, to, factor)
#define uvec4_normalize_in(a) full_trace_uvec4_normalize_in(__FILE__, __LINE__, a)
#define vec2_init(x, y, out) full_trace_vec2_init(__FILE__, __LINE__, x, y, out)
#define vec2_add(a, b, out) full_trace_vec2_add(__FILE__, __LINE__, a, b, out)
#define vec2_sub(a, b, out) full_trace_vec2_sub(__FILE__, __LINE__, a, b, out)
#define vec2_scale(a, factor, out) full_trace_vec2_scale(__FILE__, __LINE__, a, factor, out)
#define vec2_dot(a, b) full_trace_vec2_dot(__FILE__, __LINE__, a, b)
#define vec2_length(a) full_trace_vec2_length(__FILE__, __LINE__, a)
#define vec2_normalize(a, out) full_trace_vec2_normalize(__FILE__, __LINE__, a, out)
#define vec2_distance(a, b) full_trace_vec2_distance(__FILE__, __LINE__, a, b)
#define vec2_add_in(to, what) full_trace_vec2_add_in(__FILE__, __LINE__, to, what)
#define vec2_sub_in(from, what) full_trace_vec2_sub_in(__FILE__, __LINE__, from, what)
#define vec2_scale_in(to, factor) full_trace_vec2_scale_in(__FILE__, __LINE__, to, factor)
#define vec2_normalize_in(a) full_trace_vec2_normalize_in(__FILE__, __LINE__, a)
#define vec3_init(x, y, z, out) full_trace_vec3_init(__FILE__, __LINE__, x, y, z, out)
#define vec3_add(a, b, out) full_trace_vec3_add(__FILE__, __LINE__, a, b, out)
#define vec3_sub(a, b, out) full_trace_vec3_sub(__FILE__, __LINE__, a, b, out)
#define vec3_scale(a, factor, out) full_trace_vec3_scale(__FILE__, __LINE__, a, factor, out)
#define vec3_cross(a, b, out) full_trace_vec3_cross(__FILE__, __LINE__, a, b, out)
#define vec3_dot(a, b) full_trace_vec3_dot(__FILE__, __LINE__, a, b)
#define vec3_length(a) full_trace_vec3_length(__FILE__, __LINE__, a)
#define vec3_normalize(a, out) full_trace_vec3_normalize(__FILE__, __LINE__, a, out)
#define vec3_distance(a, b) full_trace_vec3_distance(__FILE__, __LINE__, a, b)
#define vec3_add_in(to, what) full_trace_vec3_add_in(__FILE__, __LINE__, to, what)
#define vec3_sub_in(from, what) full_trace_vec3_sub_in(__FILE__, __LINE__, from, what)
#define vec3_scale_in(to, factor) full_trace_vec3_scale_in(__FILE__, __LINE__, to, factor)
#define vec3_normalize_in(a) full_trace_vec3_normalize_in(__FILE__, __LINE__, a)
#define vec4_init(x, y, z, w, out) full_trace_vec4_init(__FILE__, __LINE__, x, y, z, w, out)
#define vec4_add(a, b, out) full_trace_vec4_add(__FILE__, __LINE__, a, b, out)
#define vec4_sub(a, b, out) full_trace_vec4_sub(__FILE__, __LINE__, a, b, out)
#define vec4_scale(a, factor, out) full_trace_vec4_scale(__FILE__, __LINE__, a, factor, out)
#define vec4_dot(a, b) full_trace_vec4_dot(__FILE__, __LINE__, a, b)
#define vec4_length(a) full_trace_vec4_length(__FILE__, __LINE__, a)
#define vec4_normalize(a, out) full_trace_vec4_normalize(__FILE__, __LINE__, a, out)
#define vec4_distance(a, b) full_trace_vec4_distance(__FILE__, __LINE__, a, b)
#define vec4_add_in(to, what) full_trace_vec4_add_in(__FILE__, __LINE__, to, what)
#define vec4_sub_in(from, what) full_trace_vec4_sub_in(__FILE__, __LINE__, from, what)
#define vec4_scale_in(to, factor) full_trace_vec4_scale_in(__FILE__, __LINE__, to, factor)
#define vec4_normalize_in(a) full_trace_vec4_normalize_in(__FILE__, __LINE__, a)
#define string_new(str) full_trace_string_new(__FILE__, __LINE__, str)
#define string_from(c_str) full_trace_string_from(__FILE__, __LINE__, c_str)
#define string_clone(c_str) full_trace_string_clone(__FILE__, __LINE__, c_str)
#define string_set_cstr(self, c_cstr) full_trace_string_set_cstr(__FILE__, __LINE__, self, c_cstr)
#define string_set(self, c_str) full_trace_string_set(__FILE__, __LINE__, self, c_str)
#define string_cstr(c_str) full_trace_string_cstr(__FILE__, __LINE__, c_str)
#define string_len(c_str) full_trace_string_len(__FILE__, __LINE__, c_str)
#define string_size(c_str) full_trace_string_size(__FILE__, __LINE__, c_str)
#define string_push_back(dest, src) full_trace_string_push_back(__FILE__, __LINE__, dest, src)
#define string_push_back_cstr(dest, src) full_trace_string_push_back_cstr(__FILE__, __LINE__, dest, src)
#define string_push_front(dest, src) full_trace_string_push_front(__FILE__, __LINE__, dest, src)
#define string_push_front_cstr(dest, src) full_trace_string_push_front_cstr(__FILE__, __LINE__, dest, src)
#define string_insert(dest, src, i) full_trace_string_insert(__FILE__, __LINE__, dest, src, i)
#define string_insert_cstr(dest, src, i) full_trace_string_insert_cstr(__FILE__, __LINE__, dest, src, i)
#define string_insert_cstr_ex(dest, src, i) full_trace_string_insert_cstr_ex(__FILE__, __LINE__, dest, src, i)
#define string_insert_cstr_by_byte(dest, src, b) full_trace_string_insert_cstr_by_byte(__FILE__, __LINE__, dest, src, b)
#define string_insert_by_byte(dest, src, b) full_trace_string_insert_by_byte(__FILE__, __LINE__, dest, src, b)
#define string_remove(self, i) full_trace_string_remove(__FILE__, __LINE__, self, i)
#define string_remove_by_byte(self, b) full_trace_string_remove_by_byte(__FILE__, __LINE__, self, b)
#define string_remove_n(self, i, n) full_trace_string_remove_n(__FILE__, __LINE__, self, i, n)
#define string_remove_n_by_byte(self, b, b_n) full_trace_string_remove_n_by_byte(__FILE__, __LINE__, self, b, b_n)
#define string_equals(str1, str2) full_trace_string_equals(__FILE__, __LINE__, str1, str2)
#define string_equals_cstr(str, cstr) full_trace_string_equals_cstr(__FILE__, __LINE__, str, cstr)
#define string_free(str) full_trace_string_free(__FILE__, __LINE__, str)
#define string_itr_free(str_itr) full_trace_string_itr_free(__FILE__, __LINE__, str_itr)
#define string_get_itr(c_str) full_trace_string_get_itr(__FILE__, __LINE__, c_str)
#define string_itr_next(str_itr) full_trace_string_itr_next(__FILE__, __LINE__, str_itr)
#define string_get_slice(str, s, e) full_trace_string_get_slice(__FILE__, __LINE__, str, s, e)
#define string_from_slice(str_sl) full_trace_string_from_slice(__FILE__, __LINE__, str_sl)
#define string_equals_slice(str_sl_1, str_sl_2) full_trace_string_equals_slice(__FILE__, __LINE__, str_sl_1, str_sl_2)
#define string_set_slice(self, str_sl) full_trace_string_set_slice(__FILE__, __LINE__, self, str_sl)
#define string_push_back_slice(dest, src) full_trace_string_push_back_slice(__FILE__, __LINE__, dest, src)
#define string_push_front_slice(dest, src) full_trace_string_push_front_slice(__FILE__, __LINE__, dest, src)
#define string_insert_slice_ex(dest, src, i) full_trace_string_insert_slice_ex(__FILE__, __LINE__, dest, src, i)
#define string_insert_slice(dest, src, i) full_trace_string_insert_slice(__FILE__, __LINE__, dest, src, i)
#define string_insert_slice_by_byte(dest, src, b) full_trace_string_insert_slice_by_byte(__FILE__, __LINE__, dest, src, b)
#define string_slice_free(str_sl) full_trace_string_slice_free(__FILE__, __LINE__, str_sl)
#define string_utf8_new(str) full_trace_string_utf8_new(__FILE__, __LINE__, str)
#define string_utf8_to_string(dest, str) full_trace_string_utf8_to_string(__FILE__, __LINE__, dest, str)
#define string_utf8_from(str, c_str) full_trace_string_utf8_from(__FILE__, __LINE__, str, c_str)
#define string_utf8_len(len, c_str) full_trace_string_utf8_len(__FILE__, __LINE__, len, c_str)
#define string_utf8_size(size, c_str) full_trace_string_utf8_size(__FILE__, __LINE__, size, c_str)
#define string_utf8_clone(str, c_str) full_trace_string_utf8_clone(__FILE__, __LINE__, str, c_str)
#define string_utf8_push_back_cstr(dest, src) full_trace_string_utf8_push_back_cstr(__FILE__, __LINE__, dest, src)
#define string_utf8_push_back(dest, src) full_trace_string_utf8_push_back(__FILE__, __LINE__, dest, src)
#define string_utf8_insert(dest, src, i) full_trace_string_utf8_insert(__FILE__, __LINE__, dest, src, i)
#define string_utf8_push_front(dest, src) full_trace_string_utf8_push_front(__FILE__, __LINE__, dest, src)
#define string_utf8_free(str) full_trace_string_utf8_free(__FILE__, __LINE__, str)
#define uid_new() full_trace_uid_new(__FILE__, __LINE__)
#define vfs_mount_res(path, mount_point) full_trace_vfs_mount_res(__FILE__, __LINE__, path, mount_point)
#define vfs_unmount_res(mount_point) full_trace_vfs_unmount_res(__FILE__, __LINE__, mount_point)
#define vfs_mount_rfs(mount_point) full_trace_vfs_mount_rfs(__FILE__, __LINE__, mount_point)
#define vfs_mount_rfs_whitelist(whitelist, count, mount_point) full_trace_vfs_mount_rfs_whitelist(__FILE__, __LINE__, whitelist, count, mount_point)
#define vfs_unmount_rfs() full_trace_vfs_unmount_rfs(__FILE__, __LINE__)
#define vfs_res_path_exists(path) full_trace_vfs_res_path_exists(__FILE__, __LINE__, path)
#define vfs_usr_path_exists(path, prefer_res) full_trace_vfs_usr_path_exists(__FILE__, __LINE__, path, prefer_res)
#define vfs_res_read_file(path, size) full_trace_vfs_res_read_file(__FILE__, __LINE__, path, size)
#define vfs_res_stream_open(path) full_trace_vfs_res_stream_open(__FILE__, __LINE__, path)
#define vfs_usr_read_file(path, size, prefer_res) full_trace_vfs_usr_read_file(__FILE__, __LINE__, path, size, prefer_res)
#define vfs_usr_write_file(path, data, size) full_trace_vfs_usr_write_file(__FILE__, __LINE__, path, data, size)
#define vfs_usr_stream_open(path, prefer_res) full_trace_vfs_usr_stream_open(__FILE__, __LINE__, path, prefer_res)
#define vfs_stream_size(stream) full_trace_vfs_stream_size(__FILE__, __LINE__, stream)
#define vfs_stream_read_n(stream, buffer, size) full_trace_vfs_stream_read_n(__FILE__, __LINE__, stream, buffer, size)
#define vfs_stream_read_all(stream, size) full_trace_vfs_stream_read_all(__FILE__, __LINE__, stream, size)
#define vfs_stream_write(stream, buffer, size) full_trace_vfs_stream_write(__FILE__, __LINE__, stream, buffer, size)
#define vfs_stream_seek(stream, whence, offset) full_trace_vfs_stream_seek(__FILE__, __LINE__, stream, whence, offset)
#define vfs_stream_tell(stream, success) full_trace_vfs_stream_tell(__FILE__, __LINE__, stream, success)
#define vfs_stream_flush(stream) full_trace_vfs_stream_flush(__FILE__, __LINE__, stream)
#define vfs_stream_close(stream) full_trace_vfs_stream_close(__FILE__, __LINE__, stream)
#define window_server_register_backend(name, backend) full_trace_window_server_register_backend(__FILE__, __LINE__, name, backend)
#define window_server_load_backend(name) full_trace_window_server_load_backend(__FILE__, __LINE__, name)
#define window_server_backend_new() full_trace_window_server_backend_new(__FILE__, __LINE__)
#define window_server_backend_free(backend) full_trace_window_server_backend_free(__FILE__, __LINE__, backend)
#define window_server_backend_set_function(backend, name, function) full_trace_window_server_backend_set_function(__FILE__, __LINE__, backend, name, function)
#define window_server_backend_get_function(backend, name) full_trace_window_server_backend_get_function(__FILE__, __LINE__, backend, name)
#define render_context_register_backend(render_server_name, window_server_name, backend) full_trace_render_context_register_backend(__FILE__, __LINE__, render_server_name, window_server_name, backend)
#define render_context_load_backend(render_server_name, window_server_name) full_trace_render_context_load_backend(__FILE__, __LINE__, render_server_name, window_server_name)
#define render_context_backend_new() full_trace_render_context_backend_new(__FILE__, __LINE__)
#define render_context_backend_free(backend) full_trace_render_context_backend_free(__FILE__, __LINE__, backend)
#define render_context_backend_set_function(backend, name, function) full_trace_render_context_backend_set_function(__FILE__, __LINE__, backend, name, function)
#define render_context_backend_get_function(backend, name) full_trace_render_context_backend_get_function(__FILE__, __LINE__, backend, name)
#define render_server_register_backend(name, backend) full_trace_render_server_register_backend(__FILE__, __LINE__, name, backend)
#define render_server_load_backend(name) full_trace_render_server_load_backend(__FILE__, __LINE__, name)
#define render_server_backend_new() full_trace_render_server_backend_new(__FILE__, __LINE__)
#define render_server_backend_free(backend) full_trace_render_server_backend_free(__FILE__, __LINE__, backend)
#define render_server_backend_set_function(backend, name, function) full_trace_render_server_backend_set_function(__FILE__, __LINE__, backend, name, function)
#define render_server_backend_get_function(backend, name) full_trace_render_server_backend_get_function(__FILE__, __LINE__, backend, name)
#define window_server_create_window(title, size, parent) full_trace_window_server_create_window(__FILE__, __LINE__, title, size, parent)
#define window_server_destroy_window(this) full_trace_window_server_destroy_window(__FILE__, __LINE__, this)
#define window_server_window_set_title(this, title) full_trace_window_server_window_set_title(__FILE__, __LINE__, this, title)
#define window_server_window_get_title(this) full_trace_window_server_window_get_title(__FILE__, __LINE__, this)
#define window_server_window_set_mode(this, mode) full_trace_window_server_window_set_mode(__FILE__, __LINE__, this, mode)
#define window_server_window_get_mode(this) full_trace_window_server_window_get_mode(__FILE__, __LINE__, this)
#define window_server_window_set_size(this, dimensions) full_trace_window_server_window_set_size(__FILE__, __LINE__, this, dimensions)
#define window_server_window_get_size(this, out) full_trace_window_server_window_get_size(__FILE__, __LINE__, this, out)
#define window_server_window_set_position(this, dimensions) full_trace_window_server_window_set_position(__FILE__, __LINE__, this, dimensions)
#define window_server_window_get_position(this, out) full_trace_window_server_window_get_position(__FILE__, __LINE__, this, out)
#define render_context_create_surface(window) full_trace_render_context_create_surface(__FILE__, __LINE__, window)
#define render_context_destroy_surface(surface) full_trace_render_context_destroy_surface(__FILE__, __LINE__, surface)
#define render_context_surface_make_current(surface) full_trace_render_context_surface_make_current(__FILE__, __LINE__, surface)
#define render_context_surface_present(surface) full_trace_render_context_surface_present(__FILE__, __LINE__, surface)
#define render_context_get_proc_addr(proc) full_trace_render_context_get_proc_addr(__FILE__, __LINE__, proc)

#endif

// clang-format on

