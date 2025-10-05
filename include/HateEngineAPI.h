
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

typedef float f32;
typedef double f64;

typedef uintptr_t uptr;
typedef intptr_t iptr;




typedef const char* Error;

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


#define ERROR_ARG_CHECK(arg)                                                                            \
    if (!(arg)) {                                                                                       \
        LOG_ERROR("Invalid argument: " #arg);                                                           \
        return ERROR_INVALID_ARGUMENT;                                                                  \
    }

#define ERROR_ARGS_CHECK_1(a) ERROR_ARG_CHECK(a)
#define ERROR_ARGS_CHECK_2(a, b) ERROR_ARG_CHECK(a) ERROR_ARG_CHECK(b)
#define ERROR_ARGS_CHECK_3(a, b, c) ERROR_ARG_CHECK(a) ERROR_ARG_CHECK(b) ERROR_ARG_CHECK(c)
#define ERROR_ARGS_CHECK_4(a, b, c, d)                                                                  \
    ERROR_ARG_CHECK(a) ERROR_ARG_CHECK(b) ERROR_ARG_CHECK(c) ERROR_ARG_CHECK(d)


#define ERROR_ASSERT(error, ...) ERROR_ASSERT_FATAL(error, __VA_ARGS__)

#define ERROR_SUCCESS ((Error) NULL)

#define ERROR_NOT_IMPLEMENTED "NotImplemented"
#define ERROR_INVALID_ARGUMENT "InvalidArgument"
#define ERROR_ALREADY_EXISTS "AlreadyExists"
#define ERROR_NOT_FOUND "NotFound"
#define ERROR_INVALID_STATE "InvalidState"

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
 *     "init_method": "___hate_engine_runtime_init_window_server",
 *     "global_server_backend_var": "WindowServer"
 * }
 */
typedef struct WindowServerBackend WindowServerBackend;

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
 * @brief Primitive 2D vector
 *
 * @api forward
 */
typedef struct Vec2 {
    float x;
    float y;
} Vec2;

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
typedef uint64_t UID;

/**
 * 'd' - disabled
 *
 * 'e' - enabled 
 *
 * 'a' - enabled async 
 *
 * @api
 */
typedef char WindowServerWindowVSync;

/**
 * 'w' - windowed
 *
 * 'f' - exclusive fullscreen 
 *
 * 'b' - borderless fullscreen 
 *
 * @api
 */
typedef char WindowServerWindowMode;



#if defined(HEAPI_COMPILE_TIME)

#error "HEAPI_COMPILE_TIME not supported yet"


#else

/**
 * @brief Inner function to update the full trace info. Used for tracing with HEAPI_FULL_TRACE. Don't use directly
 * 
 * @param func function_name, "" for reset
 * @param file "" for reset
 * @param line -1 for reset

 * @api
 */
extern void (*raw___he_update_full_trace_info)(const char * func, const char * file, i32 line);

/**
 * @brief Malloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param size
 * @return void*
 *
 * @api
 */
extern void * (*raw_tmalloc)(int size);

/**
 * @brief Realloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 * @param size
 * @return void*
 *
 * @api
 */
extern void * (*raw_trealloc)(void * ptr, int size);

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
extern int (*raw_get_allocated_memory)(void);

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
extern int (*raw_node_remove_child)(Node * node, Node * child);

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
extern int (*raw_node_remove_child_by_name)(Node * node, const char * name);

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
extern int (*raw_node_remove_child_by_uid)(Node * node, UID uid);

/**
 * @api
 */
extern int (*raw_node_remove_all_children)(Node * node);

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
extern Window * (*raw_window_new)(const char * name, const char * title, int w, int h);

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
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*raw_mat4_new)(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);

/**
 * @brief 
 * 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*raw_mat4_new_zero)(void);

/**
 * @brief 
 * 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*raw_mat4_new_one)(void);

/**
 * @brief 
 * 
 * @param array 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*raw_mat4_from_array)(const float *const array);

/**
 * @brief 
 * 
 * @param a 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*raw_mat4_from_mat4)(const Mat4 *const a);

/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*raw_mat4_add)(const Mat4 *const a, const Mat4 *const b);

/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*raw_mat4_sub)(const Mat4 *const a, const Mat4 *const b);

/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*raw_mat4_mul)(const Mat4 *const a, const Mat4 *const b);

/**
 * @brief 
 * 
 * @param a 
 * @param factor 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*raw_mat4_scale)(const Mat4 *const a, const float factor);

/**
 * @brief 
 * 
 * @param a 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*raw_mat4_transpose)(const Mat4 *const a);

/**
 * @brief 
 * 
 * @param a 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*raw_mat4_inverse)(const Mat4 *const a);

/**
 * @brief 
 * 
 * @api
 */
extern Vec3 (*raw_vec3_new)(float x, float y, float z);

/**
 * @brief 
 * 
 * @api
 */
extern Vec3 (*raw_vec3_add)(const Vec3 *const a, const Vec3 *const b);

/**
 * @brief 
 * 
 * @api
 */
extern Vec3 (*raw_vec3_sub)(const Vec3 *const a, const Vec3 *const b);

/**
 * @brief 
 * 
 * @api
 */
extern Vec3 (*raw_vec3_scale)(const Vec3 *const a, const float factor);

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
extern Vec3 (*raw_vec3_cross)(const Vec3 *const a, const Vec3 *const b);

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
extern Vec3 (*raw_vec3_normalize)(const Vec3 *const a);

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
 * @api
 */
extern Vec2 (*raw_vec2_new)(float x, float y, float z);

/**
 * @api
 */
extern Vec2 (*raw_vec2_add)(const Vec2 *const a, const Vec2 *const b);

/**
 * @api
 */
extern Vec2 (*raw_vec2_sub)(const Vec2 *const a, const Vec2 *const b);

/**
 * @api
 */
extern Vec2 (*raw_vec2_scale)(const Vec2 *const a, const float factor);

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
extern Vec2 (*raw_vec2_normalize)(const Vec2 *const a);

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
extern int (*raw_vfs_mount_res)(const char * path, const char * mount_point);

/**
 * @brief Unmount Resource File from Virtual File System
 * 
 * @param mount_point Mount point
 * @return true 
 * @return false 
 *
 * @api
 */
