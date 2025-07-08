#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "platform/fs/fs.h"
#define VECTOR_NOSTD

// Virtual File Systtem

enum VFSResFileType { VFSFile, VFSDirectory };

struct VFSResFile {
    enum VFSResFileType type;
    char* name;
    union {
        struct {
            size_t size; // in bytes
            size_t offset; // relative to the start of the real resource file
        } file;
        struct {
            struct VFSResFile* files;
            size_t count;
        } dir;
    } info;
};

enum VFSMntType { VFSMntTypeRFS, VFSMntTypeRFSWhitelist, VFSMntTypeResourceFile };

struct VFSMnt {
    char* name;
    char* path; // path to data dir if it's a RFS, path to the real file if it's a ResourceFile
    char* mount_point;
    struct VFSResFile root; // root of the mnt
};

struct VFSRFSMnt {
    char* name;
    char* path; // path to data dir if it's a RFS, path to the real file if it's a ResourceFile
    char* mount_point;
    char** whitelist; // NULL terminated
};


enum VFSStreamType { VFSStreamTypeRFS, VFSStreamTypeResourceFile };

// [[API Generator]]
typedef struct FileStream {
    struct FSFileStream* stream;
    enum VFSStreamType type;
    bool is_res; // true if it's a res scope, false if it's a usr scope. Used for readonly guarantee
} FileStream;


void vfs_init(void);


/* ================================> Base <================================ */
// [[API Generator]]
bool vfs_mount_res(const char* path, const char* mount_point);

// [[API Generator]]
bool vfs_unmount_res(const char* path, const char* mount_point);

// [[API Generator]]
/**
 * @brief Mount Real File System to Virtual File System Res
 *
 * @return true if mounted successfully
 * @return false if mount failed
 */
bool vfs_mount_rfs(const char* mount_point);

// [[API Generator]]
/**
 * @brief Mount Real File System to Virtual File System Res only with allowed files in whitelist
 *
 * @return true if mounted successfully
 * @return false if mount failed
 */
bool vfs_mount_rfs_whitelist(const char** whitelist, size_t count, const char* mount_point);

// [[API Generator]]
/**
 * @brief Unmount Real File System from Virtual File System Res
 *
 * @return true if unmounted successfully
 * @return false if unmount failed
 */
bool vfs_unmount_rfs(void);

// [[API Generator]]
/**
 * @brief Check if file exists
 *
 * @param path
 * @return true if exists
 * @return false if not exists
 */
bool vfs_res_path_exists(const char* path);

// [[API Generator]]
/**
 * @brief Check if file exists in User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return true if exists
 * @return false if not exists
 */
bool vfs_usr_path_exists(const char* path, bool prefer_res);


/* ================================> File Access <================================ */

// [[API Generator]]
/**
 * @brief Allocate memory and read file from Reosource scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @return Pointer to allocated memory with file data if success, else NULL
 */
void* vfs_res_read_file(const char* path, size_t* size);

// [[API Generator]]
/**
 * @brief Open file stream from Resource scope
 *
 * @param path
 * @return FileStream* If success, else NULL
 */
FileStream* vfs_res_stream_open(const char* path);

// [[API Generator]]
/**
 * @brief Allocate memory and read file from User scope
 * @warning You must free the memory
 *
 * @param path
 * @param size Size of file. Can be NULL
 * @param prefer_res If true, try to read from Resource scope first
 * @return Pointer to allocated memory with file data if success, else NULL
 */
void* vfs_usr_read_file(const char* path, size_t* size, bool prefer_res);

// [[API Generator]]
/**
 * @brief Write file to User scope
 *
 * @param path
 * @param data
 * @param size
 * @return true if success
 * @return false if fail
 */
bool vfs_usr_write_file(const char* path, const void* data, size_t size);

// [[API Generator]]
/**
 * @brief Open file stream from User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return FileStream* If success, else NULL
 */
FileStream* vfs_usr_stream_open(const char* path, bool prefer_res);


// [[API Generator]]
/**
 * @brief Get file stream size in bytes
 *
 * @param path
 * @return size_t
 */
size_t vfs_stream_size(FileStream* stream);

// [[API Generator]]
/**
 * @brief Read from file stream n bytes to buffer
 *
 * @param stream
 * @param buffer
 * @param size
 * @return size_t
 */
size_t vfs_stream_read_n(FileStream* stream, void* buffer, size_t size);

// [[API Generator]]
/**
 * @brief Allocate buffer and read all file to it
 *
 * @warning You must free returned buffer
 * @param stream
 * @param size
 * @return void*
 */
void* vfs_stream_read_all(FileStream* stream, size_t* size);

// [[API Generator]]
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
size_t vfs_stream_write(FileStream* stream, void* buffer, size_t size);

// [[API Generator]]
/**
 * @brief Seek file stream
 *
 * @param stream
 * @param whence
 * @param offset
 * @return true if success
 * @return false if fail
 */
bool vfs_stream_seek(FileStream* stream, enum FSSeekFrom whence, size_t offset);

// [[API Generator]]
/**
 * @brief Get file stream current position
 *
 * @param stream
 * @param success Can be NULL
 * @return size_t
 */
size_t vfs_stream_tell(FileStream* stream, bool* success);

// [[API Generator]]
/**
 * @brief Flush file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 */
bool vfs_stream_flush(FileStream* stream);

// [[API Generator]]
/**
 * @brief Close file stream
 *
 * @param stream
 */
void vfs_stream_close(FileStream* stream);


/* ================================> USR <================================ */
