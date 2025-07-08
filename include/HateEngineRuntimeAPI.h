
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

enum FSSeekFrom { FSSeekFromStart, FSSeekFromCurrent, FSSeekFromEnd };

typedef struct Object Object;

typedef struct Node Node;

typedef struct Node3D Node3D;

/**
 * @brief Primitive 4x4 matrix
 * Elements - el.m<row><column>
 * Raw data - data
 */
typedef union {
    struct {
        float m00;
        float m01;
        float m02;
        float m03;
        float m10;
        float m11;
        float m12;
        float m13;
        float m20;
        float m21;
        float m22;
        float m23;
        float m30;
        float m31;
        float m32;
        float m33;
    } el;
    float data[16];
} Mat4;

/**
 * @brief Primitive 3D vector
 *
 */
typedef struct {
    float x;
    float y;
    float z;
} Vec3;

typedef size_t UID;

typedef struct FileStream FileStream;



/**
 * @brief Malloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param size
 * @return void*
 */
extern void* (*tmalloc)(size_t size);

/**
 * @brief Realloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 * @param size
 * @return void*
 */
extern void* (*trealloc)(void* ptr, size_t size);

/**
 * @brief Free with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 */
extern void (*tfree)(void* ptr);

/**
 * @brief Get allocated memory
 *
 * @return size in bytes if compiled with HE_MEM_TRACK, 0 otherwise
 */
extern size_t (*get_allocated_memory)(void);

/**
 * @brief Free object by type
 *
 * @param object
 */
extern void (*auto_free)(Object* object);

extern Node* (*node_new)(const char* name);

extern Node* (*from_node)(Node* node);

extern void (*node_set_name)(Node* node, const char* name);

extern const char* (*node_get_name)(Node* node);

/**
 * @brief Add a child node to a parent node
 * @warning Take ownership of child. Do not free it!
 * @param node
 * @param child
 */
extern void (*node_add_child)(Node* node, Node* child);

/**
 * @brief Remove and free a child node by pointer from a parent node
 *
 * @param node
 * @param child
 * @return true if child was removed
 * @return false if child was not found
 */
extern bool (*node_remove_child)(Node* node, Node* child);

/**
 * @brief Remove and free a child node by name from a parent node
 *
 * @param node
 * @param name
 * @return true if child was removed
 * @return false if child was not found
 */
extern bool (*node_remove_child_by_name)(Node* node, const char* name);

/**
 * @brief Remove and free a child node by uid from a parent node
 *
 * @param node
 * @param uid
 * @return true if child was removed
 * @return false if child was not found
 */
extern bool (*node_remove_child_by_uid)(Node* node, UID uid);

extern bool (*node_remove_all_children)(Node* node);

/**
 * @brief Get a child node by name
 *
 * @param node
 * @param name
 * @return Pointer to found child, NULL if not found
 */
extern Node* (*node_get_child_by_name)(Node* node, const char* name);

extern Node* (*node_get_child_by_uid)(Node* node, UID uid);

extern Node3D* (*node3d_new)(void);

extern Mat4 (*mat4_new)(
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
);

extern Mat4 (*mat4_new_zero)(void);

extern Mat4 (*mat4_new_one)(void);

extern Mat4 (*mat4_from_array)(const float* const array);

extern Mat4 (*mat4_from_mat4)(const Mat4* const a);

extern Mat4 (*mat4_add)(const Mat4* const a, const Mat4* const b);

extern Mat4 (*mat4_sub)(const Mat4* const a, const Mat4* const b);

extern Mat4 (*mat4_mul)(const Mat4* const a, const Mat4* const b);

extern Mat4 (*mat4_scale)(const Mat4* const a, const float factor);

extern Mat4 (*mat4_transpose)(const Mat4* const a);

extern Mat4 (*mat4_inverse)(const Mat4* const a);

extern Vec3 (*vec3_new)(float x, float y, float z);

extern Vec3 (*vec3_add)(const Vec3* const a, const Vec3* const b);

