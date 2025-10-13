#pragma once

#include <stddef.h>
#include <types/types.h>

typedef struct FSFileStream FSFileStream;


/*
API ENUM {
        "name": "FSSeekFrom",
        "type": "char",
        "values": [
                ["Start", "'s'"],
                ["Current", "'c'"],
                ["End", "'e'"]
        ]
}
*/

#define FS_SEEK_FROM_START 's'
#define FS_SEEK_FROM_CURRENT 'c'
#define FS_SEEK_FROM_END 'e'

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
 * @brief Return path to directory containing the executable (in most OSes)
 *
 * @return const char* if success, else NULL
 */
const char* fs_get_data_dir(void);

/**
 * @brief Return path to directory containing the user data
 *
 * @return const char* if success, else NULL
 */
const char* fs_get_user_dir(void);

/**
 * @brief Check if file exists
 *
 * @param path
 * @return true if exists
 * @return false if not exists
 */
boolean fs_exists(const char* path);

/**
 * @brief Check if path is a directory
 *
 * @param path
 * @return true if directory
 * @return false if not directory
 */
boolean fs_is_dir(const char* path);


/**
 * @brief Get file size
 *
 * @param path
 * @param success true if success, else false. Can be NULL
 * @return u64 Can bo 0 if failed, check success parameter
 */
u64 fs_get_file_size(const char* path, boolean* success);

/**
 * @brief Open file stream
 *
 * @param path
 * @param mode
 * @return FSFileStream*
 */
FSFileStream* fs_stream_open(const char* path, const char* mode);

/**
 * @brief Get file stream size
 *
 * @param stream
 * @param success Can be NULL
 * @return u64
 */
u64 fs_stream_size(FSFileStream* stream, boolean* success);

/**
 * @brief Read from file stream n bytes to buffer
 *
 * @param stream
 * @param buffer
 * @param size
 * @return u64
 */
u64 fs_stream_read_n(FSFileStream* stream, void* buffer, u64 size);

/**
 * @brief Allocate buffer and read all file to it
 *
 * @warning You must free returned buffer
 * @param stream
 * @param size
 * @return void*
 */
void* fs_stream_read_all(FSFileStream* stream, u64* size);

/**
 * @brief Write to file stream n bytes from buffer
 *
 * @param stream
 * @param buffer
 * @param size
 *
 * @return u64 Number of bytes written. 0 if fail
 */
u64 fs_stream_write(FSFileStream* stream, void* buffer, u64 size);

/**
 * @brief Seek in file stream
 *
 * @param stream
 * @param from
 * @param offset
 * @return final offset
 */
u64 fs_stream_seek(FSFileStream* stream, FSSeekFrom from, u64 offset);

/**
 * @brief Get current position in file stream
 *
 * @param stream
 * @param success Can be NULL
 * @return u64
 */
u64 fs_stream_tell(FSFileStream* stream, boolean* success);

/**
 * @brief Flush any buffered data to the stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 */
boolean fs_stream_flush(FSFileStream* stream);

/**
 * @brief Close file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 */
boolean fs_stream_close(FSFileStream* stream);
