#include "vfs.h"
#include "extc/extc_vec.h"
#include "platform/fs/fs.h"
#include "platform/memory.h"
#include "log.h"
#include <stddef.h>
#include <string.h>

#define VECTOR_MALLOC(size) tmalloc(size)
#define VECTOR_REALLOC(ptr, size) trealloc(ptr, size)
#define VECTOR_FREE(ptr) tfree(ptr)
#define VECTOR_MEMMOVE(dest, src, size) memmove(dest, src, size)
#define VECTOR_MEMCPY(dest, src, size) memcpy(dest, src, size)

vector_template_def(vfs_mnt, struct VFSMnt);
vector_template_impl(vfs_mnt, struct VFSMnt);

static vec_vfs_mnt mnts;
static struct VFSRFSMnt rfs_mnt;

void vfs_init(void) {
    mnts = vec_vfs_mnt_init();
    rfs_mnt.path = NULL;
}

bool vfs_mount_res(const char* path, const char* mount_point) {
    // TODO: impl
    return true;
}

bool vfs_unmount_res(const char* path, const char* mount_point) {
    // TODO: impl
    return true;
}

bool vfs_mount_rfs(const char* mount_point) {
    if (mount_point == NULL) {
        HATE_ERROR("Mount point cannot be null");
        return false;
    }

    if (rfs_mnt.path != NULL) {
        HATE_ERROR("RFS is already mounted");
        return false;
    }

    const char* base_path = fs_get_data_dir();
    if (base_path == NULL) {
        HATE_ERROR("Unable to get base path");
        return false;
    }

    rfs_mnt.whitelist = NULL;
    rfs_mnt.name = NULL;

    rfs_mnt.path = tmalloc(strlen(base_path) + 1);
    strcpy(rfs_mnt.path, base_path);

    if (mount_point[0] != '/') {
        HATE_WARN("Mount point does not start with '/': %s", mount_point);
        rfs_mnt.mount_point = tmalloc(strlen(mount_point) + 2);
        strcpy(&rfs_mnt.mount_point[1], mount_point);
        rfs_mnt.mount_point[0] = '/';
    } else {
        rfs_mnt.mount_point = tmalloc(strlen(mount_point) + 1);
        strcpy(rfs_mnt.mount_point, mount_point);
    }

    return true;
}

bool vfs_mount_rfs_whitelist(const char** whitelist, size_t count, const char* mount_point) {
    if (mount_point == NULL) {
        HATE_ERROR("Mount point cannot be null");
        return false;
    }

    if (whitelist == NULL || count == 0) {
        HATE_ERROR("Whitelist cannot be null or empty");
        return false;
    }

    if (rfs_mnt.path != NULL) {
        HATE_ERROR("RFS is already mounted");
        return false;
    }

    if (!vfs_mount_rfs(mount_point)) {
        return false;
    }

    rfs_mnt.whitelist = tmalloc((count + 1) * sizeof(const char*));
    // Deep copy
    for (size_t i = 0; i < count; i++) {
        rfs_mnt.whitelist[i] = tmalloc(strlen(whitelist[i]) + 1);
        strcpy(rfs_mnt.whitelist[i], whitelist[i]);
    }
    rfs_mnt.whitelist[count] = NULL; // NULL terminate
    return true;
}

bool vfs_unmount_rfs(void) {
    if (rfs_mnt.path != NULL) {
        tfree(rfs_mnt.path);
        tfree(rfs_mnt.mount_point);
        if (rfs_mnt.whitelist != NULL) {
            for (char** ptr = rfs_mnt.whitelist; *ptr != NULL; ++ptr) {
                tfree(*ptr);
            }
            tfree(rfs_mnt.whitelist);
        }
        rfs_mnt.path = NULL;
        return true;
    }
    return false;
}


static void free_split_path(char** parts) {
    if (!parts)
        return;
    for (size_t i = 0; parts[i] != NULL; ++i) {
        tfree(parts[i]);
    }
    tfree(parts);
}