extern Vec3 (*vec3_sub)(const Vec3* const a, const Vec3* const b);

extern Vec3 (*vec3_scale)(const Vec3* const a, const float factor);

extern float (*vec3_dot)(const Vec3* const a, const Vec3* const b);

extern Vec3 (*vec3_cross)(const Vec3* const a, const Vec3* const b);

extern float (*vec3_length)(const Vec3* const a);

extern Vec3 (*vec3_normalize)(const Vec3* const a);

extern float (*vec3_distance)(const Vec3* const a, const Vec3* const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 */
extern void (*vec3_add_in)(Vec3* const to, const Vec3* const what);

extern void (*vec3_sub_in)(Vec3* const from, const Vec3* const what);

extern void (*vec3_scale_in)(Vec3* const to, const float factor);

extern void (*vec3_normalize_in)(Vec3* const a);

extern UID (*uid_new)(void);

extern bool (*vfs_mount_res)(const char* path, const char* mount_point);

extern bool (*vfs_unmount_res)(const char* path, const char* mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res
 *
 * @return true if mounted successfully
 * @return false if mount failed
 */
extern bool (*vfs_mount_rfs)(const char* mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res only with allowed files in whitelist
 *
 * @return true if mounted successfully
 * @return false if mount failed
 */
extern bool (*vfs_mount_rfs_whitelist)(const char** whitelist, size_t count, const char* mount_point);

/**
 * @brief Unmount Real File System from Virtual File System Res
 *
 * @return true if unmounted successfully
 * @return false if unmount failed
 */
extern bool (*vfs_unmount_rfs)(void);

/**
 * @brief Check if file exists
 *
 * @param path
 * @return true if exists
 * @return false if not exists
 */
extern bool (*vfs_res_path_exists)(const char* path);

/**
 * @brief Check if file exists in User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return true if exists
 * @return false if not exists
 */
extern bool (*vfs_usr_path_exists)(const char* path, bool prefer_res);

/**
 * @brief Allocate memory and read file from Reosource scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @return Pointer to allocated memory with file data if success, else NULL
 */
extern void* (*vfs_res_read_file)(const char* path, size_t* size);

/**
 * @brief Open file stream from Resource scope
 *
 * @param path
 * @return FileStream* If success, else NULL
 */
extern FileStream* (*vfs_res_stream_open)(const char* path);

/**
 * @brief Allocate memory and read file from User scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @param prefer_res If true, try to read from Resource scope first
 * @return Pointer to allocated memory with file data if success, else NULL
 */
extern void* (*vfs_usr_read_file)(const char* path, size_t* size, bool prefer_res);

/**
 * @brief Write file to User scope
 *
 * @param path
 * @param data
 * @param size
 * @return true if success
 * @return false if fail
 */
extern bool (*vfs_usr_write_file)(const char* path, const void* data, size_t size);

/**
 * @brief Open file stream from User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return FileStream* If success, else NULL
 */
extern FileStream* (*vfs_usr_stream_open)(const char* path, bool prefer_res);

/**
 * @brief Get file stream size in bytes
 *
 * @param path
 * @return size_t
 */
extern size_t (*vfs_stream_size)(FileStream* stream);

/**
 * @brief Read from file stream n bytes to buffer
 *
 * @param stream
 * @param buffer
 * @param size
 * @return size_t
 */
extern size_t (*vfs_stream_read_n)(FileStream* stream, void* buffer, size_t size);

/**
 * @brief Allocate buffer and read all file to it
 *
 * @warning You must free returned buffer
 * @param stream
 * @param size
 * @return void*
 */
extern void* (*vfs_stream_read_all)(FileStream* stream, size_t* size);

/**
 * @brief Write to file stream n bytes from buffer
 * @warning Only for 'res' scope
 *
 * @param stream
 * @param buffer
 * @param size
 *
 * @return size_t Number of bytes written. 0 if fail
 */
extern size_t (*vfs_stream_write)(FileStream* stream, void* buffer, size_t size);

/**
 * @brief Seek file stream
 *
 * @param stream
 * @param whence
 * @param offset
 * @return true if success
 * @return false if fail
 */
extern bool (*vfs_stream_seek)(FileStream* stream, enum FSSeekFrom whence, size_t offset);

/**
 * @brief Get file stream current position
 *
 * @param stream
 * @param success Can be NULL
 * @return size_t
 */
extern size_t (*vfs_stream_tell)(FileStream* stream, bool* success);

/**
 * @brief Flush file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 */
extern bool (*vfs_stream_flush)(FileStream* stream);

/**
 * @brief Close file stream
 *
 * @param stream
 */
extern void (*vfs_stream_close)(FileStream* stream);



#if defined(HERAPI_LOAD_IMPL)
        void* (*tmalloc)(size_t size);
    void* (*trealloc)(void* ptr, size_t size);
    void (*tfree)(void* ptr);
    size_t (*get_allocated_memory)(void);
    void (*auto_free)(Object* object);
    Node* (*node_new)(const char* name);
    Node* (*from_node)(Node* node);
    void (*node_set_name)(Node* node, const char* name);
    const char* (*node_get_name)(Node* node);
    void (*node_add_child)(Node* node, Node* child);
    bool (*node_remove_child)(Node* node, Node* child);
    bool (*node_remove_child_by_name)(Node* node, const char* name);
    bool (*node_remove_child_by_uid)(Node* node, UID uid);
    bool (*node_remove_all_children)(Node* node);
    Node* (*node_get_child_by_name)(Node* node, const char* name);
    Node* (*node_get_child_by_uid)(Node* node, UID uid);
    Node3D* (*node3d_new)(void);
    Mat4 (*mat4_new)(
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
);
    Mat4 (*mat4_new_zero)(void);
    Mat4 (*mat4_new_one)(void);
    Mat4 (*mat4_from_array)(const float* const array);
    Mat4 (*mat4_from_mat4)(const Mat4* const a);
    Mat4 (*mat4_add)(const Mat4* const a, const Mat4* const b);
    Mat4 (*mat4_sub)(const Mat4* const a, const Mat4* const b);
    Mat4 (*mat4_mul)(const Mat4* const a, const Mat4* const b);
    Mat4 (*mat4_scale)(const Mat4* const a, const float factor);
    Mat4 (*mat4_transpose)(const Mat4* const a);
    Mat4 (*mat4_inverse)(const Mat4* const a);
    Vec3 (*vec3_new)(float x, float y, float z);
    Vec3 (*vec3_add)(const Vec3* const a, const Vec3* const b);
    Vec3 (*vec3_sub)(const Vec3* const a, const Vec3* const b);
    Vec3 (*vec3_scale)(const Vec3* const a, const float factor);
    float (*vec3_dot)(const Vec3* const a, const Vec3* const b);
    Vec3 (*vec3_cross)(const Vec3* const a, const Vec3* const b);
    float (*vec3_length)(const Vec3* const a);
    Vec3 (*vec3_normalize)(const Vec3* const a);
    float (*vec3_distance)(const Vec3* const a, const Vec3* const b);
    void (*vec3_add_in)(Vec3* const to, const Vec3* const what);
    void (*vec3_sub_in)(Vec3* const from, const Vec3* const what);
    void (*vec3_scale_in)(Vec3* const to, const float factor);
    void (*vec3_normalize_in)(Vec3* const a);
    UID (*uid_new)(void);
    bool (*vfs_mount_res)(const char* path, const char* mount_point);
    bool (*vfs_unmount_res)(const char* path, const char* mount_point);
    bool (*vfs_mount_rfs)(const char* mount_point);
    bool (*vfs_mount_rfs_whitelist)(const char** whitelist, size_t count, const char* mount_point);
    bool (*vfs_unmount_rfs)(void);
    bool (*vfs_res_path_exists)(const char* path);
    bool (*vfs_usr_path_exists)(const char* path, bool prefer_res);
    void* (*vfs_res_read_file)(const char* path, size_t* size);
    FileStream* (*vfs_res_stream_open)(const char* path);
    void* (*vfs_usr_read_file)(const char* path, size_t* size, bool prefer_res);
    bool (*vfs_usr_write_file)(const char* path, const void* data, size_t size);
    FileStream* (*vfs_usr_stream_open)(const char* path, bool prefer_res);
    size_t (*vfs_stream_size)(FileStream* stream);
    size_t (*vfs_stream_read_n)(FileStream* stream, void* buffer, size_t size);
    void* (*vfs_stream_read_all)(FileStream* stream, size_t* size);
    size_t (*vfs_stream_write)(FileStream* stream, void* buffer, size_t size);
    bool (*vfs_stream_seek)(FileStream* stream, enum FSSeekFrom whence, size_t offset);
    size_t (*vfs_stream_tell)(FileStream* stream, bool* success);
    bool (*vfs_stream_flush)(FileStream* stream);
    void (*vfs_stream_close)(FileStream* stream);


    void ___hate_engine_runtime_init(void* (*proc_addr)(const char* name)) {
            tmalloc = (void* (*)(size_t size))proc_addr("tmalloc");
    trealloc = (void* (*)(void* ptr, size_t size))proc_addr("trealloc");
    tfree = (void (*)(void* ptr))proc_addr("tfree");
    get_allocated_memory = (size_t (*)(void))proc_addr("get_allocated_memory");
    auto_free = (void (*)(Object* object))proc_addr("auto_free");
    node_new = (Node* (*)(const char* name))proc_addr("node_new");
    from_node = (Node* (*)(Node* node))proc_addr("from_node");
    node_set_name = (void (*)(Node* node, const char* name))proc_addr("node_set_name");
    node_get_name = (const char* (*)(Node* node))proc_addr("node_get_name");
    node_add_child = (void (*)(Node* node, Node* child))proc_addr("node_add_child");
    node_remove_child = (bool (*)(Node* node, Node* child))proc_addr("node_remove_child");
    node_remove_child_by_name = (bool (*)(Node* node, const char* name))proc_addr("node_remove_child_by_name");
    node_remove_child_by_uid = (bool (*)(Node* node, UID uid))proc_addr("node_remove_child_by_uid");
    node_remove_all_children = (bool (*)(Node* node))proc_addr("node_remove_all_children");
    node_get_child_by_name = (Node* (*)(Node* node, const char* name))proc_addr("node_get_child_by_name");
    node_get_child_by_uid = (Node* (*)(Node* node, UID uid))proc_addr("node_get_child_by_uid");
    node3d_new = (Node3D* (*)(void))proc_addr("node3d_new");
    mat4_new = (Mat4 (*)(
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
))proc_addr("mat4_new");
    mat4_new_zero = (Mat4 (*)(void))proc_addr("mat4_new_zero");
    mat4_new_one = (Mat4 (*)(void))proc_addr("mat4_new_one");
    mat4_from_array = (Mat4 (*)(const float* const array))proc_addr("mat4_from_array");
    mat4_from_mat4 = (Mat4 (*)(const Mat4* const a))proc_addr("mat4_from_mat4");
    mat4_add = (Mat4 (*)(const Mat4* const a, const Mat4* const b))proc_addr("mat4_add");
    mat4_sub = (Mat4 (*)(const Mat4* const a, const Mat4* const b))proc_addr("mat4_sub");
    mat4_mul = (Mat4 (*)(const Mat4* const a, const Mat4* const b))proc_addr("mat4_mul");
    mat4_scale = (Mat4 (*)(const Mat4* const a, const float factor))proc_addr("mat4_scale");
    mat4_transpose = (Mat4 (*)(const Mat4* const a))proc_addr("mat4_transpose");
    mat4_inverse = (Mat4 (*)(const Mat4* const a))proc_addr("mat4_inverse");
    vec3_new = (Vec3 (*)(float x, float y, float z))proc_addr("vec3_new");
    vec3_add = (Vec3 (*)(const Vec3* const a, const Vec3* const b))proc_addr("vec3_add");
    vec3_sub = (Vec3 (*)(const Vec3* const a, const Vec3* const b))proc_addr("vec3_sub");
    vec3_scale = (Vec3 (*)(const Vec3* const a, const float factor))proc_addr("vec3_scale");
    vec3_dot = (float (*)(const Vec3* const a, const Vec3* const b))proc_addr("vec3_dot");
    vec3_cross = (Vec3 (*)(const Vec3* const a, const Vec3* const b))proc_addr("vec3_cross");
    vec3_length = (float (*)(const Vec3* const a))proc_addr("vec3_length");
    vec3_normalize = (Vec3 (*)(const Vec3* const a))proc_addr("vec3_normalize");
    vec3_distance = (float (*)(const Vec3* const a, const Vec3* const b))proc_addr("vec3_distance");
    vec3_add_in = (void (*)(Vec3* const to, const Vec3* const what))proc_addr("vec3_add_in");
    vec3_sub_in = (void (*)(Vec3* const from, const Vec3* const what))proc_addr("vec3_sub_in");
    vec3_scale_in = (void (*)(Vec3* const to, const float factor))proc_addr("vec3_scale_in");
    vec3_normalize_in = (void (*)(Vec3* const a))proc_addr("vec3_normalize_in");
    uid_new = (UID (*)(void))proc_addr("uid_new");
    vfs_mount_res = (bool (*)(const char* path, const char* mount_point))proc_addr("vfs_mount_res");
    vfs_unmount_res = (bool (*)(const char* path, const char* mount_point))proc_addr("vfs_unmount_res");
    vfs_mount_rfs = (bool (*)(const char* mount_point))proc_addr("vfs_mount_rfs");
    vfs_mount_rfs_whitelist = (bool (*)(const char** whitelist, size_t count, const char* mount_point))proc_addr("vfs_mount_rfs_whitelist");
    vfs_unmount_rfs = (bool (*)(void))proc_addr("vfs_unmount_rfs");
    vfs_res_path_exists = (bool (*)(const char* path))proc_addr("vfs_res_path_exists");
    vfs_usr_path_exists = (bool (*)(const char* path, bool prefer_res))proc_addr("vfs_usr_path_exists");
    vfs_res_read_file = (void* (*)(const char* path, size_t* size))proc_addr("vfs_res_read_file");
    vfs_res_stream_open = (FileStream* (*)(const char* path))proc_addr("vfs_res_stream_open");
    vfs_usr_read_file = (void* (*)(const char* path, size_t* size, bool prefer_res))proc_addr("vfs_usr_read_file");
    vfs_usr_write_file = (bool (*)(const char* path, const void* data, size_t size))proc_addr("vfs_usr_write_file");
    vfs_usr_stream_open = (FileStream* (*)(const char* path, bool prefer_res))proc_addr("vfs_usr_stream_open");
    vfs_stream_size = (size_t (*)(FileStream* stream))proc_addr("vfs_stream_size");
    vfs_stream_read_n = (size_t (*)(FileStream* stream, void* buffer, size_t size))proc_addr("vfs_stream_read_n");
    vfs_stream_read_all = (void* (*)(FileStream* stream, size_t* size))proc_addr("vfs_stream_read_all");
    vfs_stream_write = (size_t (*)(FileStream* stream, void* buffer, size_t size))proc_addr("vfs_stream_write");
    vfs_stream_seek = (bool (*)(FileStream* stream, enum FSSeekFrom whence, size_t offset))proc_addr("vfs_stream_seek");
    vfs_stream_tell = (size_t (*)(FileStream* stream, bool* success))proc_addr("vfs_stream_tell");
    vfs_stream_flush = (bool (*)(FileStream* stream))proc_addr("vfs_stream_flush");
    vfs_stream_close = (void (*)(FileStream* stream))proc_addr("vfs_stream_close");

    }
#endif
