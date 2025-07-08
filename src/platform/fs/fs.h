#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct FSFileStream FSFileStream;

// [[API Generator: forward]]
enum FSSeekFrom { FSSeekFromStart, FSSeekFromCurrent, FSSeekFromEnd };

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
bool fs_exists(const char* path);

/**
 * @brief Check if path is a directory
 *
 * @param path
 * @return true if directory
 * @return false if not directory
 */
bool fs_is_dir(const char* path);


/**
 * @brief Get file size
 *
 * @param path
 * @param success true if success, else false. Can be NULL
 * @return size_t Can bo 0 if failed, check success parameter
 */
size_t fs_get_file_size(const char* path, bool* success);

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
 * @return size_t
 */
size_t fs_stream_size(FSFileStream* stream, bool* success);

/**
 * @brief Read from file stream n bytes to buffer
 *
 * @param stream
 * @param buffer
 * @param size
 * @return size_t
 */
size_t fs_stream_read_n(FSFileStream* stream, void* buffer, size_t size);

/**
 * @brief Allocate buffer and read all file to it
 *
 * @warning You must free returned buffer
 * @param stream
 * @param size
 * @return void*
 */
void* fs_stream_read_all(FSFileStream* stream, size_t* size);

/**
 * @brief Write to file stream n bytes from buffer
 *
 * @param stream
 * @param buffer
 * @param size
 *
 * @return size_t Number of bytes written. 0 if fail
 */
size_t fs_stream_write(FSFileStream* stream, void* buffer, size_t size);

/**
 * @brief Seek in file stream
 *
 * @param stream
 * @param from
 * @param offset
 * @return true if success
 * @return false if fail
 */
bool fs_stream_seek(FSFileStream* stream, enum FSSeekFrom from, size_t offset);

/**
 * @brief Get current position in file stream
 *
 * @param stream
 * @param success Can be NULL
 * @return size_t
 */
size_t fs_stream_tell(FSFileStream* stream, bool* success);

/**
 * @brief Flush any buffered data to the stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 */
bool fs_stream_flush(FSFileStream* stream);

/**
 * @brief Close file stream
 *
 * @param stream
 * @return true if success
 * @return false if fail
 */
bool fs_stream_close(FSFileStream* stream);