// split_path: разбивает и нормализует путь
// - Удаляет "." компоненты
// - Обрабатывает ".." как возврат к родительскому каталогу
// - Требует, чтобы путь начинался с "/"
static char** split_path(const char* path) {
    if (!path || path[0] != '/') {
        // Ошибка: путь должен начинаться с /
        return NULL;
    }

    size_t capacity = 8;
    size_t count = 0;
    char** stack = tmalloc(capacity * sizeof(char*));
    if (!stack)
        return NULL;

    const char* start = path + 1; // Пропускаем начальный '/'
    while (*start) {
        // Пропуск лишних слэшей
        while (*start == '/')
            ++start;
        if (*start == '\0')
            break;

        const char* end = start;
        while (*end && *end != '/')
            ++end;

        size_t len = (size_t) (end - start);
        if (len == 0) {
            start = end;
            continue;
        }

        // Выделяем под компонент
        char* component = tmalloc(len + 1);
        if (!component) {
            free_split_path(stack);
            return NULL;
        }
        memcpy(component, start, len);
        component[len] = '\0';

        // Обработка специальных компонентов
        if (strcmp(component, ".") == 0) {
            tfree(component); // Пропускаем
        } else if (strcmp(component, "..") == 0) {
            tfree(component);
            if (count > 0) {
                tfree(stack[--count]); // Убираем последний компонент
            }
        } else {
            // Обычный компонент — добавляем
            if (count + 1 >= capacity) {
                capacity *= 2;
                char** new_stack = trealloc(stack, capacity * sizeof(char*));
                if (!new_stack) {
                    tfree(component);
                    free_split_path(stack);
                    return NULL;
                }
                stack = new_stack;
            }
            stack[count++] = component;
        }

        start = end;
    }

    // NULL-терминируем
    stack[count] = NULL;
    return stack;
}

static bool vfs_path_starts_with(const char* path, const char* mount_point) {
    size_t mp_len = strlen(mount_point);
    if (strncmp(path, mount_point, mp_len) != 0)
        return false;

    // root mount always matches
    if (strcmp(mount_point, "/") == 0)
        return true;

    // if exact match
    if (path[mp_len] == '\0')
        return true;

    // allow only if followed by '/'
    return path[mp_len] == '/' || mount_point[mp_len + 1] == '/';
}

static struct VFSResFile* vfs_res_find_file(const char* path) {
    for (size_t mnt_i = mnts.size; mnt_i > 0; mnt_i--) {
        struct VFSMnt* mnt = &mnts.data[mnt_i - 1];
        struct VFSResFile* current_dir = &mnt->root;

        if (!vfs_path_starts_with(path, mnt->mount_point))
            continue;

        char** parts = split_path(path + strlen(mnt->mount_point));
        if (!parts)
            continue;

        for (char** part = parts; *part != NULL; part++) {
            bool dir_found = false;
            for (size_t i = 0; i < current_dir->info.dir.count; i++) {
                struct VFSResFile* file = &current_dir->info.dir.files[i];
                if (strcmp(file->name, *part) == 0) {
                    // If file found
                    if (file->type == VFSFile && *(part + 1) == NULL) {
                        free_split_path(parts);
                        return file;
                    } else if (file->type == VFSFile) { // File found but not the last component
                        goto out_parts_for;
                    }

                    current_dir = file;
                    dir_found = true;
                    break;
                }
            }

            if (!dir_found)
                break;
        }

    out_parts_for:
        free_split_path(parts);
    }

    return NULL;
}


void* vfs_res_read_file(const char* path, size_t* size) {
    // Check path
    char** _parts = split_path(path);
    if (!_parts) {
        HATE_ERROR("Invalid path: %s", path);
        return NULL;
    }

    struct VFSResFile* file = vfs_res_find_file(path);
    if (file) {
        // TODO: impl read
        HATE_FATAL("TODO: vfs_res_read_file");
    }

    if (rfs_mnt.path != NULL) {
        size_t parts_len = 0;
        for (char** part = _parts; *part != NULL; part++)
            parts_len += strlen(*part) + 1;

        char* full_path = tmalloc(parts_len);
        strcpy(full_path, rfs_mnt.path);
        for (char** part = _parts; *part != NULL; part++) {
            strcat(full_path, *part);
            if (*(part + 1) != NULL) {
                strcat(full_path, "/");
            }
        }

        FSFileStream* stream = fs_stream_open(path, "rb");
        if (stream) {
            void* data = fs_stream_read_all(stream, size);
            fs_stream_close(stream);
            free_split_path(_parts);
            return data;
        }
    }

    free_split_path(_parts);
    return NULL;
}


FileStream* vfs_res_stream_open(const char* path) {
}


void* vfs_usr_read_file(const char* path, size_t* size, bool prefer_res) {
}


FileStream* vfs_usr_stream_open(const char* path, bool prefer_res) {
}


size_t vfs_stream_size(FileStream* stream) {
}


size_t vfs_stream_read_n(FileStream* stream, void* buffer, size_t size) {
}


void* vfs_stream_read_all(FileStream* stream, size_t* size) {
}


size_t vfs_stream_write(FileStream* stream, void* buffer, size_t size) {
}


bool vfs_stream_seek(FileStream* stream, enum FSSeekFrom whence, size_t offset) {
}


size_t vfs_stream_tell(FileStream* stream, bool* success) {
}


bool vfs_stream_flush(FileStream* stream) {
}


void vfs_stream_close(FileStream* stream) {
}
