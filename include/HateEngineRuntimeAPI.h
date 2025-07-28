
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct FileStream FileStream;

typedef uint64_t UID;

enum FSSeekFrom { FSSeekFromStart, FSSeekFromCurrent, FSSeekFromEnd };

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

typedef struct Object Object;

typedef struct Node Node;

typedef struct Node3D Node3D;



#if defined(HEAPI_COMPILE_TIME)

 bool vfs_mount_res (const char* path, const char* mount_point);

 bool vfs_unmount_res (const char* path, const char* mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res
 *
 * @return true if mounted successfully
 * @return false if mount failed
 */
 bool vfs_mount_rfs (const char* mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res only with allowed files in whitelist
 *
 * @return true if mounted successfully
 * @return false if mount failed
 */
 bool vfs_mount_rfs_whitelist (const char** whitelist, size_t count, const char* mount_point);

/**
 * @brief Unmount Real File System from Virtual File System Res
 *
 * @return true if unmounted successfully
 * @return false if unmount failed
 */
 bool vfs_unmount_rfs (void);

/**
 * @brief Check if file exists
 *
 * @param path
 * @return true if exists
 * @return false if not exists
 */
 bool vfs_res_path_exists (const char* path);

/**
 * @brief Check if file exists in User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return true if exists
 * @return false if not exists
 */
 bool vfs_usr_path_exists (const char* path, bool prefer_res);

/**
 * @brief Allocate memory and read file from Reosource scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @return Pointer to allocated memory with file data if success, else NULL
 */
 void* vfs_res_read_file (const char* path, size_t* size);

/**
 * @brief Open file stream from Resource scope
 *
 * @param path
 * @return FileStream* If success, else NULL
 */
 FileStream* vfs_res_stream_open (const char* path);

/**
 * @brief Allocate memory and read file from User scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @param prefer_res If true, try to read from Resource scope first
 * @return Pointer to allocated memory with file data if success, else NULL
 */
 void* vfs_usr_read_file (const char* path, size_t* size, bool prefer_res);

/**
 * @brief Write file to User scope
 *
 * @param path
 * @param data
 * @param size
 * @return true if success
 * @return false if fail
 */
 bool vfs_usr_write_file (const char* path, const void* data, size_t size);

/**
 * @brief Open file stream from User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return FileStream* If success, else NULL
 */
 FileStream* vfs_usr_stream_open (const char* path, bool prefer_res);

/**
 * @brief Get file stream size in bytes
 *
 * @param path
 * @return size_t
 */
 size_t vfs_stream_size (FileStream* stream);

/**
 * @brief Read from file stream n bytes to buffer
 *
 * @param stream
 * @param buffer
 * @param size
 * @return size_t
 */
 size_t vfs_stream_read_n (FileStream* stream, void* buffer, size_t size);

/**
 * @brief Allocate buffer and read all file to it
 *
 * @warning You must free returned buffer
 * @param stream
 * @param size
 * @return void*
 */
 void* vfs_stream_read_all (FileStream* stream, size_t* size);

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
 size_t vfs_stream_write (FileStream* stream, void* buffer, size_t size);

/**
 * @brief Seek file stream
 *
 * @param stream
 * @param whence
 * @param offset
 * @return true if success
 * @return false if fail
 */
 bool vfs_stream_seek (FileStream* stream, enum FSSeekFrom whence, size_t offset);

/**
 * @brief Get file stream current position
 *
 * @param stream
 * @param success Can be NULL
 * @return size_t
 */
 size_t vfs_stream_tell (FileStream* stream, bool* success);

/**
 * @brief Flush file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 */
 bool vfs_stream_flush (FileStream* stream);

/**
 * @brief Close file stream
 *
 * @param stream
 */
 void vfs_stream_close (FileStream* stream);

 UID uid_new (void);

/**
 * @brief Malloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param size
 * @return void*
 */
 void* tmalloc (size_t size);

/**
 * @brief Realloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 * @param size
 * @return void*
 */
 void* trealloc (void* ptr, size_t size);

/**
 * @brief Free with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 */
 void tfree (void* ptr);

/**
 * @brief Get allocated memory
 *
 * @return size in bytes if compiled with HE_MEM_TRACK, 0 otherwise
 */
 size_t get_allocated_memory (void);

 Mat4 mat4_new (
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
);

 Mat4 mat4_new_zero (void);

 Mat4 mat4_new_one (void);

 Mat4 mat4_from_array (const float* const array);

 Mat4 mat4_from_mat4 (const Mat4* const a);

 Mat4 mat4_add (const Mat4* const a, const Mat4* const b);

 Mat4 mat4_sub (const Mat4* const a, const Mat4* const b);

 Mat4 mat4_mul (const Mat4* const a, const Mat4* const b);

 Mat4 mat4_scale (const Mat4* const a, const float factor);

 Mat4 mat4_transpose (const Mat4* const a);

 Mat4 mat4_inverse (const Mat4* const a);

 Vec3 vec3_new (float x, float y, float z);

 Vec3 vec3_add (const Vec3* const a, const Vec3* const b);

 Vec3 vec3_sub (const Vec3* const a, const Vec3* const b);

 Vec3 vec3_scale (const Vec3* const a, const float factor);

 float vec3_dot (const Vec3* const a, const Vec3* const b);

 Vec3 vec3_cross (const Vec3* const a, const Vec3* const b);

 float vec3_length (const Vec3* const a);

 Vec3 vec3_normalize (const Vec3* const a);

 float vec3_distance (const Vec3* const a, const Vec3* const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 */
 void vec3_add_in (Vec3* const to, const Vec3* const what);

 void vec3_sub_in (Vec3* const from, const Vec3* const what);

 void vec3_scale_in (Vec3* const to, const float factor);

 void vec3_normalize_in (Vec3* const a);

/**
 * @brief Free object by type
 *
 * @param object
 */
 void auto_free (Object* object);

 Node* node_new (const char* name);

 Node* from_node (Node* node);

 void node_set_name (Node* node, const char* name);

 const char* node_get_name (Node* node);

/**
 * @brief Add a child node to a parent node
 * @warning Take ownership of child. Do not free it!
 * @param node
 * @param child
 */
 void node_add_child (Node* node, Node* child);

/**
 * @brief Remove and free a child node by pointer from a parent node
 *
 * @param node
 * @param child
 * @return true if child was removed
 * @return false if child was not found
 */
 bool node_remove_child (Node* node, Node* child);

/**
 * @brief Remove and free a child node by name from a parent node
 *
 * @param node
 * @param name
 * @return true if child was removed
 * @return false if child was not found
 */
 bool node_remove_child_by_name (Node* node, const char* name);

/**
 * @brief Remove and free a child node by uid from a parent node
 *
 * @param node
 * @param uid
 * @return true if child was removed
 * @return false if child was not found
 */
 bool node_remove_child_by_uid (Node* node, UID uid);

 bool node_remove_all_children (Node* node);

/**
 * @brief Get a child node by name
 *
 * @param node
 * @param name
 * @return Pointer to found child, NULL if not found
 */
 Node* node_get_child_by_name (Node* node, const char* name);

 Node* node_get_child_by_uid (Node* node, UID uid);

 Node3D* node3d_new (void);



#else

extern bool (*raw_vfs_mount_res)(const char* path, const char* mount_point);

extern bool (*trace_vfs_mount_res)(const char *___file___, int ___line___, const char* path, const char* mount_point);

extern bool (*raw_vfs_unmount_res)(const char* path, const char* mount_point);

extern bool (*trace_vfs_unmount_res)(const char *___file___, int ___line___, const char* path, const char* mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res
 *
 * @return true if mounted successfully
 * @return false if mount failed
 */
extern bool (*raw_vfs_mount_rfs)(const char* mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res
 *
 * @return true if mounted successfully
 * @return false if mount failed
 */
extern bool (*trace_vfs_mount_rfs)(const char *___file___, int ___line___, const char* mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res only with allowed files in whitelist
 *
 * @return true if mounted successfully
 * @return false if mount failed
 */
extern bool (*raw_vfs_mount_rfs_whitelist)(const char** whitelist, size_t count, const char* mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res only with allowed files in whitelist
 *
 * @return true if mounted successfully
 * @return false if mount failed
 */
extern bool (*trace_vfs_mount_rfs_whitelist)(const char *___file___, int ___line___, const char** whitelist, size_t count, const char* mount_point);

/**
 * @brief Unmount Real File System from Virtual File System Res
 *
 * @return true if unmounted successfully
 * @return false if unmount failed
 */
extern bool (*raw_vfs_unmount_rfs)(void);

/**
 * @brief Unmount Real File System from Virtual File System Res
 *
 * @return true if unmounted successfully
 * @return false if unmount failed
 */
extern bool (*trace_vfs_unmount_rfs)(const char *___file___, int ___line___);

/**
 * @brief Check if file exists
 *
 * @param path
 * @return true if exists
 * @return false if not exists
 */
extern bool (*raw_vfs_res_path_exists)(const char* path);

/**
 * @brief Check if file exists
 *
 * @param path
 * @return true if exists
 * @return false if not exists
 */
extern bool (*trace_vfs_res_path_exists)(const char *___file___, int ___line___, const char* path);

/**
 * @brief Check if file exists in User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return true if exists
 * @return false if not exists
 */
extern bool (*raw_vfs_usr_path_exists)(const char* path, bool prefer_res);

/**
 * @brief Check if file exists in User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return true if exists
 * @return false if not exists
 */
extern bool (*trace_vfs_usr_path_exists)(const char *___file___, int ___line___, const char* path, bool prefer_res);

/**
 * @brief Allocate memory and read file from Reosource scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @return Pointer to allocated memory with file data if success, else NULL
 */
extern void* (*raw_vfs_res_read_file)(const char* path, size_t* size);

/**
 * @brief Allocate memory and read file from Reosource scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @return Pointer to allocated memory with file data if success, else NULL
 */
extern void* (*trace_vfs_res_read_file)(const char *___file___, int ___line___, const char* path, size_t* size);

/**
 * @brief Open file stream from Resource scope
 *
 * @param path
 * @return FileStream* If success, else NULL
 */
extern FileStream* (*raw_vfs_res_stream_open)(const char* path);

/**
 * @brief Open file stream from Resource scope
 *
 * @param path
 * @return FileStream* If success, else NULL
 */
extern FileStream* (*trace_vfs_res_stream_open)(const char *___file___, int ___line___, const char* path);

/**
 * @brief Allocate memory and read file from User scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @param prefer_res If true, try to read from Resource scope first
 * @return Pointer to allocated memory with file data if success, else NULL
 */
extern void* (*raw_vfs_usr_read_file)(const char* path, size_t* size, bool prefer_res);

/**
 * @brief Allocate memory and read file from User scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @param prefer_res If true, try to read from Resource scope first
 * @return Pointer to allocated memory with file data if success, else NULL
 */
extern void* (*trace_vfs_usr_read_file)(const char *___file___, int ___line___, const char* path, size_t* size, bool prefer_res);

/**
 * @brief Write file to User scope
 *
 * @param path
 * @param data
 * @param size
 * @return true if success
 * @return false if fail
 */
extern bool (*raw_vfs_usr_write_file)(const char* path, const void* data, size_t size);

/**
 * @brief Write file to User scope
 *
 * @param path
 * @param data
 * @param size
 * @return true if success
 * @return false if fail
 */
extern bool (*trace_vfs_usr_write_file)(const char *___file___, int ___line___, const char* path, const void* data, size_t size);

/**
 * @brief Open file stream from User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return FileStream* If success, else NULL
 */
extern FileStream* (*raw_vfs_usr_stream_open)(const char* path, bool prefer_res);

/**
 * @brief Open file stream from User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return FileStream* If success, else NULL
 */
extern FileStream* (*trace_vfs_usr_stream_open)(const char *___file___, int ___line___, const char* path, bool prefer_res);

/**
 * @brief Get file stream size in bytes
 *
 * @param path
 * @return size_t
 */
extern size_t (*raw_vfs_stream_size)(FileStream* stream);

/**
 * @brief Get file stream size in bytes
 *
 * @param path
 * @return size_t
 */
extern size_t (*trace_vfs_stream_size)(const char *___file___, int ___line___, FileStream* stream);

/**
 * @brief Read from file stream n bytes to buffer
 *
 * @param stream
 * @param buffer
 * @param size
 * @return size_t
 */
extern size_t (*raw_vfs_stream_read_n)(FileStream* stream, void* buffer, size_t size);

/**
 * @brief Read from file stream n bytes to buffer
 *
 * @param stream
 * @param buffer
 * @param size
 * @return size_t
 */
extern size_t (*trace_vfs_stream_read_n)(const char *___file___, int ___line___, FileStream* stream, void* buffer, size_t size);

/**
 * @brief Allocate buffer and read all file to it
 *
 * @warning You must free returned buffer
 * @param stream
 * @param size
 * @return void*
 */
extern void* (*raw_vfs_stream_read_all)(FileStream* stream, size_t* size);

/**
 * @brief Allocate buffer and read all file to it
 *
 * @warning You must free returned buffer
 * @param stream
 * @param size
 * @return void*
 */
extern void* (*trace_vfs_stream_read_all)(const char *___file___, int ___line___, FileStream* stream, size_t* size);

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
extern size_t (*raw_vfs_stream_write)(FileStream* stream, void* buffer, size_t size);

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
extern size_t (*trace_vfs_stream_write)(const char *___file___, int ___line___, FileStream* stream, void* buffer, size_t size);

/**
 * @brief Seek file stream
 *
 * @param stream
 * @param whence
 * @param offset
 * @return true if success
 * @return false if fail
 */
extern bool (*raw_vfs_stream_seek)(FileStream* stream, enum FSSeekFrom whence, size_t offset);

/**
 * @brief Seek file stream
 *
 * @param stream
 * @param whence
 * @param offset
 * @return true if success
 * @return false if fail
 */
extern bool (*trace_vfs_stream_seek)(const char *___file___, int ___line___, FileStream* stream, enum FSSeekFrom whence, size_t offset);

/**
 * @brief Get file stream current position
 *
 * @param stream
 * @param success Can be NULL
 * @return size_t
 */
extern size_t (*raw_vfs_stream_tell)(FileStream* stream, bool* success);

/**
 * @brief Get file stream current position
 *
 * @param stream
 * @param success Can be NULL
 * @return size_t
 */
extern size_t (*trace_vfs_stream_tell)(const char *___file___, int ___line___, FileStream* stream, bool* success);

/**
 * @brief Flush file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 */
extern bool (*raw_vfs_stream_flush)(FileStream* stream);

/**
 * @brief Flush file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 */
extern bool (*trace_vfs_stream_flush)(const char *___file___, int ___line___, FileStream* stream);

/**
 * @brief Close file stream
 *
 * @param stream
 */
extern void (*raw_vfs_stream_close)(FileStream* stream);

/**
 * @brief Close file stream
 *
 * @param stream
 */
extern void (*trace_vfs_stream_close)(const char *___file___, int ___line___, FileStream* stream);

extern UID (*raw_uid_new)(void);

extern UID (*trace_uid_new)(const char *___file___, int ___line___);

/**
 * @brief Malloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param size
 * @return void*
 */
extern void* (*raw_tmalloc)(size_t size);

/**
 * @brief Malloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param size
 * @return void*
 */
extern void* (*trace_tmalloc)(const char *___file___, int ___line___, size_t size);

/**
 * @brief Realloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 * @param size
 * @return void*
 */
extern void* (*raw_trealloc)(void* ptr, size_t size);

/**
 * @brief Realloc with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 * @param size
 * @return void*
 */
extern void* (*trace_trealloc)(const char *___file___, int ___line___, void* ptr, size_t size);

/**
 * @brief Free with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 */
extern void (*raw_tfree)(void* ptr);

/**
 * @brief Free with tracking if compiled with HE_MEM_TRACK
 *
 * @param ptr
 */
extern void (*trace_tfree)(const char *___file___, int ___line___, void* ptr);

/**
 * @brief Get allocated memory
 *
 * @return size in bytes if compiled with HE_MEM_TRACK, 0 otherwise
 */
extern size_t (*raw_get_allocated_memory)(void);

/**
 * @brief Get allocated memory
 *
 * @return size in bytes if compiled with HE_MEM_TRACK, 0 otherwise
 */
extern size_t (*trace_get_allocated_memory)(const char *___file___, int ___line___);

extern Mat4 (*raw_mat4_new)(
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
);

extern Mat4 (*trace_mat4_new)(const char *___file___, int ___line___, 
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
);

extern Mat4 (*raw_mat4_new_zero)(void);

extern Mat4 (*trace_mat4_new_zero)(const char *___file___, int ___line___);

extern Mat4 (*raw_mat4_new_one)(void);

extern Mat4 (*trace_mat4_new_one)(const char *___file___, int ___line___);

extern Mat4 (*raw_mat4_from_array)(const float* const array);

extern Mat4 (*trace_mat4_from_array)(const char *___file___, int ___line___, const float* const array);

extern Mat4 (*raw_mat4_from_mat4)(const Mat4* const a);

extern Mat4 (*trace_mat4_from_mat4)(const char *___file___, int ___line___, const Mat4* const a);

extern Mat4 (*raw_mat4_add)(const Mat4* const a, const Mat4* const b);

extern Mat4 (*trace_mat4_add)(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b);

extern Mat4 (*raw_mat4_sub)(const Mat4* const a, const Mat4* const b);

extern Mat4 (*trace_mat4_sub)(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b);

extern Mat4 (*raw_mat4_mul)(const Mat4* const a, const Mat4* const b);

extern Mat4 (*trace_mat4_mul)(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b);

extern Mat4 (*raw_mat4_scale)(const Mat4* const a, const float factor);

extern Mat4 (*trace_mat4_scale)(const char *___file___, int ___line___, const Mat4* const a, const float factor);

extern Mat4 (*raw_mat4_transpose)(const Mat4* const a);

extern Mat4 (*trace_mat4_transpose)(const char *___file___, int ___line___, const Mat4* const a);

extern Mat4 (*raw_mat4_inverse)(const Mat4* const a);

extern Mat4 (*trace_mat4_inverse)(const char *___file___, int ___line___, const Mat4* const a);

extern Vec3 (*raw_vec3_new)(float x, float y, float z);

extern Vec3 (*trace_vec3_new)(const char *___file___, int ___line___, float x, float y, float z);

extern Vec3 (*raw_vec3_add)(const Vec3* const a, const Vec3* const b);

extern Vec3 (*trace_vec3_add)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);

extern Vec3 (*raw_vec3_sub)(const Vec3* const a, const Vec3* const b);

extern Vec3 (*trace_vec3_sub)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);

extern Vec3 (*raw_vec3_scale)(const Vec3* const a, const float factor);

extern Vec3 (*trace_vec3_scale)(const char *___file___, int ___line___, const Vec3* const a, const float factor);

extern float (*raw_vec3_dot)(const Vec3* const a, const Vec3* const b);

extern float (*trace_vec3_dot)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);

extern Vec3 (*raw_vec3_cross)(const Vec3* const a, const Vec3* const b);

extern Vec3 (*trace_vec3_cross)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);

extern float (*raw_vec3_length)(const Vec3* const a);

extern float (*trace_vec3_length)(const char *___file___, int ___line___, const Vec3* const a);

extern Vec3 (*raw_vec3_normalize)(const Vec3* const a);

extern Vec3 (*trace_vec3_normalize)(const char *___file___, int ___line___, const Vec3* const a);

extern float (*raw_vec3_distance)(const Vec3* const a, const Vec3* const b);

extern float (*trace_vec3_distance)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 */
extern void (*raw_vec3_add_in)(Vec3* const to, const Vec3* const what);

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 */
extern void (*trace_vec3_add_in)(const char *___file___, int ___line___, Vec3* const to, const Vec3* const what);

extern void (*raw_vec3_sub_in)(Vec3* const from, const Vec3* const what);

extern void (*trace_vec3_sub_in)(const char *___file___, int ___line___, Vec3* const from, const Vec3* const what);

extern void (*raw_vec3_scale_in)(Vec3* const to, const float factor);

extern void (*trace_vec3_scale_in)(const char *___file___, int ___line___, Vec3* const to, const float factor);

extern void (*raw_vec3_normalize_in)(Vec3* const a);

extern void (*trace_vec3_normalize_in)(const char *___file___, int ___line___, Vec3* const a);

/**
 * @brief Free object by type
 *
 * @param object
 */
extern void (*raw_auto_free)(Object* object);

/**
 * @brief Free object by type
 *
 * @param object
 */
extern void (*trace_auto_free)(const char *___file___, int ___line___, Object* object);

extern Node* (*raw_node_new)(const char* name);

extern Node* (*trace_node_new)(const char *___file___, int ___line___, const char* name);

extern Node* (*raw_from_node)(Node* node);

extern Node* (*trace_from_node)(const char *___file___, int ___line___, Node* node);

extern void (*raw_node_set_name)(Node* node, const char* name);

extern void (*trace_node_set_name)(const char *___file___, int ___line___, Node* node, const char* name);

extern const char* (*raw_node_get_name)(Node* node);

extern const char* (*trace_node_get_name)(const char *___file___, int ___line___, Node* node);

/**
 * @brief Add a child node to a parent node
 * @warning Take ownership of child. Do not free it!
 * @param node
 * @param child
 */
extern void (*raw_node_add_child)(Node* node, Node* child);

/**
 * @brief Add a child node to a parent node
 * @warning Take ownership of child. Do not free it!
 * @param node
 * @param child
 */
extern void (*trace_node_add_child)(const char *___file___, int ___line___, Node* node, Node* child);

/**
 * @brief Remove and free a child node by pointer from a parent node
 *
 * @param node
 * @param child
 * @return true if child was removed
 * @return false if child was not found
 */
extern bool (*raw_node_remove_child)(Node* node, Node* child);

/**
 * @brief Remove and free a child node by pointer from a parent node
 *
 * @param node
 * @param child
 * @return true if child was removed
 * @return false if child was not found
 */
extern bool (*trace_node_remove_child)(const char *___file___, int ___line___, Node* node, Node* child);

/**
 * @brief Remove and free a child node by name from a parent node
 *
 * @param node
 * @param name
 * @return true if child was removed
 * @return false if child was not found
 */
extern bool (*raw_node_remove_child_by_name)(Node* node, const char* name);

/**
 * @brief Remove and free a child node by name from a parent node
 *
 * @param node
 * @param name
 * @return true if child was removed
 * @return false if child was not found
 */
extern bool (*trace_node_remove_child_by_name)(const char *___file___, int ___line___, Node* node, const char* name);

/**
 * @brief Remove and free a child node by uid from a parent node
 *
 * @param node
 * @param uid
 * @return true if child was removed
 * @return false if child was not found
 */
extern bool (*raw_node_remove_child_by_uid)(Node* node, UID uid);

/**
 * @brief Remove and free a child node by uid from a parent node
 *
 * @param node
 * @param uid
 * @return true if child was removed
 * @return false if child was not found
 */
extern bool (*trace_node_remove_child_by_uid)(const char *___file___, int ___line___, Node* node, UID uid);

extern bool (*raw_node_remove_all_children)(Node* node);

extern bool (*trace_node_remove_all_children)(const char *___file___, int ___line___, Node* node);

/**
 * @brief Get a child node by name
 *
 * @param node
 * @param name
 * @return Pointer to found child, NULL if not found
 */
extern Node* (*raw_node_get_child_by_name)(Node* node, const char* name);

/**
 * @brief Get a child node by name
 *
 * @param node
 * @param name
 * @return Pointer to found child, NULL if not found
 */
extern Node* (*trace_node_get_child_by_name)(const char *___file___, int ___line___, Node* node, const char* name);

extern Node* (*raw_node_get_child_by_uid)(Node* node, UID uid);

extern Node* (*trace_node_get_child_by_uid)(const char *___file___, int ___line___, Node* node, UID uid);

extern Node3D* (*raw_node3d_new)(void);

extern Node3D* (*trace_node3d_new)(const char *___file___, int ___line___);



#if defined(HEAPI_LOAD_IMPL)
        bool (*raw_vfs_mount_res)(const char* path, const char* mount_point);
    bool (*trace_vfs_mount_res)(const char *___file___, int ___line___, const char* path, const char* mount_point);
    bool (*raw_vfs_unmount_res)(const char* path, const char* mount_point);
    bool (*trace_vfs_unmount_res)(const char *___file___, int ___line___, const char* path, const char* mount_point);
    bool (*raw_vfs_mount_rfs)(const char* mount_point);
    bool (*trace_vfs_mount_rfs)(const char *___file___, int ___line___, const char* mount_point);
    bool (*raw_vfs_mount_rfs_whitelist)(const char** whitelist, size_t count, const char* mount_point);
    bool (*trace_vfs_mount_rfs_whitelist)(const char *___file___, int ___line___, const char** whitelist, size_t count, const char* mount_point);
    bool (*raw_vfs_unmount_rfs)(void);
    bool (*trace_vfs_unmount_rfs)(const char *___file___, int ___line___);
    bool (*raw_vfs_res_path_exists)(const char* path);
    bool (*trace_vfs_res_path_exists)(const char *___file___, int ___line___, const char* path);
    bool (*raw_vfs_usr_path_exists)(const char* path, bool prefer_res);
    bool (*trace_vfs_usr_path_exists)(const char *___file___, int ___line___, const char* path, bool prefer_res);
    void* (*raw_vfs_res_read_file)(const char* path, size_t* size);
    void* (*trace_vfs_res_read_file)(const char *___file___, int ___line___, const char* path, size_t* size);
    FileStream* (*raw_vfs_res_stream_open)(const char* path);
    FileStream* (*trace_vfs_res_stream_open)(const char *___file___, int ___line___, const char* path);
    void* (*raw_vfs_usr_read_file)(const char* path, size_t* size, bool prefer_res);
    void* (*trace_vfs_usr_read_file)(const char *___file___, int ___line___, const char* path, size_t* size, bool prefer_res);
    bool (*raw_vfs_usr_write_file)(const char* path, const void* data, size_t size);
    bool (*trace_vfs_usr_write_file)(const char *___file___, int ___line___, const char* path, const void* data, size_t size);
    FileStream* (*raw_vfs_usr_stream_open)(const char* path, bool prefer_res);
    FileStream* (*trace_vfs_usr_stream_open)(const char *___file___, int ___line___, const char* path, bool prefer_res);
    size_t (*raw_vfs_stream_size)(FileStream* stream);
    size_t (*trace_vfs_stream_size)(const char *___file___, int ___line___, FileStream* stream);
    size_t (*raw_vfs_stream_read_n)(FileStream* stream, void* buffer, size_t size);
    size_t (*trace_vfs_stream_read_n)(const char *___file___, int ___line___, FileStream* stream, void* buffer, size_t size);
    void* (*raw_vfs_stream_read_all)(FileStream* stream, size_t* size);
    void* (*trace_vfs_stream_read_all)(const char *___file___, int ___line___, FileStream* stream, size_t* size);
    size_t (*raw_vfs_stream_write)(FileStream* stream, void* buffer, size_t size);
    size_t (*trace_vfs_stream_write)(const char *___file___, int ___line___, FileStream* stream, void* buffer, size_t size);
    bool (*raw_vfs_stream_seek)(FileStream* stream, enum FSSeekFrom whence, size_t offset);
    bool (*trace_vfs_stream_seek)(const char *___file___, int ___line___, FileStream* stream, enum FSSeekFrom whence, size_t offset);
    size_t (*raw_vfs_stream_tell)(FileStream* stream, bool* success);
    size_t (*trace_vfs_stream_tell)(const char *___file___, int ___line___, FileStream* stream, bool* success);
    bool (*raw_vfs_stream_flush)(FileStream* stream);
    bool (*trace_vfs_stream_flush)(const char *___file___, int ___line___, FileStream* stream);
    void (*raw_vfs_stream_close)(FileStream* stream);
    void (*trace_vfs_stream_close)(const char *___file___, int ___line___, FileStream* stream);
    UID (*raw_uid_new)(void);
    UID (*trace_uid_new)(const char *___file___, int ___line___);
    void* (*raw_tmalloc)(size_t size);
    void* (*trace_tmalloc)(const char *___file___, int ___line___, size_t size);
    void* (*raw_trealloc)(void* ptr, size_t size);
    void* (*trace_trealloc)(const char *___file___, int ___line___, void* ptr, size_t size);
    void (*raw_tfree)(void* ptr);
    void (*trace_tfree)(const char *___file___, int ___line___, void* ptr);
    size_t (*raw_get_allocated_memory)(void);
    size_t (*trace_get_allocated_memory)(const char *___file___, int ___line___);
    Mat4 (*raw_mat4_new)(
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
);
    Mat4 (*trace_mat4_new)(const char *___file___, int ___line___, 
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
);
    Mat4 (*raw_mat4_new_zero)(void);
    Mat4 (*trace_mat4_new_zero)(const char *___file___, int ___line___);
    Mat4 (*raw_mat4_new_one)(void);
    Mat4 (*trace_mat4_new_one)(const char *___file___, int ___line___);
    Mat4 (*raw_mat4_from_array)(const float* const array);
    Mat4 (*trace_mat4_from_array)(const char *___file___, int ___line___, const float* const array);
    Mat4 (*raw_mat4_from_mat4)(const Mat4* const a);
    Mat4 (*trace_mat4_from_mat4)(const char *___file___, int ___line___, const Mat4* const a);
    Mat4 (*raw_mat4_add)(const Mat4* const a, const Mat4* const b);
    Mat4 (*trace_mat4_add)(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b);
    Mat4 (*raw_mat4_sub)(const Mat4* const a, const Mat4* const b);
    Mat4 (*trace_mat4_sub)(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b);
    Mat4 (*raw_mat4_mul)(const Mat4* const a, const Mat4* const b);
    Mat4 (*trace_mat4_mul)(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b);
    Mat4 (*raw_mat4_scale)(const Mat4* const a, const float factor);
    Mat4 (*trace_mat4_scale)(const char *___file___, int ___line___, const Mat4* const a, const float factor);
    Mat4 (*raw_mat4_transpose)(const Mat4* const a);
    Mat4 (*trace_mat4_transpose)(const char *___file___, int ___line___, const Mat4* const a);
    Mat4 (*raw_mat4_inverse)(const Mat4* const a);
    Mat4 (*trace_mat4_inverse)(const char *___file___, int ___line___, const Mat4* const a);
    Vec3 (*raw_vec3_new)(float x, float y, float z);
    Vec3 (*trace_vec3_new)(const char *___file___, int ___line___, float x, float y, float z);
    Vec3 (*raw_vec3_add)(const Vec3* const a, const Vec3* const b);
    Vec3 (*trace_vec3_add)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);
    Vec3 (*raw_vec3_sub)(const Vec3* const a, const Vec3* const b);
    Vec3 (*trace_vec3_sub)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);
    Vec3 (*raw_vec3_scale)(const Vec3* const a, const float factor);
    Vec3 (*trace_vec3_scale)(const char *___file___, int ___line___, const Vec3* const a, const float factor);
    float (*raw_vec3_dot)(const Vec3* const a, const Vec3* const b);
    float (*trace_vec3_dot)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);
    Vec3 (*raw_vec3_cross)(const Vec3* const a, const Vec3* const b);
    Vec3 (*trace_vec3_cross)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);
    float (*raw_vec3_length)(const Vec3* const a);
    float (*trace_vec3_length)(const char *___file___, int ___line___, const Vec3* const a);
    Vec3 (*raw_vec3_normalize)(const Vec3* const a);
    Vec3 (*trace_vec3_normalize)(const char *___file___, int ___line___, const Vec3* const a);
    float (*raw_vec3_distance)(const Vec3* const a, const Vec3* const b);
    float (*trace_vec3_distance)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);
    void (*raw_vec3_add_in)(Vec3* const to, const Vec3* const what);
    void (*trace_vec3_add_in)(const char *___file___, int ___line___, Vec3* const to, const Vec3* const what);
    void (*raw_vec3_sub_in)(Vec3* const from, const Vec3* const what);
    void (*trace_vec3_sub_in)(const char *___file___, int ___line___, Vec3* const from, const Vec3* const what);
    void (*raw_vec3_scale_in)(Vec3* const to, const float factor);
    void (*trace_vec3_scale_in)(const char *___file___, int ___line___, Vec3* const to, const float factor);
    void (*raw_vec3_normalize_in)(Vec3* const a);
    void (*trace_vec3_normalize_in)(const char *___file___, int ___line___, Vec3* const a);
    void (*raw_auto_free)(Object* object);
    void (*trace_auto_free)(const char *___file___, int ___line___, Object* object);
    Node* (*raw_node_new)(const char* name);
    Node* (*trace_node_new)(const char *___file___, int ___line___, const char* name);
    Node* (*raw_from_node)(Node* node);
    Node* (*trace_from_node)(const char *___file___, int ___line___, Node* node);
    void (*raw_node_set_name)(Node* node, const char* name);
    void (*trace_node_set_name)(const char *___file___, int ___line___, Node* node, const char* name);
    const char* (*raw_node_get_name)(Node* node);
    const char* (*trace_node_get_name)(const char *___file___, int ___line___, Node* node);
    void (*raw_node_add_child)(Node* node, Node* child);
    void (*trace_node_add_child)(const char *___file___, int ___line___, Node* node, Node* child);
    bool (*raw_node_remove_child)(Node* node, Node* child);
    bool (*trace_node_remove_child)(const char *___file___, int ___line___, Node* node, Node* child);
    bool (*raw_node_remove_child_by_name)(Node* node, const char* name);
    bool (*trace_node_remove_child_by_name)(const char *___file___, int ___line___, Node* node, const char* name);
    bool (*raw_node_remove_child_by_uid)(Node* node, UID uid);
    bool (*trace_node_remove_child_by_uid)(const char *___file___, int ___line___, Node* node, UID uid);
    bool (*raw_node_remove_all_children)(Node* node);
    bool (*trace_node_remove_all_children)(const char *___file___, int ___line___, Node* node);
    Node* (*raw_node_get_child_by_name)(Node* node, const char* name);
    Node* (*trace_node_get_child_by_name)(const char *___file___, int ___line___, Node* node, const char* name);
    Node* (*raw_node_get_child_by_uid)(Node* node, UID uid);
    Node* (*trace_node_get_child_by_uid)(const char *___file___, int ___line___, Node* node, UID uid);
    Node3D* (*raw_node3d_new)(void);
    Node3D* (*trace_node3d_new)(const char *___file___, int ___line___);


    void ___hate_engine_runtime_init(void* (*proc_addr)(const char* name)) {
            raw_vfs_mount_res = (bool (*)(const char* path, const char* mount_point))proc_addr("vfs_mount_res");
    trace_vfs_mount_res = (bool (*)(const char *___file___, int ___line___, const char* path, const char* mount_point))proc_addr("t_vfs_mount_res");
    raw_vfs_unmount_res = (bool (*)(const char* path, const char* mount_point))proc_addr("vfs_unmount_res");
    trace_vfs_unmount_res = (bool (*)(const char *___file___, int ___line___, const char* path, const char* mount_point))proc_addr("t_vfs_unmount_res");
    raw_vfs_mount_rfs = (bool (*)(const char* mount_point))proc_addr("vfs_mount_rfs");
    trace_vfs_mount_rfs = (bool (*)(const char *___file___, int ___line___, const char* mount_point))proc_addr("t_vfs_mount_rfs");
    raw_vfs_mount_rfs_whitelist = (bool (*)(const char** whitelist, size_t count, const char* mount_point))proc_addr("vfs_mount_rfs_whitelist");
    trace_vfs_mount_rfs_whitelist = (bool (*)(const char *___file___, int ___line___, const char** whitelist, size_t count, const char* mount_point))proc_addr("t_vfs_mount_rfs_whitelist");
    raw_vfs_unmount_rfs = (bool (*)(void))proc_addr("vfs_unmount_rfs");
    trace_vfs_unmount_rfs = (bool (*)(const char *___file___, int ___line___))proc_addr("t_vfs_unmount_rfs");
    raw_vfs_res_path_exists = (bool (*)(const char* path))proc_addr("vfs_res_path_exists");
    trace_vfs_res_path_exists = (bool (*)(const char *___file___, int ___line___, const char* path))proc_addr("t_vfs_res_path_exists");
    raw_vfs_usr_path_exists = (bool (*)(const char* path, bool prefer_res))proc_addr("vfs_usr_path_exists");
    trace_vfs_usr_path_exists = (bool (*)(const char *___file___, int ___line___, const char* path, bool prefer_res))proc_addr("t_vfs_usr_path_exists");
    raw_vfs_res_read_file = (void* (*)(const char* path, size_t* size))proc_addr("vfs_res_read_file");
    trace_vfs_res_read_file = (void* (*)(const char *___file___, int ___line___, const char* path, size_t* size))proc_addr("t_vfs_res_read_file");
    raw_vfs_res_stream_open = (FileStream* (*)(const char* path))proc_addr("vfs_res_stream_open");
    trace_vfs_res_stream_open = (FileStream* (*)(const char *___file___, int ___line___, const char* path))proc_addr("t_vfs_res_stream_open");
    raw_vfs_usr_read_file = (void* (*)(const char* path, size_t* size, bool prefer_res))proc_addr("vfs_usr_read_file");
    trace_vfs_usr_read_file = (void* (*)(const char *___file___, int ___line___, const char* path, size_t* size, bool prefer_res))proc_addr("t_vfs_usr_read_file");
    raw_vfs_usr_write_file = (bool (*)(const char* path, const void* data, size_t size))proc_addr("vfs_usr_write_file");
    trace_vfs_usr_write_file = (bool (*)(const char *___file___, int ___line___, const char* path, const void* data, size_t size))proc_addr("t_vfs_usr_write_file");
    raw_vfs_usr_stream_open = (FileStream* (*)(const char* path, bool prefer_res))proc_addr("vfs_usr_stream_open");
    trace_vfs_usr_stream_open = (FileStream* (*)(const char *___file___, int ___line___, const char* path, bool prefer_res))proc_addr("t_vfs_usr_stream_open");
    raw_vfs_stream_size = (size_t (*)(FileStream* stream))proc_addr("vfs_stream_size");
    trace_vfs_stream_size = (size_t (*)(const char *___file___, int ___line___, FileStream* stream))proc_addr("t_vfs_stream_size");
    raw_vfs_stream_read_n = (size_t (*)(FileStream* stream, void* buffer, size_t size))proc_addr("vfs_stream_read_n");
    trace_vfs_stream_read_n = (size_t (*)(const char *___file___, int ___line___, FileStream* stream, void* buffer, size_t size))proc_addr("t_vfs_stream_read_n");
    raw_vfs_stream_read_all = (void* (*)(FileStream* stream, size_t* size))proc_addr("vfs_stream_read_all");
    trace_vfs_stream_read_all = (void* (*)(const char *___file___, int ___line___, FileStream* stream, size_t* size))proc_addr("t_vfs_stream_read_all");
    raw_vfs_stream_write = (size_t (*)(FileStream* stream, void* buffer, size_t size))proc_addr("vfs_stream_write");
    trace_vfs_stream_write = (size_t (*)(const char *___file___, int ___line___, FileStream* stream, void* buffer, size_t size))proc_addr("t_vfs_stream_write");
    raw_vfs_stream_seek = (bool (*)(FileStream* stream, enum FSSeekFrom whence, size_t offset))proc_addr("vfs_stream_seek");
    trace_vfs_stream_seek = (bool (*)(const char *___file___, int ___line___, FileStream* stream, enum FSSeekFrom whence, size_t offset))proc_addr("t_vfs_stream_seek");
    raw_vfs_stream_tell = (size_t (*)(FileStream* stream, bool* success))proc_addr("vfs_stream_tell");
    trace_vfs_stream_tell = (size_t (*)(const char *___file___, int ___line___, FileStream* stream, bool* success))proc_addr("t_vfs_stream_tell");
    raw_vfs_stream_flush = (bool (*)(FileStream* stream))proc_addr("vfs_stream_flush");
    trace_vfs_stream_flush = (bool (*)(const char *___file___, int ___line___, FileStream* stream))proc_addr("t_vfs_stream_flush");
    raw_vfs_stream_close = (void (*)(FileStream* stream))proc_addr("vfs_stream_close");
    trace_vfs_stream_close = (void (*)(const char *___file___, int ___line___, FileStream* stream))proc_addr("t_vfs_stream_close");
    raw_uid_new = (UID (*)(void))proc_addr("uid_new");
    trace_uid_new = (UID (*)(const char *___file___, int ___line___))proc_addr("t_uid_new");
    raw_tmalloc = (void* (*)(size_t size))proc_addr("tmalloc");
    trace_tmalloc = (void* (*)(const char *___file___, int ___line___, size_t size))proc_addr("t_tmalloc");
    raw_trealloc = (void* (*)(void* ptr, size_t size))proc_addr("trealloc");
    trace_trealloc = (void* (*)(const char *___file___, int ___line___, void* ptr, size_t size))proc_addr("t_trealloc");
    raw_tfree = (void (*)(void* ptr))proc_addr("tfree");
    trace_tfree = (void (*)(const char *___file___, int ___line___, void* ptr))proc_addr("t_tfree");
    raw_get_allocated_memory = (size_t (*)(void))proc_addr("get_allocated_memory");
    trace_get_allocated_memory = (size_t (*)(const char *___file___, int ___line___))proc_addr("t_get_allocated_memory");
    raw_mat4_new = (Mat4 (*)(
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
))proc_addr("mat4_new");
    trace_mat4_new = (Mat4 (*)(const char *___file___, int ___line___, 
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
))proc_addr("t_mat4_new");
    raw_mat4_new_zero = (Mat4 (*)(void))proc_addr("mat4_new_zero");
    trace_mat4_new_zero = (Mat4 (*)(const char *___file___, int ___line___))proc_addr("t_mat4_new_zero");
    raw_mat4_new_one = (Mat4 (*)(void))proc_addr("mat4_new_one");
    trace_mat4_new_one = (Mat4 (*)(const char *___file___, int ___line___))proc_addr("t_mat4_new_one");
    raw_mat4_from_array = (Mat4 (*)(const float* const array))proc_addr("mat4_from_array");
    trace_mat4_from_array = (Mat4 (*)(const char *___file___, int ___line___, const float* const array))proc_addr("t_mat4_from_array");
    raw_mat4_from_mat4 = (Mat4 (*)(const Mat4* const a))proc_addr("mat4_from_mat4");
    trace_mat4_from_mat4 = (Mat4 (*)(const char *___file___, int ___line___, const Mat4* const a))proc_addr("t_mat4_from_mat4");
    raw_mat4_add = (Mat4 (*)(const Mat4* const a, const Mat4* const b))proc_addr("mat4_add");
    trace_mat4_add = (Mat4 (*)(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b))proc_addr("t_mat4_add");
    raw_mat4_sub = (Mat4 (*)(const Mat4* const a, const Mat4* const b))proc_addr("mat4_sub");
    trace_mat4_sub = (Mat4 (*)(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b))proc_addr("t_mat4_sub");
    raw_mat4_mul = (Mat4 (*)(const Mat4* const a, const Mat4* const b))proc_addr("mat4_mul");
    trace_mat4_mul = (Mat4 (*)(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b))proc_addr("t_mat4_mul");
    raw_mat4_scale = (Mat4 (*)(const Mat4* const a, const float factor))proc_addr("mat4_scale");
    trace_mat4_scale = (Mat4 (*)(const char *___file___, int ___line___, const Mat4* const a, const float factor))proc_addr("t_mat4_scale");
    raw_mat4_transpose = (Mat4 (*)(const Mat4* const a))proc_addr("mat4_transpose");
    trace_mat4_transpose = (Mat4 (*)(const char *___file___, int ___line___, const Mat4* const a))proc_addr("t_mat4_transpose");
    raw_mat4_inverse = (Mat4 (*)(const Mat4* const a))proc_addr("mat4_inverse");
    trace_mat4_inverse = (Mat4 (*)(const char *___file___, int ___line___, const Mat4* const a))proc_addr("t_mat4_inverse");
    raw_vec3_new = (Vec3 (*)(float x, float y, float z))proc_addr("vec3_new");
    trace_vec3_new = (Vec3 (*)(const char *___file___, int ___line___, float x, float y, float z))proc_addr("t_vec3_new");
    raw_vec3_add = (Vec3 (*)(const Vec3* const a, const Vec3* const b))proc_addr("vec3_add");
    trace_vec3_add = (Vec3 (*)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b))proc_addr("t_vec3_add");
    raw_vec3_sub = (Vec3 (*)(const Vec3* const a, const Vec3* const b))proc_addr("vec3_sub");
    trace_vec3_sub = (Vec3 (*)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b))proc_addr("t_vec3_sub");
    raw_vec3_scale = (Vec3 (*)(const Vec3* const a, const float factor))proc_addr("vec3_scale");
    trace_vec3_scale = (Vec3 (*)(const char *___file___, int ___line___, const Vec3* const a, const float factor))proc_addr("t_vec3_scale");
    raw_vec3_dot = (float (*)(const Vec3* const a, const Vec3* const b))proc_addr("vec3_dot");
    trace_vec3_dot = (float (*)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b))proc_addr("t_vec3_dot");
    raw_vec3_cross = (Vec3 (*)(const Vec3* const a, const Vec3* const b))proc_addr("vec3_cross");
    trace_vec3_cross = (Vec3 (*)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b))proc_addr("t_vec3_cross");
    raw_vec3_length = (float (*)(const Vec3* const a))proc_addr("vec3_length");
    trace_vec3_length = (float (*)(const char *___file___, int ___line___, const Vec3* const a))proc_addr("t_vec3_length");
    raw_vec3_normalize = (Vec3 (*)(const Vec3* const a))proc_addr("vec3_normalize");
    trace_vec3_normalize = (Vec3 (*)(const char *___file___, int ___line___, const Vec3* const a))proc_addr("t_vec3_normalize");
    raw_vec3_distance = (float (*)(const Vec3* const a, const Vec3* const b))proc_addr("vec3_distance");
    trace_vec3_distance = (float (*)(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b))proc_addr("t_vec3_distance");
    raw_vec3_add_in = (void (*)(Vec3* const to, const Vec3* const what))proc_addr("vec3_add_in");
    trace_vec3_add_in = (void (*)(const char *___file___, int ___line___, Vec3* const to, const Vec3* const what))proc_addr("t_vec3_add_in");
    raw_vec3_sub_in = (void (*)(Vec3* const from, const Vec3* const what))proc_addr("vec3_sub_in");
    trace_vec3_sub_in = (void (*)(const char *___file___, int ___line___, Vec3* const from, const Vec3* const what))proc_addr("t_vec3_sub_in");
    raw_vec3_scale_in = (void (*)(Vec3* const to, const float factor))proc_addr("vec3_scale_in");
    trace_vec3_scale_in = (void (*)(const char *___file___, int ___line___, Vec3* const to, const float factor))proc_addr("t_vec3_scale_in");
    raw_vec3_normalize_in = (void (*)(Vec3* const a))proc_addr("vec3_normalize_in");
    trace_vec3_normalize_in = (void (*)(const char *___file___, int ___line___, Vec3* const a))proc_addr("t_vec3_normalize_in");
    raw_auto_free = (void (*)(Object* object))proc_addr("auto_free");
    trace_auto_free = (void (*)(const char *___file___, int ___line___, Object* object))proc_addr("t_auto_free");
    raw_node_new = (Node* (*)(const char* name))proc_addr("node_new");
    trace_node_new = (Node* (*)(const char *___file___, int ___line___, const char* name))proc_addr("t_node_new");
    raw_from_node = (Node* (*)(Node* node))proc_addr("from_node");
    trace_from_node = (Node* (*)(const char *___file___, int ___line___, Node* node))proc_addr("t_from_node");
    raw_node_set_name = (void (*)(Node* node, const char* name))proc_addr("node_set_name");
    trace_node_set_name = (void (*)(const char *___file___, int ___line___, Node* node, const char* name))proc_addr("t_node_set_name");
    raw_node_get_name = (const char* (*)(Node* node))proc_addr("node_get_name");
    trace_node_get_name = (const char* (*)(const char *___file___, int ___line___, Node* node))proc_addr("t_node_get_name");
    raw_node_add_child = (void (*)(Node* node, Node* child))proc_addr("node_add_child");
    trace_node_add_child = (void (*)(const char *___file___, int ___line___, Node* node, Node* child))proc_addr("t_node_add_child");
    raw_node_remove_child = (bool (*)(Node* node, Node* child))proc_addr("node_remove_child");
    trace_node_remove_child = (bool (*)(const char *___file___, int ___line___, Node* node, Node* child))proc_addr("t_node_remove_child");
    raw_node_remove_child_by_name = (bool (*)(Node* node, const char* name))proc_addr("node_remove_child_by_name");
    trace_node_remove_child_by_name = (bool (*)(const char *___file___, int ___line___, Node* node, const char* name))proc_addr("t_node_remove_child_by_name");
    raw_node_remove_child_by_uid = (bool (*)(Node* node, UID uid))proc_addr("node_remove_child_by_uid");
    trace_node_remove_child_by_uid = (bool (*)(const char *___file___, int ___line___, Node* node, UID uid))proc_addr("t_node_remove_child_by_uid");
    raw_node_remove_all_children = (bool (*)(Node* node))proc_addr("node_remove_all_children");
    trace_node_remove_all_children = (bool (*)(const char *___file___, int ___line___, Node* node))proc_addr("t_node_remove_all_children");
    raw_node_get_child_by_name = (Node* (*)(Node* node, const char* name))proc_addr("node_get_child_by_name");
    trace_node_get_child_by_name = (Node* (*)(const char *___file___, int ___line___, Node* node, const char* name))proc_addr("t_node_get_child_by_name");
    raw_node_get_child_by_uid = (Node* (*)(Node* node, UID uid))proc_addr("node_get_child_by_uid");
    trace_node_get_child_by_uid = (Node* (*)(const char *___file___, int ___line___, Node* node, UID uid))proc_addr("t_node_get_child_by_uid");
    raw_node3d_new = (Node3D* (*)(void))proc_addr("node3d_new");
    trace_node3d_new = (Node3D* (*)(const char *___file___, int ___line___))proc_addr("t_node3d_new");

    }
