#pragma once
#include <stdint.h>

const uint8_t HEFS_MAGIC[4] = {'H', 'E', 'F', 'S'};

const uint32_t HEFS_CURRENT_VERSION = 0x1000;

/*
 * ru:
 * Сначала идёт заголовок файла, который содержит информацию о версии, флагах и размере файла.
 * Затем идёт дерево файлов, которое содержит информацию о каждом файле в директории.
 * Работает рекурсивно в глубину: выход из директории это HEFS_FILE_FLAG_NONE | HEFS_FILE_FLAG_IS_DIR
 * В конце идёт данные файлов, которые содержат информацию о содержимом каждого файла.
 * В начале может идти хэш, если установлен флаг HEFS_FILE_FLAG_HAS_HASH
 */


/* ======================== Version 0x1000 ========================= */

// Compression method - LZ4
// Encryption method - хз
// Hash method - xxHash64

enum HEFSFileFlags {
    HEFS_FILE_FLAG_NONE = 1 << 0,
    HEFS_FILE_FLAG_COMPRESSED = 1 << 1,
    HEFS_FILE_FLAG_ENCRYPTED = 1 << 2,
    HEFS_FILE_FLAG_HAS_HASH = 1 << 3,
    HEFS_FILE_FLAG_IS_DIR = 1 << 7,
};

struct HEFSFile_HeaderSpec_v1 {
    uint8_t magic[4];
    uint32_t version;
    uint32_t flags;
};

struct HEFSFile_TreeNodeSpec_v1 {
    uint8_t flags;
    char* filename; // null terminated
    uint64_t data_offset;
    uint64_t data_size;
};