extern int (*raw_vfs_unmount_res)(const char * mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res
 *
 * @return true if mounted successfully
 * @return false if mount failed
 *
 * @api
 */
extern int (*raw_vfs_mount_rfs)(const char * mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res only with allowed files in whitelist
 *
 * @return true if mounted successfully
 * @return false if mount failed
 *
 * @api
 */
extern int (*raw_vfs_mount_rfs_whitelist)(const char ** whitelist, int count, const char * mount_point);

/**
 * @brief Unmount Real File System from Virtual File System Res
 *
 * @return true if unmounted successfully
 * @return false if unmount failed
 *
 * @api
 */
extern int (*raw_vfs_unmount_rfs)(void);

/**
 * @brief Check if file exists
 *
 * @param path
 * @return true if exists
 * @return false if not exists
 *
 * @api
 */
extern int (*raw_vfs_res_path_exists)(const char * path);

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
extern int (*raw_vfs_usr_path_exists)(const char * path, int prefer_res);

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
extern void * (*raw_vfs_res_read_file)(const char * path, int * size);

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
extern void * (*raw_vfs_usr_read_file)(const char * path, int * size, int prefer_res);

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
extern int (*raw_vfs_usr_write_file)(const char * path, const void * data, int size);

/**
 * @brief Open file stream from User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return FileStream* If success, else NULL
 *
 * @api
 */
extern FileStream * (*raw_vfs_usr_stream_open)(const char * path, int prefer_res);

/**
 * @brief Get file stream size in bytes
 *
 * @param path
 * @return size_t
 *
 * @api
 */
extern int (*raw_vfs_stream_size)(FileStream * stream);

/**
 * @brief Read from file stream n bytes to buffer
 *
 * @param stream
 * @param buffer
 * @param size
 * @return size_t
 *
 * @api
 */
extern int (*raw_vfs_stream_read_n)(FileStream * stream, void * buffer, int size);

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
extern void * (*raw_vfs_stream_read_all)(FileStream * stream, int * size);

/**
 * @brief Write to file stream n bytes from buffer
 * @warning Only for 'res' scope
 *
 * @param stream
 * @param buffer
 * @param size
 *
 * @return size_t Number of bytes written. 0 if fail
 *
 * @api
 */
extern int (*raw_vfs_stream_write)(FileStream * stream, void * buffer, int size);

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
extern int (*raw_vfs_stream_seek)(FileStream * stream, FSSeekFrom whence, int offset);

/**
 * @brief Get file stream current position
 *
 * @param stream
 * @param success Can be NULL
 * @return size_t
 *
 * @api
 */
extern int (*raw_vfs_stream_tell)(FileStream * stream, int * success);

/**
 * @brief Flush file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 * @api
 */
extern int (*raw_vfs_stream_flush)(FileStream * stream);

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
extern Error (*raw_window_server_register_backend)(const char * name, WindowServerBackend * backend);

/**
 * @brief Load a backend. First you should register them via window_server_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if name is NULL
 * @return "NotFound" if a backend with the given name is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
extern Error (*raw_window_server_load_backend)(const char * name);

/**
 * @brief Create a new WindowServerBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
extern WindowServerBackend * (*raw_window_server_backend_new)(void);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
extern Error (*raw_window_server_backend_set_function)(WindowServerBackend * backend, const char * name, void (* function) (void));

/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 * 
 * @api
 */
extern Error (*raw_window_server_backend_get_function)(WindowServerBackend * backend, const char * name, void (** function) (void));

extern Error (*raw_window_server_create_window)(const char * title, int w, int h, WindowServerWindow * parent, WindowServerWindow ** out);

extern Error (*raw_window_server_destroy_window)(WindowServerWindow * this);

extern Error (*raw_window_server_window_set_title)(WindowServerWindow * this, const char * title);

extern Error (*raw_window_server_window_get_title)(WindowServerWindow * this, const char ** out);

/**
     * @brief HOLA BOLA
     * 
     */
extern Error (*raw_window_server_window_set_mode)(WindowServerWindow * this, WindowServerWindowMode mode);

extern Error (*raw_window_server_window_get_mode)(WindowServerWindow * this, WindowServerWindowMode * out);

extern Error (*raw_window_server_window_set_size)(WindowServerWindow * this, int w, int h);

extern Error (*raw_window_server_window_get_size)(WindowServerWindow * this, int * w, int * h);

extern Error (*raw_window_server_window_set_position)(WindowServerWindow * this, int x, int y);

extern Error (*raw_window_server_window_get_position)(WindowServerWindow * this, int * x, int * y);

extern Error (*raw_window_server_window_set_fullscreen_display)(WindowServerWindow * this, WindowServerDisplay * fullscreen);



#if !defined(HEAPI_FULL_TRACE)
    /**
 * @brief Inner function to update the full trace info. Used for tracing with HEAPI_FULL_TRACE. Don't use directly
 * 
 * @param func function_name, "" for reset
 * @param file "" for reset
 * @param line -1 for reset

 * @api
 */
extern void (*__he_update_full_trace_info)(const char * func, const char * file, i32 line);

/**
 * @brief Malloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param size
 * @return void*
 *
 * @api
 */
extern void * (*tmalloc)(int size);

/**
 * @brief Realloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 * @param size
 * @return void*
 *
 * @api
 */
extern void * (*trealloc)(void * ptr, int size);

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
extern int (*get_allocated_memory)(void);

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
extern int (*node_remove_child)(Node * node, Node * child);

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
extern int (*node_remove_child_by_name)(Node * node, const char * name);

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
extern int (*node_remove_child_by_uid)(Node * node, UID uid);

/**
 * @api
 */
extern int (*node_remove_all_children)(Node * node);

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
extern Window * (*window_new)(const char * name, const char * title, int w, int h);

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
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*mat4_new)(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);

/**
 * @brief 
 * 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*mat4_new_zero)(void);

/**
 * @brief 
 * 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*mat4_new_one)(void);

/**
 * @brief 
 * 
 * @param array 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*mat4_from_array)(const float *const array);

/**
 * @brief 
 * 
 * @param a 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*mat4_from_mat4)(const Mat4 *const a);

/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*mat4_add)(const Mat4 *const a, const Mat4 *const b);

/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*mat4_sub)(const Mat4 *const a, const Mat4 *const b);

/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*mat4_mul)(const Mat4 *const a, const Mat4 *const b);

/**
 * @brief 
 * 
 * @param a 
 * @param factor 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*mat4_scale)(const Mat4 *const a, const float factor);

/**
 * @brief 
 * 
 * @param a 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*mat4_transpose)(const Mat4 *const a);

/**
 * @brief 
 * 
 * @param a 
 * @return Mat4 
 *
 * @api
 */
extern Mat4 (*mat4_inverse)(const Mat4 *const a);

/**
 * @brief 
 * 
 * @api
 */
extern Vec3 (*vec3_new)(float x, float y, float z);

/**
 * @brief 
 * 
 * @api
 */
extern Vec3 (*vec3_add)(const Vec3 *const a, const Vec3 *const b);

/**
 * @brief 
 * 
 * @api
 */
extern Vec3 (*vec3_sub)(const Vec3 *const a, const Vec3 *const b);

/**
 * @brief 
 * 
 * @api
 */
extern Vec3 (*vec3_scale)(const Vec3 *const a, const float factor);

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
extern Vec3 (*vec3_cross)(const Vec3 *const a, const Vec3 *const b);

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
extern Vec3 (*vec3_normalize)(const Vec3 *const a);

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
 * @api
 */
extern Vec2 (*vec2_new)(float x, float y, float z);

/**
 * @api
 */
extern Vec2 (*vec2_add)(const Vec2 *const a, const Vec2 *const b);

/**
 * @api
 */
extern Vec2 (*vec2_sub)(const Vec2 *const a, const Vec2 *const b);

/**
 * @api
 */
extern Vec2 (*vec2_scale)(const Vec2 *const a, const float factor);

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
extern Vec2 (*vec2_normalize)(const Vec2 *const a);

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
extern int (*vfs_mount_res)(const char * path, const char * mount_point);

/**
 * @brief Unmount Resource File from Virtual File System
 * 
 * @param mount_point Mount point
 * @return true 
 * @return false 
 *
 * @api
 */
extern int (*vfs_unmount_res)(const char * mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res
 *
 * @return true if mounted successfully
 * @return false if mount failed
 *
 * @api
 */
extern int (*vfs_mount_rfs)(const char * mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res only with allowed files in whitelist
 *
 * @return true if mounted successfully
 * @return false if mount failed
 *
 * @api
 */
extern int (*vfs_mount_rfs_whitelist)(const char ** whitelist, int count, const char * mount_point);

/**
 * @brief Unmount Real File System from Virtual File System Res
 *
 * @return true if unmounted successfully
 * @return false if unmount failed
 *
 * @api
 */
extern int (*vfs_unmount_rfs)(void);

/**
 * @brief Check if file exists
 *
 * @param path
 * @return true if exists
 * @return false if not exists
 *
 * @api
 */
extern int (*vfs_res_path_exists)(const char * path);

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
extern int (*vfs_usr_path_exists)(const char * path, int prefer_res);

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
extern void * (*vfs_res_read_file)(const char * path, int * size);

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
extern void * (*vfs_usr_read_file)(const char * path, int * size, int prefer_res);

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
extern int (*vfs_usr_write_file)(const char * path, const void * data, int size);

/**
 * @brief Open file stream from User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return FileStream* If success, else NULL
 *
 * @api
 */
extern FileStream * (*vfs_usr_stream_open)(const char * path, int prefer_res);

/**
 * @brief Get file stream size in bytes
 *
 * @param path
 * @return size_t
 *
 * @api
 */
extern int (*vfs_stream_size)(FileStream * stream);

/**
 * @brief Read from file stream n bytes to buffer
 *
 * @param stream
 * @param buffer
 * @param size
 * @return size_t
 *
 * @api
 */
extern int (*vfs_stream_read_n)(FileStream * stream, void * buffer, int size);

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
extern void * (*vfs_stream_read_all)(FileStream * stream, int * size);

/**
 * @brief Write to file stream n bytes from buffer
 * @warning Only for 'res' scope
 *
 * @param stream
 * @param buffer
 * @param size
 *
 * @return size_t Number of bytes written. 0 if fail
 *
 * @api
 */
extern int (*vfs_stream_write)(FileStream * stream, void * buffer, int size);

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
extern int (*vfs_stream_seek)(FileStream * stream, FSSeekFrom whence, int offset);

/**
 * @brief Get file stream current position
 *
 * @param stream
 * @param success Can be NULL
 * @return size_t
 *
 * @api
 */
extern int (*vfs_stream_tell)(FileStream * stream, int * success);

/**
 * @brief Flush file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 * @api
 */
extern int (*vfs_stream_flush)(FileStream * stream);

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
extern Error (*window_server_register_backend)(const char * name, WindowServerBackend * backend);

/**
 * @brief Load a backend. First you should register them via window_server_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if name is NULL
 * @return "NotFound" if a backend with the given name is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
extern Error (*window_server_load_backend)(const char * name);

/**
 * @brief Create a new WindowServerBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
extern WindowServerBackend * (*window_server_backend_new)(void);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
extern Error (*window_server_backend_set_function)(WindowServerBackend * backend, const char * name, void (* function) (void));

/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 * 
 * @api
 */
extern Error (*window_server_backend_get_function)(WindowServerBackend * backend, const char * name, void (** function) (void));

extern Error (*window_server_create_window)(const char * title, int w, int h, WindowServerWindow * parent, WindowServerWindow ** out);

extern Error (*window_server_destroy_window)(WindowServerWindow * this);

extern Error (*window_server_window_set_title)(WindowServerWindow * this, const char * title);

extern Error (*window_server_window_get_title)(WindowServerWindow * this, const char ** out);

/**
     * @brief HOLA BOLA
     * 
     */
extern Error (*window_server_window_set_mode)(WindowServerWindow * this, WindowServerWindowMode mode);

extern Error (*window_server_window_get_mode)(WindowServerWindow * this, WindowServerWindowMode * out);

extern Error (*window_server_window_set_size)(WindowServerWindow * this, int w, int h);

extern Error (*window_server_window_get_size)(WindowServerWindow * this, int * w, int * h);

extern Error (*window_server_window_set_position)(WindowServerWindow * this, int x, int y);

extern Error (*window_server_window_get_position)(WindowServerWindow * this, int * x, int * y);

extern Error (*window_server_window_set_fullscreen_display)(WindowServerWindow * this, WindowServerDisplay * fullscreen);


#endif

#if defined(HEAPI_LOAD_IMPL)
        void (*raw___he_update_full_trace_info)(const char * func, const char * file, i32 line);
    void * (*raw_tmalloc)(int size);
    void * (*raw_trealloc)(void * ptr, int size);
    void (*raw_tfree)(void * ptr);
    int (*raw_get_allocated_memory)(void);
    void (*raw_auto_free)(Object * object);
    Node * (*raw_node_new)(const char * name);
    Node * (*raw_from_node)(Node * node);
    void (*raw_node_set_name)(Node * node, const char * name);
    const char * (*raw_node_get_name)(Node * node);
    void (*raw_node_add_child)(Node * node, Node * child);
    int (*raw_node_remove_child)(Node * node, Node * child);
    int (*raw_node_remove_child_by_name)(Node * node, const char * name);
    int (*raw_node_remove_child_by_uid)(Node * node, UID uid);
    int (*raw_node_remove_all_children)(Node * node);
    Node * (*raw_node_get_child_by_name)(Node * node, const char * name);
    Node * (*raw_node_get_child_by_uid)(Node * node, UID uid);
    Node3D * (*raw_node3d_new)(const char * name);
    Window * (*raw_window_new)(const char * name, const char * title, int w, int h);
    void (*raw_window_set_title)(Window * this, const char * title);
    const char * (*raw_window_get_title)(const Window * this);
    CanvasItem * (*raw_canvas_item_new)(const char * name);
    Control * (*raw_control_new)(const char * name);
    Viewport * (*raw_viewport_new)(const char * name);
    Mat4 (*raw_mat4_new)(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
    Mat4 (*raw_mat4_new_zero)(void);
    Mat4 (*raw_mat4_new_one)(void);
    Mat4 (*raw_mat4_from_array)(const float *const array);
    Mat4 (*raw_mat4_from_mat4)(const Mat4 *const a);
    Mat4 (*raw_mat4_add)(const Mat4 *const a, const Mat4 *const b);
    Mat4 (*raw_mat4_sub)(const Mat4 *const a, const Mat4 *const b);
    Mat4 (*raw_mat4_mul)(const Mat4 *const a, const Mat4 *const b);
    Mat4 (*raw_mat4_scale)(const Mat4 *const a, const float factor);
    Mat4 (*raw_mat4_transpose)(const Mat4 *const a);
    Mat4 (*raw_mat4_inverse)(const Mat4 *const a);
    Vec3 (*raw_vec3_new)(float x, float y, float z);
    Vec3 (*raw_vec3_add)(const Vec3 *const a, const Vec3 *const b);
    Vec3 (*raw_vec3_sub)(const Vec3 *const a, const Vec3 *const b);
    Vec3 (*raw_vec3_scale)(const Vec3 *const a, const float factor);
    float (*raw_vec3_dot)(const Vec3 *const a, const Vec3 *const b);
    Vec3 (*raw_vec3_cross)(const Vec3 *const a, const Vec3 *const b);
    float (*raw_vec3_length)(const Vec3 *const a);
    Vec3 (*raw_vec3_normalize)(const Vec3 *const a);
    float (*raw_vec3_distance)(const Vec3 *const a, const Vec3 *const b);
    void (*raw_vec3_add_in)(Vec3 *const to, const Vec3 *const what);
    void (*raw_vec3_sub_in)(Vec3 *const from, const Vec3 *const what);
    void (*raw_vec3_scale_in)(Vec3 *const to, const float factor);
    void (*raw_vec3_normalize_in)(Vec3 *const a);
    Vec2 (*raw_vec2_new)(float x, float y, float z);
    Vec2 (*raw_vec2_add)(const Vec2 *const a, const Vec2 *const b);
    Vec2 (*raw_vec2_sub)(const Vec2 *const a, const Vec2 *const b);
    Vec2 (*raw_vec2_scale)(const Vec2 *const a, const float factor);
    float (*raw_vec2_dot)(const Vec2 *const a, const Vec2 *const b);
    float (*raw_vec2_length)(const Vec2 *const a);
    Vec2 (*raw_vec2_normalize)(const Vec2 *const a);
    float (*raw_vec2_distance)(const Vec2 *const a, const Vec2 *const b);
    void (*raw_vec2_add_in)(Vec2 *const to, const Vec2 *const what);
    void (*raw_vec2_sub_in)(Vec2 *const from, const Vec2 *const what);
    void (*raw_vec2_scale_in)(Vec2 *const to, const float factor);
    void (*raw_vec2_normalize_in)(Vec2 *const a);
    UID (*raw_uid_new)(void);
    int (*raw_vfs_mount_res)(const char * path, const char * mount_point);
    int (*raw_vfs_unmount_res)(const char * mount_point);
    int (*raw_vfs_mount_rfs)(const char * mount_point);
    int (*raw_vfs_mount_rfs_whitelist)(const char ** whitelist, int count, const char * mount_point);
    int (*raw_vfs_unmount_rfs)(void);
    int (*raw_vfs_res_path_exists)(const char * path);
    int (*raw_vfs_usr_path_exists)(const char * path, int prefer_res);
    void * (*raw_vfs_res_read_file)(const char * path, int * size);
    FileStream * (*raw_vfs_res_stream_open)(const char * path);
    void * (*raw_vfs_usr_read_file)(const char * path, int * size, int prefer_res);
    int (*raw_vfs_usr_write_file)(const char * path, const void * data, int size);
    FileStream * (*raw_vfs_usr_stream_open)(const char * path, int prefer_res);
    int (*raw_vfs_stream_size)(FileStream * stream);
    int (*raw_vfs_stream_read_n)(FileStream * stream, void * buffer, int size);
    void * (*raw_vfs_stream_read_all)(FileStream * stream, int * size);
    int (*raw_vfs_stream_write)(FileStream * stream, void * buffer, int size);
    int (*raw_vfs_stream_seek)(FileStream * stream, FSSeekFrom whence, int offset);
    int (*raw_vfs_stream_tell)(FileStream * stream, int * success);
    int (*raw_vfs_stream_flush)(FileStream * stream);
    void (*raw_vfs_stream_close)(FileStream * stream);
    Error (*raw_window_server_register_backend)(const char * name, WindowServerBackend * backend);
    Error (*raw_window_server_load_backend)(const char * name);
    WindowServerBackend * (*raw_window_server_backend_new)(void);
    Error (*raw_window_server_backend_set_function)(WindowServerBackend * backend, const char * name, void (* function) (void));
    Error (*raw_window_server_backend_get_function)(WindowServerBackend * backend, const char * name, void (** function) (void));
    Error (*raw_window_server_create_window)(const char * title, int w, int h, WindowServerWindow * parent, WindowServerWindow ** out);
    Error (*raw_window_server_destroy_window)(WindowServerWindow * this);
    Error (*raw_window_server_window_set_title)(WindowServerWindow * this, const char * title);
    Error (*raw_window_server_window_get_title)(WindowServerWindow * this, const char ** out);
    Error (*raw_window_server_window_set_mode)(WindowServerWindow * this, WindowServerWindowMode mode);
    Error (*raw_window_server_window_get_mode)(WindowServerWindow * this, WindowServerWindowMode * out);
    Error (*raw_window_server_window_set_size)(WindowServerWindow * this, int w, int h);
    Error (*raw_window_server_window_get_size)(WindowServerWindow * this, int * w, int * h);
    Error (*raw_window_server_window_set_position)(WindowServerWindow * this, int x, int y);
    Error (*raw_window_server_window_get_position)(WindowServerWindow * this, int * x, int * y);
    Error (*raw_window_server_window_set_fullscreen_display)(WindowServerWindow * this, WindowServerDisplay * fullscreen);


    #if !defined(HEAPI_FULL_TRACE)
            void (*__he_update_full_trace_info)(const char * func, const char * file, i32 line);
    void * (*tmalloc)(int size);
    void * (*trealloc)(void * ptr, int size);
    void (*tfree)(void * ptr);
    int (*get_allocated_memory)(void);
    void (*auto_free)(Object * object);
    Node * (*node_new)(const char * name);
    Node * (*from_node)(Node * node);
    void (*node_set_name)(Node * node, const char * name);
    const char * (*node_get_name)(Node * node);
    void (*node_add_child)(Node * node, Node * child);
    int (*node_remove_child)(Node * node, Node * child);
    int (*node_remove_child_by_name)(Node * node, const char * name);
    int (*node_remove_child_by_uid)(Node * node, UID uid);
    int (*node_remove_all_children)(Node * node);
    Node * (*node_get_child_by_name)(Node * node, const char * name);
    Node * (*node_get_child_by_uid)(Node * node, UID uid);
    Node3D * (*node3d_new)(const char * name);
    Window * (*window_new)(const char * name, const char * title, int w, int h);
    void (*window_set_title)(Window * this, const char * title);
    const char * (*window_get_title)(const Window * this);
    CanvasItem * (*canvas_item_new)(const char * name);
    Control * (*control_new)(const char * name);
    Viewport * (*viewport_new)(const char * name);
    Mat4 (*mat4_new)(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
    Mat4 (*mat4_new_zero)(void);
    Mat4 (*mat4_new_one)(void);
    Mat4 (*mat4_from_array)(const float *const array);
    Mat4 (*mat4_from_mat4)(const Mat4 *const a);
    Mat4 (*mat4_add)(const Mat4 *const a, const Mat4 *const b);
    Mat4 (*mat4_sub)(const Mat4 *const a, const Mat4 *const b);
    Mat4 (*mat4_mul)(const Mat4 *const a, const Mat4 *const b);
    Mat4 (*mat4_scale)(const Mat4 *const a, const float factor);
    Mat4 (*mat4_transpose)(const Mat4 *const a);
    Mat4 (*mat4_inverse)(const Mat4 *const a);
    Vec3 (*vec3_new)(float x, float y, float z);
    Vec3 (*vec3_add)(const Vec3 *const a, const Vec3 *const b);
    Vec3 (*vec3_sub)(const Vec3 *const a, const Vec3 *const b);
    Vec3 (*vec3_scale)(const Vec3 *const a, const float factor);
    float (*vec3_dot)(const Vec3 *const a, const Vec3 *const b);
    Vec3 (*vec3_cross)(const Vec3 *const a, const Vec3 *const b);
    float (*vec3_length)(const Vec3 *const a);
    Vec3 (*vec3_normalize)(const Vec3 *const a);
    float (*vec3_distance)(const Vec3 *const a, const Vec3 *const b);
    void (*vec3_add_in)(Vec3 *const to, const Vec3 *const what);
    void (*vec3_sub_in)(Vec3 *const from, const Vec3 *const what);
    void (*vec3_scale_in)(Vec3 *const to, const float factor);
    void (*vec3_normalize_in)(Vec3 *const a);
    Vec2 (*vec2_new)(float x, float y, float z);
    Vec2 (*vec2_add)(const Vec2 *const a, const Vec2 *const b);
    Vec2 (*vec2_sub)(const Vec2 *const a, const Vec2 *const b);
    Vec2 (*vec2_scale)(const Vec2 *const a, const float factor);
    float (*vec2_dot)(const Vec2 *const a, const Vec2 *const b);
    float (*vec2_length)(const Vec2 *const a);
    Vec2 (*vec2_normalize)(const Vec2 *const a);
    float (*vec2_distance)(const Vec2 *const a, const Vec2 *const b);
    void (*vec2_add_in)(Vec2 *const to, const Vec2 *const what);
    void (*vec2_sub_in)(Vec2 *const from, const Vec2 *const what);
    void (*vec2_scale_in)(Vec2 *const to, const float factor);
    void (*vec2_normalize_in)(Vec2 *const a);
    UID (*uid_new)(void);
    int (*vfs_mount_res)(const char * path, const char * mount_point);
    int (*vfs_unmount_res)(const char * mount_point);
    int (*vfs_mount_rfs)(const char * mount_point);
    int (*vfs_mount_rfs_whitelist)(const char ** whitelist, int count, const char * mount_point);
    int (*vfs_unmount_rfs)(void);
    int (*vfs_res_path_exists)(const char * path);
    int (*vfs_usr_path_exists)(const char * path, int prefer_res);
    void * (*vfs_res_read_file)(const char * path, int * size);
    FileStream * (*vfs_res_stream_open)(const char * path);
    void * (*vfs_usr_read_file)(const char * path, int * size, int prefer_res);
    int (*vfs_usr_write_file)(const char * path, const void * data, int size);
    FileStream * (*vfs_usr_stream_open)(const char * path, int prefer_res);
    int (*vfs_stream_size)(FileStream * stream);
    int (*vfs_stream_read_n)(FileStream * stream, void * buffer, int size);
    void * (*vfs_stream_read_all)(FileStream * stream, int * size);
    int (*vfs_stream_write)(FileStream * stream, void * buffer, int size);
    int (*vfs_stream_seek)(FileStream * stream, FSSeekFrom whence, int offset);
    int (*vfs_stream_tell)(FileStream * stream, int * success);
    int (*vfs_stream_flush)(FileStream * stream);
    void (*vfs_stream_close)(FileStream * stream);
    Error (*window_server_register_backend)(const char * name, WindowServerBackend * backend);
    Error (*window_server_load_backend)(const char * name);
    WindowServerBackend * (*window_server_backend_new)(void);
    Error (*window_server_backend_set_function)(WindowServerBackend * backend, const char * name, void (* function) (void));
    Error (*window_server_backend_get_function)(WindowServerBackend * backend, const char * name, void (** function) (void));
    Error (*window_server_create_window)(const char * title, int w, int h, WindowServerWindow * parent, WindowServerWindow ** out);
    Error (*window_server_destroy_window)(WindowServerWindow * this);
    Error (*window_server_window_set_title)(WindowServerWindow * this, const char * title);
    Error (*window_server_window_get_title)(WindowServerWindow * this, const char ** out);
    Error (*window_server_window_set_mode)(WindowServerWindow * this, WindowServerWindowMode mode);
    Error (*window_server_window_get_mode)(WindowServerWindow * this, WindowServerWindowMode * out);
    Error (*window_server_window_set_size)(WindowServerWindow * this, int w, int h);
    Error (*window_server_window_get_size)(WindowServerWindow * this, int * w, int * h);
    Error (*window_server_window_set_position)(WindowServerWindow * this, int x, int y);
    Error (*window_server_window_get_position)(WindowServerWindow * this, int * x, int * y);
    Error (*window_server_window_set_fullscreen_display)(WindowServerWindow * this, WindowServerDisplay * fullscreen);

    #endif

    void ___hate_engine_runtime_init(void* (*proc_addr)(const char* name)) {
                raw___he_update_full_trace_info = (void (*)(const char *, const char *, i32))proc_addr("__he_update_full_trace_info");
        raw_tmalloc = (void * (*)(int))proc_addr("tmalloc");
        raw_trealloc = (void * (*)(void *, int))proc_addr("trealloc");
        raw_tfree = (void (*)(void *))proc_addr("tfree");
        raw_get_allocated_memory = (int (*)(void))proc_addr("get_allocated_memory");
        raw_auto_free = (void (*)(Object *))proc_addr("auto_free");
        raw_node_new = (Node * (*)(const char *))proc_addr("node_new");
        raw_from_node = (Node * (*)(Node *))proc_addr("from_node");
        raw_node_set_name = (void (*)(Node *, const char *))proc_addr("node_set_name");
        raw_node_get_name = (const char * (*)(Node *))proc_addr("node_get_name");
        raw_node_add_child = (void (*)(Node *, Node *))proc_addr("node_add_child");
        raw_node_remove_child = (int (*)(Node *, Node *))proc_addr("node_remove_child");
        raw_node_remove_child_by_name = (int (*)(Node *, const char *))proc_addr("node_remove_child_by_name");
        raw_node_remove_child_by_uid = (int (*)(Node *, UID))proc_addr("node_remove_child_by_uid");
        raw_node_remove_all_children = (int (*)(Node *))proc_addr("node_remove_all_children");
        raw_node_get_child_by_name = (Node * (*)(Node *, const char *))proc_addr("node_get_child_by_name");
        raw_node_get_child_by_uid = (Node * (*)(Node *, UID))proc_addr("node_get_child_by_uid");
        raw_node3d_new = (Node3D * (*)(const char *))proc_addr("node3d_new");
        raw_window_new = (Window * (*)(const char *, const char *, int, int))proc_addr("window_new");
        raw_window_set_title = (void (*)(Window *, const char *))proc_addr("window_set_title");
        raw_window_get_title = (const char * (*)(const Window *))proc_addr("window_get_title");
        raw_canvas_item_new = (CanvasItem * (*)(const char *))proc_addr("canvas_item_new");
        raw_control_new = (Control * (*)(const char *))proc_addr("control_new");
        raw_viewport_new = (Viewport * (*)(const char *))proc_addr("viewport_new");
        raw_mat4_new = (Mat4 (*)(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float))proc_addr("mat4_new");
        raw_mat4_new_zero = (Mat4 (*)(void))proc_addr("mat4_new_zero");
        raw_mat4_new_one = (Mat4 (*)(void))proc_addr("mat4_new_one");
        raw_mat4_from_array = (Mat4 (*)(const float *const))proc_addr("mat4_from_array");
        raw_mat4_from_mat4 = (Mat4 (*)(const Mat4 *const))proc_addr("mat4_from_mat4");
        raw_mat4_add = (Mat4 (*)(const Mat4 *const, const Mat4 *const))proc_addr("mat4_add");
        raw_mat4_sub = (Mat4 (*)(const Mat4 *const, const Mat4 *const))proc_addr("mat4_sub");
        raw_mat4_mul = (Mat4 (*)(const Mat4 *const, const Mat4 *const))proc_addr("mat4_mul");
        raw_mat4_scale = (Mat4 (*)(const Mat4 *const, const float))proc_addr("mat4_scale");
        raw_mat4_transpose = (Mat4 (*)(const Mat4 *const))proc_addr("mat4_transpose");
        raw_mat4_inverse = (Mat4 (*)(const Mat4 *const))proc_addr("mat4_inverse");
        raw_vec3_new = (Vec3 (*)(float, float, float))proc_addr("vec3_new");
        raw_vec3_add = (Vec3 (*)(const Vec3 *const, const Vec3 *const))proc_addr("vec3_add");
        raw_vec3_sub = (Vec3 (*)(const Vec3 *const, const Vec3 *const))proc_addr("vec3_sub");
        raw_vec3_scale = (Vec3 (*)(const Vec3 *const, const float))proc_addr("vec3_scale");
        raw_vec3_dot = (float (*)(const Vec3 *const, const Vec3 *const))proc_addr("vec3_dot");
        raw_vec3_cross = (Vec3 (*)(const Vec3 *const, const Vec3 *const))proc_addr("vec3_cross");
        raw_vec3_length = (float (*)(const Vec3 *const))proc_addr("vec3_length");
        raw_vec3_normalize = (Vec3 (*)(const Vec3 *const))proc_addr("vec3_normalize");
        raw_vec3_distance = (float (*)(const Vec3 *const, const Vec3 *const))proc_addr("vec3_distance");
        raw_vec3_add_in = (void (*)(Vec3 *const, const Vec3 *const))proc_addr("vec3_add_in");
        raw_vec3_sub_in = (void (*)(Vec3 *const, const Vec3 *const))proc_addr("vec3_sub_in");
        raw_vec3_scale_in = (void (*)(Vec3 *const, const float))proc_addr("vec3_scale_in");
        raw_vec3_normalize_in = (void (*)(Vec3 *const))proc_addr("vec3_normalize_in");
        raw_vec2_new = (Vec2 (*)(float, float, float))proc_addr("vec2_new");
        raw_vec2_add = (Vec2 (*)(const Vec2 *const, const Vec2 *const))proc_addr("vec2_add");
        raw_vec2_sub = (Vec2 (*)(const Vec2 *const, const Vec2 *const))proc_addr("vec2_sub");
        raw_vec2_scale = (Vec2 (*)(const Vec2 *const, const float))proc_addr("vec2_scale");
        raw_vec2_dot = (float (*)(const Vec2 *const, const Vec2 *const))proc_addr("vec2_dot");
        raw_vec2_length = (float (*)(const Vec2 *const))proc_addr("vec2_length");
        raw_vec2_normalize = (Vec2 (*)(const Vec2 *const))proc_addr("vec2_normalize");
        raw_vec2_distance = (float (*)(const Vec2 *const, const Vec2 *const))proc_addr("vec2_distance");
        raw_vec2_add_in = (void (*)(Vec2 *const, const Vec2 *const))proc_addr("vec2_add_in");
        raw_vec2_sub_in = (void (*)(Vec2 *const, const Vec2 *const))proc_addr("vec2_sub_in");
        raw_vec2_scale_in = (void (*)(Vec2 *const, const float))proc_addr("vec2_scale_in");
        raw_vec2_normalize_in = (void (*)(Vec2 *const))proc_addr("vec2_normalize_in");
        raw_uid_new = (UID (*)(void))proc_addr("uid_new");
        raw_vfs_mount_res = (int (*)(const char *, const char *))proc_addr("vfs_mount_res");
        raw_vfs_unmount_res = (int (*)(const char *))proc_addr("vfs_unmount_res");
        raw_vfs_mount_rfs = (int (*)(const char *))proc_addr("vfs_mount_rfs");
        raw_vfs_mount_rfs_whitelist = (int (*)(const char **, int, const char *))proc_addr("vfs_mount_rfs_whitelist");
        raw_vfs_unmount_rfs = (int (*)(void))proc_addr("vfs_unmount_rfs");
        raw_vfs_res_path_exists = (int (*)(const char *))proc_addr("vfs_res_path_exists");
        raw_vfs_usr_path_exists = (int (*)(const char *, int))proc_addr("vfs_usr_path_exists");
        raw_vfs_res_read_file = (void * (*)(const char *, int *))proc_addr("vfs_res_read_file");
        raw_vfs_res_stream_open = (FileStream * (*)(const char *))proc_addr("vfs_res_stream_open");
        raw_vfs_usr_read_file = (void * (*)(const char *, int *, int))proc_addr("vfs_usr_read_file");
        raw_vfs_usr_write_file = (int (*)(const char *, const void *, int))proc_addr("vfs_usr_write_file");
        raw_vfs_usr_stream_open = (FileStream * (*)(const char *, int))proc_addr("vfs_usr_stream_open");
        raw_vfs_stream_size = (int (*)(FileStream *))proc_addr("vfs_stream_size");
        raw_vfs_stream_read_n = (int (*)(FileStream *, void *, int))proc_addr("vfs_stream_read_n");
        raw_vfs_stream_read_all = (void * (*)(FileStream *, int *))proc_addr("vfs_stream_read_all");
        raw_vfs_stream_write = (int (*)(FileStream *, void *, int))proc_addr("vfs_stream_write");
        raw_vfs_stream_seek = (int (*)(FileStream *, FSSeekFrom, int))proc_addr("vfs_stream_seek");
        raw_vfs_stream_tell = (int (*)(FileStream *, int *))proc_addr("vfs_stream_tell");
        raw_vfs_stream_flush = (int (*)(FileStream *))proc_addr("vfs_stream_flush");
        raw_vfs_stream_close = (void (*)(FileStream *))proc_addr("vfs_stream_close");
        raw_window_server_register_backend = (Error (*)(const char *, WindowServerBackend *))proc_addr("window_server_register_backend");
        raw_window_server_load_backend = (Error (*)(const char *))proc_addr("window_server_load_backend");
        raw_window_server_backend_new = (WindowServerBackend * (*)(void))proc_addr("window_server_backend_new");
        raw_window_server_backend_set_function = (Error (*)(WindowServerBackend *, const char *, void (*)(void)))proc_addr("window_server_backend_set_function");
        raw_window_server_backend_get_function = (Error (*)(WindowServerBackend *, const char *, void (**)(void)))proc_addr("window_server_backend_get_function");


        #if !defined(HEAPI_FULL_TRACE)
                        __he_update_full_trace_info = raw___he_update_full_trace_info;
            tmalloc = raw_tmalloc;
            trealloc = raw_trealloc;
            tfree = raw_tfree;
            get_allocated_memory = raw_get_allocated_memory;
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
            mat4_new = raw_mat4_new;
            mat4_new_zero = raw_mat4_new_zero;
            mat4_new_one = raw_mat4_new_one;
            mat4_from_array = raw_mat4_from_array;
            mat4_from_mat4 = raw_mat4_from_mat4;
            mat4_add = raw_mat4_add;
            mat4_sub = raw_mat4_sub;
            mat4_mul = raw_mat4_mul;
            mat4_scale = raw_mat4_scale;
            mat4_transpose = raw_mat4_transpose;
            mat4_inverse = raw_mat4_inverse;
            vec3_new = raw_vec3_new;
            vec3_add = raw_vec3_add;
            vec3_sub = raw_vec3_sub;
            vec3_scale = raw_vec3_scale;
            vec3_dot = raw_vec3_dot;
            vec3_cross = raw_vec3_cross;
            vec3_length = raw_vec3_length;
            vec3_normalize = raw_vec3_normalize;
            vec3_distance = raw_vec3_distance;
            vec3_add_in = raw_vec3_add_in;
            vec3_sub_in = raw_vec3_sub_in;
            vec3_scale_in = raw_vec3_scale_in;
            vec3_normalize_in = raw_vec3_normalize_in;
            vec2_new = raw_vec2_new;
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
            window_server_backend_set_function = raw_window_server_backend_set_function;
            window_server_backend_get_function = raw_window_server_backend_get_function;

        #endif
    }

    void ___hate_engine_runtime_init_window_server(WindowServerBackend* backend) {
        raw_window_server_backend_get_function(backend, "create_window", (void (**)(void))raw_window_server_create_window);
        raw_window_server_backend_get_function(backend, "destroy_window", (void (**)(void))raw_window_server_destroy_window);
        raw_window_server_backend_get_function(backend, "window_set_title", (void (**)(void))raw_window_server_window_set_title);
        raw_window_server_backend_get_function(backend, "window_get_title", (void (**)(void))raw_window_server_window_get_title);
        raw_window_server_backend_get_function(backend, "window_set_mode", (void (**)(void))raw_window_server_window_set_mode);
        raw_window_server_backend_get_function(backend, "window_get_mode", (void (**)(void))raw_window_server_window_get_mode);
        raw_window_server_backend_get_function(backend, "window_set_size", (void (**)(void))raw_window_server_window_set_size);
        raw_window_server_backend_get_function(backend, "window_get_size", (void (**)(void))raw_window_server_window_get_size);
        raw_window_server_backend_get_function(backend, "window_set_position", (void (**)(void))raw_window_server_window_set_position);
        raw_window_server_backend_get_function(backend, "window_get_position", (void (**)(void))raw_window_server_window_get_position);
        raw_window_server_backend_get_function(backend, "window_set_fullscreen_display", (void (**)(void))raw_window_server_window_set_fullscreen_display);
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
            window_server_window_set_fullscreen_display = raw_window_server_window_set_fullscreen_display;
        #endif
    }
#endif
#endif

#if defined(HEAPI_FULL_TRACE)
void * full_trace_tmalloc(const char* ___file___, uint32_t ___line___, int);
void * full_trace_trealloc(const char* ___file___, uint32_t ___line___, void *, int);
void full_trace_tfree(const char* ___file___, uint32_t ___line___, void *);
int full_trace_get_allocated_memory(const char* ___file___, uint32_t ___line___);
void full_trace_auto_free(const char* ___file___, uint32_t ___line___, Object *);
Node * full_trace_node_new(const char* ___file___, uint32_t ___line___, const char *);
Node * full_trace_from_node(const char* ___file___, uint32_t ___line___, Node *);
void full_trace_node_set_name(const char* ___file___, uint32_t ___line___, Node *, const char *);
const char * full_trace_node_get_name(const char* ___file___, uint32_t ___line___, Node *);
void full_trace_node_add_child(const char* ___file___, uint32_t ___line___, Node *, Node *);
int full_trace_node_remove_child(const char* ___file___, uint32_t ___line___, Node *, Node *);
int full_trace_node_remove_child_by_name(const char* ___file___, uint32_t ___line___, Node *, const char *);
int full_trace_node_remove_child_by_uid(const char* ___file___, uint32_t ___line___, Node *, UID);
int full_trace_node_remove_all_children(const char* ___file___, uint32_t ___line___, Node *);
Node * full_trace_node_get_child_by_name(const char* ___file___, uint32_t ___line___, Node *, const char *);
Node * full_trace_node_get_child_by_uid(const char* ___file___, uint32_t ___line___, Node *, UID);
Node3D * full_trace_node3d_new(const char* ___file___, uint32_t ___line___, const char *);
Window * full_trace_window_new(const char* ___file___, uint32_t ___line___, const char *, const char *, int, int);
void full_trace_window_set_title(const char* ___file___, uint32_t ___line___, Window *, const char *);
const char * full_trace_window_get_title(const char* ___file___, uint32_t ___line___, const Window *);
CanvasItem * full_trace_canvas_item_new(const char* ___file___, uint32_t ___line___, const char *);
Control * full_trace_control_new(const char* ___file___, uint32_t ___line___, const char *);
Viewport * full_trace_viewport_new(const char* ___file___, uint32_t ___line___, const char *);
Mat4 full_trace_mat4_new(const char* ___file___, uint32_t ___line___, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
Mat4 full_trace_mat4_new_zero(const char* ___file___, uint32_t ___line___);
Mat4 full_trace_mat4_new_one(const char* ___file___, uint32_t ___line___);
Mat4 full_trace_mat4_from_array(const char* ___file___, uint32_t ___line___, const float *const);
Mat4 full_trace_mat4_from_mat4(const char* ___file___, uint32_t ___line___, const Mat4 *const);
Mat4 full_trace_mat4_add(const char* ___file___, uint32_t ___line___, const Mat4 *const, const Mat4 *const);
Mat4 full_trace_mat4_sub(const char* ___file___, uint32_t ___line___, const Mat4 *const, const Mat4 *const);
Mat4 full_trace_mat4_mul(const char* ___file___, uint32_t ___line___, const Mat4 *const, const Mat4 *const);
Mat4 full_trace_mat4_scale(const char* ___file___, uint32_t ___line___, const Mat4 *const, const float);
Mat4 full_trace_mat4_transpose(const char* ___file___, uint32_t ___line___, const Mat4 *const);
Mat4 full_trace_mat4_inverse(const char* ___file___, uint32_t ___line___, const Mat4 *const);
Vec3 full_trace_vec3_new(const char* ___file___, uint32_t ___line___, float, float, float);
Vec3 full_trace_vec3_add(const char* ___file___, uint32_t ___line___, const Vec3 *const, const Vec3 *const);
Vec3 full_trace_vec3_sub(const char* ___file___, uint32_t ___line___, const Vec3 *const, const Vec3 *const);
Vec3 full_trace_vec3_scale(const char* ___file___, uint32_t ___line___, const Vec3 *const, const float);
float full_trace_vec3_dot(const char* ___file___, uint32_t ___line___, const Vec3 *const, const Vec3 *const);
Vec3 full_trace_vec3_cross(const char* ___file___, uint32_t ___line___, const Vec3 *const, const Vec3 *const);
float full_trace_vec3_length(const char* ___file___, uint32_t ___line___, const Vec3 *const);
Vec3 full_trace_vec3_normalize(const char* ___file___, uint32_t ___line___, const Vec3 *const);
float full_trace_vec3_distance(const char* ___file___, uint32_t ___line___, const Vec3 *const, const Vec3 *const);
void full_trace_vec3_add_in(const char* ___file___, uint32_t ___line___, Vec3 *const, const Vec3 *const);
void full_trace_vec3_sub_in(const char* ___file___, uint32_t ___line___, Vec3 *const, const Vec3 *const);
void full_trace_vec3_scale_in(const char* ___file___, uint32_t ___line___, Vec3 *const, const float);
void full_trace_vec3_normalize_in(const char* ___file___, uint32_t ___line___, Vec3 *const);
Vec2 full_trace_vec2_new(const char* ___file___, uint32_t ___line___, float, float, float);
Vec2 full_trace_vec2_add(const char* ___file___, uint32_t ___line___, const Vec2 *const, const Vec2 *const);
Vec2 full_trace_vec2_sub(const char* ___file___, uint32_t ___line___, const Vec2 *const, const Vec2 *const);
Vec2 full_trace_vec2_scale(const char* ___file___, uint32_t ___line___, const Vec2 *const, const float);
float full_trace_vec2_dot(const char* ___file___, uint32_t ___line___, const Vec2 *const, const Vec2 *const);
float full_trace_vec2_length(const char* ___file___, uint32_t ___line___, const Vec2 *const);
Vec2 full_trace_vec2_normalize(const char* ___file___, uint32_t ___line___, const Vec2 *const);
float full_trace_vec2_distance(const char* ___file___, uint32_t ___line___, const Vec2 *const, const Vec2 *const);
void full_trace_vec2_add_in(const char* ___file___, uint32_t ___line___, Vec2 *const, const Vec2 *const);
void full_trace_vec2_sub_in(const char* ___file___, uint32_t ___line___, Vec2 *const, const Vec2 *const);
void full_trace_vec2_scale_in(const char* ___file___, uint32_t ___line___, Vec2 *const, const float);
void full_trace_vec2_normalize_in(const char* ___file___, uint32_t ___line___, Vec2 *const);
UID full_trace_uid_new(const char* ___file___, uint32_t ___line___);
int full_trace_vfs_mount_res(const char* ___file___, uint32_t ___line___, const char *, const char *);
int full_trace_vfs_unmount_res(const char* ___file___, uint32_t ___line___, const char *);
int full_trace_vfs_mount_rfs(const char* ___file___, uint32_t ___line___, const char *);
int full_trace_vfs_mount_rfs_whitelist(const char* ___file___, uint32_t ___line___, const char **, int, const char *);
int full_trace_vfs_unmount_rfs(const char* ___file___, uint32_t ___line___);
int full_trace_vfs_res_path_exists(const char* ___file___, uint32_t ___line___, const char *);
int full_trace_vfs_usr_path_exists(const char* ___file___, uint32_t ___line___, const char *, int);
void * full_trace_vfs_res_read_file(const char* ___file___, uint32_t ___line___, const char *, int *);
FileStream * full_trace_vfs_res_stream_open(const char* ___file___, uint32_t ___line___, const char *);
void * full_trace_vfs_usr_read_file(const char* ___file___, uint32_t ___line___, const char *, int *, int);
int full_trace_vfs_usr_write_file(const char* ___file___, uint32_t ___line___, const char *, const void *, int);
FileStream * full_trace_vfs_usr_stream_open(const char* ___file___, uint32_t ___line___, const char *, int);
int full_trace_vfs_stream_size(const char* ___file___, uint32_t ___line___, FileStream *);
int full_trace_vfs_stream_read_n(const char* ___file___, uint32_t ___line___, FileStream *, void *, int);
void * full_trace_vfs_stream_read_all(const char* ___file___, uint32_t ___line___, FileStream *, int *);
int full_trace_vfs_stream_write(const char* ___file___, uint32_t ___line___, FileStream *, void *, int);
int full_trace_vfs_stream_seek(const char* ___file___, uint32_t ___line___, FileStream *, FSSeekFrom, int);
int full_trace_vfs_stream_tell(const char* ___file___, uint32_t ___line___, FileStream *, int *);
int full_trace_vfs_stream_flush(const char* ___file___, uint32_t ___line___, FileStream *);
void full_trace_vfs_stream_close(const char* ___file___, uint32_t ___line___, FileStream *);
Error full_trace_window_server_register_backend(const char* ___file___, uint32_t ___line___, const char *, WindowServerBackend *);
Error full_trace_window_server_load_backend(const char* ___file___, uint32_t ___line___, const char *);
WindowServerBackend * full_trace_window_server_backend_new(const char* ___file___, uint32_t ___line___);
Error full_trace_window_server_backend_set_function(const char* ___file___, uint32_t ___line___, WindowServerBackend *, const char *, void (*)(void));
Error full_trace_window_server_backend_get_function(const char* ___file___, uint32_t ___line___, WindowServerBackend *, const char *, void (**)(void));
Error full_trace_window_server_create_window(const char* ___file___, uint32_t ___line___, const char *, int, int, WindowServerWindow *, WindowServerWindow **);
Error full_trace_window_server_destroy_window(const char* ___file___, uint32_t ___line___, WindowServerWindow *);
Error full_trace_window_server_window_set_title(const char* ___file___, uint32_t ___line___, WindowServerWindow *, const char *);
Error full_trace_window_server_window_get_title(const char* ___file___, uint32_t ___line___, WindowServerWindow *, const char **);
Error full_trace_window_server_window_set_mode(const char* ___file___, uint32_t ___line___, WindowServerWindow *, WindowServerWindowMode);
Error full_trace_window_server_window_get_mode(const char* ___file___, uint32_t ___line___, WindowServerWindow *, WindowServerWindowMode *);
Error full_trace_window_server_window_set_size(const char* ___file___, uint32_t ___line___, WindowServerWindow *, int, int);
Error full_trace_window_server_window_get_size(const char* ___file___, uint32_t ___line___, WindowServerWindow *, int *, int *);
Error full_trace_window_server_window_set_position(const char* ___file___, uint32_t ___line___, WindowServerWindow *, int, int);
Error full_trace_window_server_window_get_position(const char* ___file___, uint32_t ___line___, WindowServerWindow *, int *, int *);
Error full_trace_window_server_window_set_fullscreen_display(const char* ___file___, uint32_t ___line___, WindowServerWindow *, WindowServerDisplay *);


#if defined(HEAPI_LOAD_IMPL)
    inline void * full_trace_tmalloc(const char* ___file___, uint32_t ___line___, int size) {
    raw___he_update_full_trace_info("tmalloc", ___file___, ___line___);
    void * result = raw_tmalloc(size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void * full_trace_trealloc(const char* ___file___, uint32_t ___line___, void * ptr, int size) {
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

inline int full_trace_get_allocated_memory(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("get_allocated_memory", ___file___, ___line___);
    int result = raw_get_allocated_memory();
    raw___he_update_full_trace_info("", "", -1);
    return result;
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

inline int full_trace_node_remove_child(const char* ___file___, uint32_t ___line___, Node * node, Node * child) {
    raw___he_update_full_trace_info("node_remove_child", ___file___, ___line___);
    int result = raw_node_remove_child(node, child);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_node_remove_child_by_name(const char* ___file___, uint32_t ___line___, Node * node, const char * name) {
    raw___he_update_full_trace_info("node_remove_child_by_name", ___file___, ___line___);
    int result = raw_node_remove_child_by_name(node, name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_node_remove_child_by_uid(const char* ___file___, uint32_t ___line___, Node * node, UID uid) {
    raw___he_update_full_trace_info("node_remove_child_by_uid", ___file___, ___line___);
    int result = raw_node_remove_child_by_uid(node, uid);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_node_remove_all_children(const char* ___file___, uint32_t ___line___, Node * node) {
    raw___he_update_full_trace_info("node_remove_all_children", ___file___, ___line___);
    int result = raw_node_remove_all_children(node);
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

inline Window * full_trace_window_new(const char* ___file___, uint32_t ___line___, const char * name, const char * title, int w, int h) {
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

inline Mat4 full_trace_mat4_new(const char* ___file___, uint32_t ___line___, float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) {
    raw___he_update_full_trace_info("mat4_new", ___file___, ___line___);
    Mat4 result = raw_mat4_new(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_new_zero(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("mat4_new_zero", ___file___, ___line___);
    Mat4 result = raw_mat4_new_zero();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_new_one(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("mat4_new_one", ___file___, ___line___);
    Mat4 result = raw_mat4_new_one();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_from_array(const char* ___file___, uint32_t ___line___, const float *const array) {
    raw___he_update_full_trace_info("mat4_from_array", ___file___, ___line___);
    Mat4 result = raw_mat4_from_array(array);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_from_mat4(const char* ___file___, uint32_t ___line___, const Mat4 *const a) {
    raw___he_update_full_trace_info("mat4_from_mat4", ___file___, ___line___);
    Mat4 result = raw_mat4_from_mat4(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_add(const char* ___file___, uint32_t ___line___, const Mat4 *const a, const Mat4 *const b) {
    raw___he_update_full_trace_info("mat4_add", ___file___, ___line___);
    Mat4 result = raw_mat4_add(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_sub(const char* ___file___, uint32_t ___line___, const Mat4 *const a, const Mat4 *const b) {
    raw___he_update_full_trace_info("mat4_sub", ___file___, ___line___);
    Mat4 result = raw_mat4_sub(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_mul(const char* ___file___, uint32_t ___line___, const Mat4 *const a, const Mat4 *const b) {
    raw___he_update_full_trace_info("mat4_mul", ___file___, ___line___);
    Mat4 result = raw_mat4_mul(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_scale(const char* ___file___, uint32_t ___line___, const Mat4 *const a, const float factor) {
    raw___he_update_full_trace_info("mat4_scale", ___file___, ___line___);
    Mat4 result = raw_mat4_scale(a, factor);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_transpose(const char* ___file___, uint32_t ___line___, const Mat4 *const a) {
    raw___he_update_full_trace_info("mat4_transpose", ___file___, ___line___);
    Mat4 result = raw_mat4_transpose(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_inverse(const char* ___file___, uint32_t ___line___, const Mat4 *const a) {
    raw___he_update_full_trace_info("mat4_inverse", ___file___, ___line___);
    Mat4 result = raw_mat4_inverse(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_new(const char* ___file___, uint32_t ___line___, float x, float y, float z) {
    raw___he_update_full_trace_info("vec3_new", ___file___, ___line___);
    Vec3 result = raw_vec3_new(x, y, z);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_add(const char* ___file___, uint32_t ___line___, const Vec3 *const a, const Vec3 *const b) {
    raw___he_update_full_trace_info("vec3_add", ___file___, ___line___);
    Vec3 result = raw_vec3_add(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_sub(const char* ___file___, uint32_t ___line___, const Vec3 *const a, const Vec3 *const b) {
    raw___he_update_full_trace_info("vec3_sub", ___file___, ___line___);
    Vec3 result = raw_vec3_sub(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_scale(const char* ___file___, uint32_t ___line___, const Vec3 *const a, const float factor) {
    raw___he_update_full_trace_info("vec3_scale", ___file___, ___line___);
    Vec3 result = raw_vec3_scale(a, factor);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_vec3_dot(const char* ___file___, uint32_t ___line___, const Vec3 *const a, const Vec3 *const b) {
    raw___he_update_full_trace_info("vec3_dot", ___file___, ___line___);
    float result = raw_vec3_dot(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_cross(const char* ___file___, uint32_t ___line___, const Vec3 *const a, const Vec3 *const b) {
    raw___he_update_full_trace_info("vec3_cross", ___file___, ___line___);
    Vec3 result = raw_vec3_cross(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_vec3_length(const char* ___file___, uint32_t ___line___, const Vec3 *const a) {
    raw___he_update_full_trace_info("vec3_length", ___file___, ___line___);
    float result = raw_vec3_length(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_normalize(const char* ___file___, uint32_t ___line___, const Vec3 *const a) {
    raw___he_update_full_trace_info("vec3_normalize", ___file___, ___line___);
    Vec3 result = raw_vec3_normalize(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
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

inline Vec2 full_trace_vec2_new(const char* ___file___, uint32_t ___line___, float x, float y, float z) {
    raw___he_update_full_trace_info("vec2_new", ___file___, ___line___);
    Vec2 result = raw_vec2_new(x, y, z);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec2 full_trace_vec2_add(const char* ___file___, uint32_t ___line___, const Vec2 *const a, const Vec2 *const b) {
    raw___he_update_full_trace_info("vec2_add", ___file___, ___line___);
    Vec2 result = raw_vec2_add(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec2 full_trace_vec2_sub(const char* ___file___, uint32_t ___line___, const Vec2 *const a, const Vec2 *const b) {
    raw___he_update_full_trace_info("vec2_sub", ___file___, ___line___);
    Vec2 result = raw_vec2_sub(a, b);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec2 full_trace_vec2_scale(const char* ___file___, uint32_t ___line___, const Vec2 *const a, const float factor) {
    raw___he_update_full_trace_info("vec2_scale", ___file___, ___line___);
    Vec2 result = raw_vec2_scale(a, factor);
    raw___he_update_full_trace_info("", "", -1);
    return result;
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

inline Vec2 full_trace_vec2_normalize(const char* ___file___, uint32_t ___line___, const Vec2 *const a) {
    raw___he_update_full_trace_info("vec2_normalize", ___file___, ___line___);
    Vec2 result = raw_vec2_normalize(a);
    raw___he_update_full_trace_info("", "", -1);
    return result;
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

inline UID full_trace_uid_new(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("uid_new", ___file___, ___line___);
    UID result = raw_uid_new();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_mount_res(const char* ___file___, uint32_t ___line___, const char * path, const char * mount_point) {
    raw___he_update_full_trace_info("vfs_mount_res", ___file___, ___line___);
    int result = raw_vfs_mount_res(path, mount_point);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_unmount_res(const char* ___file___, uint32_t ___line___, const char * mount_point) {
    raw___he_update_full_trace_info("vfs_unmount_res", ___file___, ___line___);
    int result = raw_vfs_unmount_res(mount_point);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_mount_rfs(const char* ___file___, uint32_t ___line___, const char * mount_point) {
    raw___he_update_full_trace_info("vfs_mount_rfs", ___file___, ___line___);
    int result = raw_vfs_mount_rfs(mount_point);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_mount_rfs_whitelist(const char* ___file___, uint32_t ___line___, const char ** whitelist, int count, const char * mount_point) {
    raw___he_update_full_trace_info("vfs_mount_rfs_whitelist", ___file___, ___line___);
    int result = raw_vfs_mount_rfs_whitelist(whitelist, count, mount_point);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_unmount_rfs(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("vfs_unmount_rfs", ___file___, ___line___);
    int result = raw_vfs_unmount_rfs();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_res_path_exists(const char* ___file___, uint32_t ___line___, const char * path) {
    raw___he_update_full_trace_info("vfs_res_path_exists", ___file___, ___line___);
    int result = raw_vfs_res_path_exists(path);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_usr_path_exists(const char* ___file___, uint32_t ___line___, const char * path, int prefer_res) {
    raw___he_update_full_trace_info("vfs_usr_path_exists", ___file___, ___line___);
    int result = raw_vfs_usr_path_exists(path, prefer_res);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void * full_trace_vfs_res_read_file(const char* ___file___, uint32_t ___line___, const char * path, int * size) {
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

inline void * full_trace_vfs_usr_read_file(const char* ___file___, uint32_t ___line___, const char * path, int * size, int prefer_res) {
    raw___he_update_full_trace_info("vfs_usr_read_file", ___file___, ___line___);
    void * result = raw_vfs_usr_read_file(path, size, prefer_res);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_usr_write_file(const char* ___file___, uint32_t ___line___, const char * path, const void * data, int size) {
    raw___he_update_full_trace_info("vfs_usr_write_file", ___file___, ___line___);
    int result = raw_vfs_usr_write_file(path, data, size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline FileStream * full_trace_vfs_usr_stream_open(const char* ___file___, uint32_t ___line___, const char * path, int prefer_res) {
    raw___he_update_full_trace_info("vfs_usr_stream_open", ___file___, ___line___);
    FileStream * result = raw_vfs_usr_stream_open(path, prefer_res);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_stream_size(const char* ___file___, uint32_t ___line___, FileStream * stream) {
    raw___he_update_full_trace_info("vfs_stream_size", ___file___, ___line___);
    int result = raw_vfs_stream_size(stream);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_stream_read_n(const char* ___file___, uint32_t ___line___, FileStream * stream, void * buffer, int size) {
    raw___he_update_full_trace_info("vfs_stream_read_n", ___file___, ___line___);
    int result = raw_vfs_stream_read_n(stream, buffer, size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void * full_trace_vfs_stream_read_all(const char* ___file___, uint32_t ___line___, FileStream * stream, int * size) {
    raw___he_update_full_trace_info("vfs_stream_read_all", ___file___, ___line___);
    void * result = raw_vfs_stream_read_all(stream, size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_stream_write(const char* ___file___, uint32_t ___line___, FileStream * stream, void * buffer, int size) {
    raw___he_update_full_trace_info("vfs_stream_write", ___file___, ___line___);
    int result = raw_vfs_stream_write(stream, buffer, size);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_stream_seek(const char* ___file___, uint32_t ___line___, FileStream * stream, FSSeekFrom whence, int offset) {
    raw___he_update_full_trace_info("vfs_stream_seek", ___file___, ___line___);
    int result = raw_vfs_stream_seek(stream, whence, offset);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_stream_tell(const char* ___file___, uint32_t ___line___, FileStream * stream, int * success) {
    raw___he_update_full_trace_info("vfs_stream_tell", ___file___, ___line___);
    int result = raw_vfs_stream_tell(stream, success);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline int full_trace_vfs_stream_flush(const char* ___file___, uint32_t ___line___, FileStream * stream) {
    raw___he_update_full_trace_info("vfs_stream_flush", ___file___, ___line___);
    int result = raw_vfs_stream_flush(stream);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_vfs_stream_close(const char* ___file___, uint32_t ___line___, FileStream * stream) {
    raw___he_update_full_trace_info("vfs_stream_close", ___file___, ___line___);
    raw_vfs_stream_close(stream);
    raw___he_update_full_trace_info("", "", -1);
}

inline Error full_trace_window_server_register_backend(const char* ___file___, uint32_t ___line___, const char * name, WindowServerBackend * backend) {
    raw___he_update_full_trace_info("window_server_register_backend", ___file___, ___line___);
    Error result = raw_window_server_register_backend(name, backend);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_load_backend(const char* ___file___, uint32_t ___line___, const char * name) {
    raw___he_update_full_trace_info("window_server_load_backend", ___file___, ___line___);
    Error result = raw_window_server_load_backend(name);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline WindowServerBackend * full_trace_window_server_backend_new(const char* ___file___, uint32_t ___line___) {
    raw___he_update_full_trace_info("window_server_backend_new", ___file___, ___line___);
    WindowServerBackend * result = raw_window_server_backend_new();
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_backend_set_function(const char* ___file___, uint32_t ___line___, WindowServerBackend * backend, const char * name, void (* function) (void)) {
    raw___he_update_full_trace_info("window_server_backend_set_function", ___file___, ___line___);
    Error result = raw_window_server_backend_set_function(backend, name, function);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_backend_get_function(const char* ___file___, uint32_t ___line___, WindowServerBackend * backend, const char * name, void (** function) (void)) {
    raw___he_update_full_trace_info("window_server_backend_get_function", ___file___, ___line___);
    Error result = raw_window_server_backend_get_function(backend, name, function);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_create_window(const char* ___file___, uint32_t ___line___, const char * title, int w, int h, WindowServerWindow * parent, WindowServerWindow ** out) {
    raw___he_update_full_trace_info("window_server_create_window", ___file___, ___line___);
    Error result = raw_window_server_create_window(title, w, h, parent, out);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_destroy_window(const char* ___file___, uint32_t ___line___, WindowServerWindow * this) {
    raw___he_update_full_trace_info("window_server_destroy_window", ___file___, ___line___);
    Error result = raw_window_server_destroy_window(this);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_window_set_title(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, const char * title) {
    raw___he_update_full_trace_info("window_server_window_set_title", ___file___, ___line___);
    Error result = raw_window_server_window_set_title(this, title);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_window_get_title(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, const char ** out) {
    raw___he_update_full_trace_info("window_server_window_get_title", ___file___, ___line___);
    Error result = raw_window_server_window_get_title(this, out);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_window_set_mode(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, WindowServerWindowMode mode) {
    raw___he_update_full_trace_info("window_server_window_set_mode", ___file___, ___line___);
    Error result = raw_window_server_window_set_mode(this, mode);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_window_get_mode(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, WindowServerWindowMode * out) {
    raw___he_update_full_trace_info("window_server_window_get_mode", ___file___, ___line___);
    Error result = raw_window_server_window_get_mode(this, out);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_window_set_size(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, int w, int h) {
    raw___he_update_full_trace_info("window_server_window_set_size", ___file___, ___line___);
    Error result = raw_window_server_window_set_size(this, w, h);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_window_get_size(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, int * w, int * h) {
    raw___he_update_full_trace_info("window_server_window_get_size", ___file___, ___line___);
    Error result = raw_window_server_window_get_size(this, w, h);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_window_set_position(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, int x, int y) {
    raw___he_update_full_trace_info("window_server_window_set_position", ___file___, ___line___);
    Error result = raw_window_server_window_set_position(this, x, y);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_window_get_position(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, int * x, int * y) {
    raw___he_update_full_trace_info("window_server_window_get_position", ___file___, ___line___);
    Error result = raw_window_server_window_get_position(this, x, y);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}

inline Error full_trace_window_server_window_set_fullscreen_display(const char* ___file___, uint32_t ___line___, WindowServerWindow * this, WindowServerDisplay * fullscreen) {
    raw___he_update_full_trace_info("window_server_window_set_fullscreen_display", ___file___, ___line___);
    Error result = raw_window_server_window_set_fullscreen_display(this, fullscreen);
    raw___he_update_full_trace_info("", "", -1);
    return result;
}


#endif

#define tmalloc(size) full_trace_tmalloc(__FILE__, __LINE__, size)
#define trealloc(ptr, size) full_trace_trealloc(__FILE__, __LINE__, ptr, size)
#define tfree(ptr) full_trace_tfree(__FILE__, __LINE__, ptr)
#define get_allocated_memory() full_trace_get_allocated_memory(__FILE__, __LINE__)
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
#define mat4_new(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) full_trace_mat4_new(__FILE__, __LINE__, m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)
#define mat4_new_zero() full_trace_mat4_new_zero(__FILE__, __LINE__)
#define mat4_new_one() full_trace_mat4_new_one(__FILE__, __LINE__)
#define mat4_from_array(array) full_trace_mat4_from_array(__FILE__, __LINE__, array)
#define mat4_from_mat4(a) full_trace_mat4_from_mat4(__FILE__, __LINE__, a)
#define mat4_add(a, b) full_trace_mat4_add(__FILE__, __LINE__, a, b)
#define mat4_sub(a, b) full_trace_mat4_sub(__FILE__, __LINE__, a, b)
#define mat4_mul(a, b) full_trace_mat4_mul(__FILE__, __LINE__, a, b)
#define mat4_scale(a, factor) full_trace_mat4_scale(__FILE__, __LINE__, a, factor)
#define mat4_transpose(a) full_trace_mat4_transpose(__FILE__, __LINE__, a)
#define mat4_inverse(a) full_trace_mat4_inverse(__FILE__, __LINE__, a)
#define vec3_new(x, y, z) full_trace_vec3_new(__FILE__, __LINE__, x, y, z)
#define vec3_add(a, b) full_trace_vec3_add(__FILE__, __LINE__, a, b)
#define vec3_sub(a, b) full_trace_vec3_sub(__FILE__, __LINE__, a, b)
#define vec3_scale(a, factor) full_trace_vec3_scale(__FILE__, __LINE__, a, factor)
#define vec3_dot(a, b) full_trace_vec3_dot(__FILE__, __LINE__, a, b)
#define vec3_cross(a, b) full_trace_vec3_cross(__FILE__, __LINE__, a, b)
#define vec3_length(a) full_trace_vec3_length(__FILE__, __LINE__, a)
#define vec3_normalize(a) full_trace_vec3_normalize(__FILE__, __LINE__, a)
#define vec3_distance(a, b) full_trace_vec3_distance(__FILE__, __LINE__, a, b)
#define vec3_add_in(to, what) full_trace_vec3_add_in(__FILE__, __LINE__, to, what)
#define vec3_sub_in(from, what) full_trace_vec3_sub_in(__FILE__, __LINE__, from, what)
#define vec3_scale_in(to, factor) full_trace_vec3_scale_in(__FILE__, __LINE__, to, factor)
#define vec3_normalize_in(a) full_trace_vec3_normalize_in(__FILE__, __LINE__, a)
#define vec2_new(x, y, z) full_trace_vec2_new(__FILE__, __LINE__, x, y, z)
#define vec2_add(a, b) full_trace_vec2_add(__FILE__, __LINE__, a, b)
#define vec2_sub(a, b) full_trace_vec2_sub(__FILE__, __LINE__, a, b)
#define vec2_scale(a, factor) full_trace_vec2_scale(__FILE__, __LINE__, a, factor)
#define vec2_dot(a, b) full_trace_vec2_dot(__FILE__, __LINE__, a, b)
#define vec2_length(a) full_trace_vec2_length(__FILE__, __LINE__, a)
#define vec2_normalize(a) full_trace_vec2_normalize(__FILE__, __LINE__, a)
#define vec2_distance(a, b) full_trace_vec2_distance(__FILE__, __LINE__, a, b)
#define vec2_add_in(to, what) full_trace_vec2_add_in(__FILE__, __LINE__, to, what)
#define vec2_sub_in(from, what) full_trace_vec2_sub_in(__FILE__, __LINE__, from, what)
#define vec2_scale_in(to, factor) full_trace_vec2_scale_in(__FILE__, __LINE__, to, factor)
#define vec2_normalize_in(a) full_trace_vec2_normalize_in(__FILE__, __LINE__, a)
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
#define window_server_backend_set_function(backend, name, function) full_trace_window_server_backend_set_function(__FILE__, __LINE__, backend, name, function)
#define window_server_backend_get_function(backend, name, function) full_trace_window_server_backend_get_function(__FILE__, __LINE__, backend, name, function)
#define window_server_create_window(title, w, h, parent, out) full_trace_window_server_create_window(__FILE__, __LINE__, title, w, h, parent, out)
#define window_server_destroy_window(this) full_trace_window_server_destroy_window(__FILE__, __LINE__, this)
#define window_server_window_set_title(this, title) full_trace_window_server_window_set_title(__FILE__, __LINE__, this, title)
#define window_server_window_get_title(this, out) full_trace_window_server_window_get_title(__FILE__, __LINE__, this, out)
#define window_server_window_set_mode(this, mode) full_trace_window_server_window_set_mode(__FILE__, __LINE__, this, mode)
#define window_server_window_get_mode(this, out) full_trace_window_server_window_get_mode(__FILE__, __LINE__, this, out)
#define window_server_window_set_size(this, w, h) full_trace_window_server_window_set_size(__FILE__, __LINE__, this, w, h)
#define window_server_window_get_size(this, w, h) full_trace_window_server_window_get_size(__FILE__, __LINE__, this, w, h)
#define window_server_window_set_position(this, x, y) full_trace_window_server_window_set_position(__FILE__, __LINE__, this, x, y)
#define window_server_window_get_position(this, x, y) full_trace_window_server_window_get_position(__FILE__, __LINE__, this, x, y)
#define window_server_window_set_fullscreen_display(this, fullscreen) full_trace_window_server_window_set_fullscreen_display(__FILE__, __LINE__, this, fullscreen)

#endif

// clang-format on

