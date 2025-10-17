#pragma once

#include <stddef.h>
#include "platform/fs/fs.h"
#include "types/types.h"
#define VECTOR_NOSTD

// Virtual File Systtem

enum VFSResFileType { VFSFile, VFSDirectory };

struct VFSResFile {
    enum VFSResFileType type;
    char* name;
    union {
        struct {
            usize size; // in bytes
            usize offset; // relative to the start of the real resource file
        } file;
        struct {
            struct VFSResFile* files;
            usize count;
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

/**
 * @brief
 *
 * @api
 */
typedef struct FileStream {
    struct FSFileStream* stream;
    enum VFSStreamType type;
    boolean is_res_scope; // true if it's a res scope, false if it's a usr scope. Used for readonly
                          // guarantee
} FileStream;


void vfs_init(void);

void vfs_exit(void);


/* ================================> Base <================================ */
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
boolean vfs_mount_res(const char* path, const char* mount_point);

/**
 * @brief Unmount Resource File from Virtual File System
 *
 * @param mount_point Mount point
 * @return true
 * @return false
 *
 * @api
 */
boolean vfs_unmount_res(const char* mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res
 *
 * @return true if mounted successfully
 * @return false if mount failed
 *
 * @api
 */
boolean vfs_mount_rfs(const char* mount_point);

/**
 * @brief Mount Real File System to Virtual File System Res only with allowed files in whitelist
 *
 * @return true if mounted successfully
 * @return false if mount failed
 *
 * @api
 */
boolean vfs_mount_rfs_whitelist(const char** whitelist, u64 count, const char* mount_point);

/**
 * @brief Unmount Real File System from Virtual File System Res
 *
 * @return true if unmounted successfully
 * @return false if unmount failed
 *
 * @api
 */
boolean vfs_unmount_rfs(void);

/**
 * @brief Check if file exists
 *
 * @param path
 * @return true if exists
 * @return false if not exists
 *
 * @api
 */
boolean vfs_res_path_exists(const char* path);

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
boolean vfs_usr_path_exists(const char* path, boolean prefer_res);


/* ================================> File Access <================================ */

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
void* vfs_res_read_file(const char* path, u64* size);

/**
 * @brief Open file stream from Resource scope
 *
 * @param path
 * @return FileStream* If success, else NULL
 *
 * @api
 */
FileStream* vfs_res_stream_open(const char* path);

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
void* vfs_usr_read_file(const char* path, u64* size, boolean prefer_res);

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
boolean vfs_usr_write_file(const char* path, const void* data, u64 size);

/**
 * @brief Open file stream from User scope
 *
 * @param path
 * @param prefer_res If true, try to read from Resource scope first
 * @return FileStream* If success, else NULL
 *
 * @api
 */
FileStream* vfs_usr_stream_open(const char* path, boolean prefer_res);


/**
 * @brief Get file stream size in bytes
 *
 * @param path
 * @return u64
 *
 * @api
 */
u64 vfs_stream_size(FileStream* stream);

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
u64 vfs_stream_read_n(FileStream* stream, void* buffer, u64 size);

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
void* vfs_stream_read_all(FileStream* stream, u64* size);

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
u64 vfs_stream_write(FileStream* stream, void* buffer, u64 size);

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
boolean vfs_stream_seek(FileStream* stream, FSSeekFrom whence, u64 offset);

/**
 * @brief Get file stream current position
 *
 * @param stream
 * @param success Can be NULL
 * @return u64
 *
 * @api
 */
u64 vfs_stream_tell(FileStream* stream, boolean* success);

/**
 * @brief Flush file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 * @api
 */
boolean vfs_stream_flush(FileStream* stream);

/**
 * @brief Close file stream
 *
 * @param stream
 * @api
 */
void vfs_stream_close(FileStream* stream);


/* ================================> USR <================================ */