#endif
#endif

#if defined(HEAPI_FULL_TRACE)
#define vfs_mount_res(path, mount_point) trace_vfs_mount_res(__FILE__, __LINE__,  path, mount_point)
#define vfs_unmount_res(path, mount_point) trace_vfs_unmount_res(__FILE__, __LINE__,  path, mount_point)
#define vfs_mount_rfs(mount_point) trace_vfs_mount_rfs(__FILE__, __LINE__,  mount_point)
#define vfs_mount_rfs_whitelist(whitelist, count, mount_point) trace_vfs_mount_rfs_whitelist(__FILE__, __LINE__,  whitelist, count, mount_point)
#define vfs_unmount_rfs() trace_vfs_unmount_rfs(__FILE__, __LINE__ )
#define vfs_res_path_exists(path) trace_vfs_res_path_exists(__FILE__, __LINE__,  path)
#define vfs_usr_path_exists(path, prefer_res) trace_vfs_usr_path_exists(__FILE__, __LINE__,  path, prefer_res)
#define vfs_res_read_file(path, size) trace_vfs_res_read_file(__FILE__, __LINE__,  path, size)
#define vfs_res_stream_open(path) trace_vfs_res_stream_open(__FILE__, __LINE__,  path)
#define vfs_usr_read_file(path, size, prefer_res) trace_vfs_usr_read_file(__FILE__, __LINE__,  path, size, prefer_res)
#define vfs_usr_write_file(path, data, size) trace_vfs_usr_write_file(__FILE__, __LINE__,  path, data, size)
#define vfs_usr_stream_open(path, prefer_res) trace_vfs_usr_stream_open(__FILE__, __LINE__,  path, prefer_res)
#define vfs_stream_size(stream) trace_vfs_stream_size(__FILE__, __LINE__,  stream)
#define vfs_stream_read_n(stream, buffer, size) trace_vfs_stream_read_n(__FILE__, __LINE__,  stream, buffer, size)
#define vfs_stream_read_all(stream, size) trace_vfs_stream_read_all(__FILE__, __LINE__,  stream, size)
#define vfs_stream_write(stream, buffer, size) trace_vfs_stream_write(__FILE__, __LINE__,  stream, buffer, size)
#define vfs_stream_seek(stream, whence, offset) trace_vfs_stream_seek(__FILE__, __LINE__,  stream, whence, offset)
#define vfs_stream_tell(stream, success) trace_vfs_stream_tell(__FILE__, __LINE__,  stream, success)
#define vfs_stream_flush(stream) trace_vfs_stream_flush(__FILE__, __LINE__,  stream)
#define vfs_stream_close(stream) trace_vfs_stream_close(__FILE__, __LINE__,  stream)
#define uid_new() trace_uid_new(__FILE__, __LINE__ )
#define tmalloc(size) trace_tmalloc(__FILE__, __LINE__,  size)
#define trealloc(ptr, size) trace_trealloc(__FILE__, __LINE__,  ptr, size)
#define tfree(ptr) trace_tfree(__FILE__, __LINE__,  ptr)
#define get_allocated_memory() trace_get_allocated_memory(__FILE__, __LINE__ )
#define mat4_new(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) trace_mat4_new(__FILE__, __LINE__,  m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)
#define mat4_new_zero() trace_mat4_new_zero(__FILE__, __LINE__ )
#define mat4_new_one() trace_mat4_new_one(__FILE__, __LINE__ )
#define mat4_from_array(array) trace_mat4_from_array(__FILE__, __LINE__,  array)
#define mat4_from_mat4(a) trace_mat4_from_mat4(__FILE__, __LINE__,  a)
#define mat4_add(a, b) trace_mat4_add(__FILE__, __LINE__,  a, b)
#define mat4_sub(a, b) trace_mat4_sub(__FILE__, __LINE__,  a, b)
#define mat4_mul(a, b) trace_mat4_mul(__FILE__, __LINE__,  a, b)
#define mat4_scale(a, factor) trace_mat4_scale(__FILE__, __LINE__,  a, factor)
#define mat4_transpose(a) trace_mat4_transpose(__FILE__, __LINE__,  a)
#define mat4_inverse(a) trace_mat4_inverse(__FILE__, __LINE__,  a)
#define vec3_new(x, y, z) trace_vec3_new(__FILE__, __LINE__,  x, y, z)
#define vec3_add(a, b) trace_vec3_add(__FILE__, __LINE__,  a, b)
#define vec3_sub(a, b) trace_vec3_sub(__FILE__, __LINE__,  a, b)
#define vec3_scale(a, factor) trace_vec3_scale(__FILE__, __LINE__,  a, factor)
#define vec3_dot(a, b) trace_vec3_dot(__FILE__, __LINE__,  a, b)
#define vec3_cross(a, b) trace_vec3_cross(__FILE__, __LINE__,  a, b)
#define vec3_length(a) trace_vec3_length(__FILE__, __LINE__,  a)
#define vec3_normalize(a) trace_vec3_normalize(__FILE__, __LINE__,  a)
#define vec3_distance(a, b) trace_vec3_distance(__FILE__, __LINE__,  a, b)
#define vec3_add_in(to, what) trace_vec3_add_in(__FILE__, __LINE__,  to, what)
#define vec3_sub_in(from, what) trace_vec3_sub_in(__FILE__, __LINE__,  from, what)
#define vec3_scale_in(to, factor) trace_vec3_scale_in(__FILE__, __LINE__,  to, factor)
#define vec3_normalize_in(a) trace_vec3_normalize_in(__FILE__, __LINE__,  a)
#define auto_free(object) trace_auto_free(__FILE__, __LINE__,  object)
#define node_new(name) trace_node_new(__FILE__, __LINE__,  name)
#define from_node(node) trace_from_node(__FILE__, __LINE__,  node)
#define node_set_name(node, name) trace_node_set_name(__FILE__, __LINE__,  node, name)
#define node_get_name(node) trace_node_get_name(__FILE__, __LINE__,  node)
#define node_add_child(node, child) trace_node_add_child(__FILE__, __LINE__,  node, child)
#define node_remove_child(node, child) trace_node_remove_child(__FILE__, __LINE__,  node, child)
#define node_remove_child_by_name(node, name) trace_node_remove_child_by_name(__FILE__, __LINE__,  node, name)
#define node_remove_child_by_uid(node, uid) trace_node_remove_child_by_uid(__FILE__, __LINE__,  node, uid)
#define node_remove_all_children(node) trace_node_remove_all_children(__FILE__, __LINE__,  node)
#define node_get_child_by_name(node, name) trace_node_get_child_by_name(__FILE__, __LINE__,  node, name)
#define node_get_child_by_uid(node, uid) trace_node_get_child_by_uid(__FILE__, __LINE__,  node, uid)
#define node3d_new() trace_node3d_new(__FILE__, __LINE__ )

