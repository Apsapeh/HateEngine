#pragma once
#include <types/types.h>

const u8 HEFS_MAGIC[4] = {'H', 'E', 'F', 'S'};

const u32 HEFS_CURRENT_VERSION = 0x1000;

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
    u8 magic[4];
    u32 version;
    u32 flags;
};

struct HEFSFile_TreeNodeSpec_v1 {
    u8 flags;
    str filename; // null terminated
    u64 data_offset;
    u64 data_size;
};