#else
#define vfs_mount_res(path, mount_point) raw_vfs_mount_res(path, mount_point)
#define vfs_unmount_res(path, mount_point) raw_vfs_unmount_res(path, mount_point)
#define vfs_mount_rfs(mount_point) raw_vfs_mount_rfs(mount_point)
#define vfs_mount_rfs_whitelist(whitelist, count, mount_point) raw_vfs_mount_rfs_whitelist(whitelist, count, mount_point)
#define vfs_unmount_rfs() raw_vfs_unmount_rfs()
#define vfs_res_path_exists(path) raw_vfs_res_path_exists(path)
#define vfs_usr_path_exists(path, prefer_res) raw_vfs_usr_path_exists(path, prefer_res)
#define vfs_res_read_file(path, size) raw_vfs_res_read_file(path, size)
#define vfs_res_stream_open(path) raw_vfs_res_stream_open(path)
#define vfs_usr_read_file(path, size, prefer_res) raw_vfs_usr_read_file(path, size, prefer_res)
#define vfs_usr_write_file(path, data, size) raw_vfs_usr_write_file(path, data, size)
#define vfs_usr_stream_open(path, prefer_res) raw_vfs_usr_stream_open(path, prefer_res)
#define vfs_stream_size(stream) raw_vfs_stream_size(stream)
#define vfs_stream_read_n(stream, buffer, size) raw_vfs_stream_read_n(stream, buffer, size)
#define vfs_stream_read_all(stream, size) raw_vfs_stream_read_all(stream, size)
#define vfs_stream_write(stream, buffer, size) raw_vfs_stream_write(stream, buffer, size)
#define vfs_stream_seek(stream, whence, offset) raw_vfs_stream_seek(stream, whence, offset)
#define vfs_stream_tell(stream, success) raw_vfs_stream_tell(stream, success)
#define vfs_stream_flush(stream) raw_vfs_stream_flush(stream)
#define vfs_stream_close(stream) raw_vfs_stream_close(stream)
#define uid_new() raw_uid_new()
#define tmalloc(size) raw_tmalloc(size)
#define trealloc(ptr, size) raw_trealloc(ptr, size)
#define tfree(ptr) raw_tfree(ptr)
#define get_allocated_memory() raw_get_allocated_memory()
#define mat4_new(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) raw_mat4_new(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)
#define mat4_new_zero() raw_mat4_new_zero()
#define mat4_new_one() raw_mat4_new_one()
#define mat4_from_array(array) raw_mat4_from_array(array)
#define mat4_from_mat4(a) raw_mat4_from_mat4(a)
#define mat4_add(a, b) raw_mat4_add(a, b)
#define mat4_sub(a, b) raw_mat4_sub(a, b)
#define mat4_mul(a, b) raw_mat4_mul(a, b)
#define mat4_scale(a, factor) raw_mat4_scale(a, factor)
#define mat4_transpose(a) raw_mat4_transpose(a)
#define mat4_inverse(a) raw_mat4_inverse(a)
#define vec3_new(x, y, z) raw_vec3_new(x, y, z)
#define vec3_add(a, b) raw_vec3_add(a, b)
#define vec3_sub(a, b) raw_vec3_sub(a, b)
#define vec3_scale(a, factor) raw_vec3_scale(a, factor)
#define vec3_dot(a, b) raw_vec3_dot(a, b)
#define vec3_cross(a, b) raw_vec3_cross(a, b)
#define vec3_length(a) raw_vec3_length(a)
#define vec3_normalize(a) raw_vec3_normalize(a)
#define vec3_distance(a, b) raw_vec3_distance(a, b)
#define vec3_add_in(to, what) raw_vec3_add_in(to, what)
#define vec3_sub_in(from, what) raw_vec3_sub_in(from, what)
#define vec3_scale_in(to, factor) raw_vec3_scale_in(to, factor)
#define vec3_normalize_in(a) raw_vec3_normalize_in(a)
#define auto_free(object) raw_auto_free(object)
#define node_new(name) raw_node_new(name)
#define from_node(node) raw_from_node(node)
#define node_set_name(node, name) raw_node_set_name(node, name)
#define node_get_name(node) raw_node_get_name(node)
#define node_add_child(node, child) raw_node_add_child(node, child)
#define node_remove_child(node, child) raw_node_remove_child(node, child)
#define node_remove_child_by_name(node, name) raw_node_remove_child_by_name(node, name)
#define node_remove_child_by_uid(node, uid) raw_node_remove_child_by_uid(node, uid)
#define node_remove_all_children(node) raw_node_remove_all_children(node)
#define node_get_child_by_name(node, name) raw_node_get_child_by_name(node, name)
#define node_get_child_by_uid(node, uid) raw_node_get_child_by_uid(node, uid)
#define node3d_new() raw_node3d_new()

#endif

