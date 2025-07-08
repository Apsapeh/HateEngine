#include "fs.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <string.h>

#include "platform/memory.h"
#include "log.h"

struct FSFileStream {
    SDL_IOStream* io_stream;
    char* path;
};

const char* fs_get_data_dir(void) {
    const char* base_path = SDL_GetBasePath();
    if (base_path == NULL) {
        HATE_ERROR("Failed to get base path. SDL Error: %s", SDL_GetError());
    }
    return base_path;
}

const char* fs_get_user_dir(void) {
    // TODO: Impl user org and app details
    const char* pref_path = SDL_GetPrefPath("", "MyApp");
    if (pref_path == NULL) {
        HATE_ERROR("Failed to get pref path. SDL Error: %s", SDL_GetError());
    }
    return pref_path;
}

bool fs_exists(const char* path) {
    if (path == NULL) {
        return false;
    }

    SDL_PathInfo info;
    if (SDL_GetPathInfo(path, &info) == 0) {
        return true;
    }

    HATE_ERROR("Failed to get path info. SDL Error: %s", SDL_GetError());
    return false;
}

bool fs_is_dir(const char* path) {
    if (path == NULL) {
        return false;
    }

    SDL_PathInfo info;
    if (SDL_GetPathInfo(path, &info) == 0) {
        return info.type == SDL_PATHTYPE_DIRECTORY;
    }

    HATE_ERROR("Failed to get path info. SDL Error: %s", SDL_GetError());
    return false;
}

size_t fs_get_file_size(const char* path, bool* success) {
    if (path == NULL) {
        if (success)
            *success = false;
        return 0;
    }

    SDL_PathInfo info;
    if (SDL_GetPathInfo(path, &info) == 0) {
        if (info.type == SDL_PATHTYPE_FILE) {
            if (success)
                *success = true;
            return (size_t) info.size;
        } else {
            HATE_ERROR("Path '%s' is not a file", path);
        }
    } else {
        HATE_ERROR("Failed to get path info. SDL Error: %s", SDL_GetError());
    }

    if (success)
        *success = false;
    return 0;
}

FSFileStream* fs_stream_open(const char* path, const char* mode) {
    if (path == NULL || mode == NULL) {
        return NULL;
    }

    SDL_IOStream* io_stream = SDL_IOFromFile(path, mode);
    if (io_stream == NULL) {
        HATE_ERROR("Failed to open file '%s' with mode '%s. SDL Error: %s", path, mode, SDL_GetError());
        return NULL;
    }

    FSFileStream* stream = (FSFileStream*) tmalloc(sizeof(FSFileStream));
    stream->io_stream = io_stream;
    stream->path = tmalloc(strlen(path) + 1);
    strcpy(stream->path, path);

    return (FSFileStream*) io_stream;
}

size_t fs_stream_size(FSFileStream* stream, bool* success) {
    if (stream == NULL) {
        if (success)
            *success = false;
        return 0;
    }

    Sint64 size = SDL_GetIOSize(stream->io_stream);
    if (size < 0) {
        if (success)
            *success = false;
        HATE_ERROR("Failed to get size of file '%s'. SDL Error: %s", stream->path, SDL_GetError());
        return 0;
    }

    if (success)
        *success = true;
    return (size_t) size;
}

size_t fs_stream_read_n(FSFileStream* stream, void* buffer, size_t size) {
    if (stream == NULL || buffer == NULL) {
        return 0;
    }

    size_t bytes_read = SDL_ReadIO(stream->io_stream, buffer, size);
    if (bytes_read == 0) {
        HATE_ERROR("Failed to read from file '%s'. SDL Error: %s", stream->path, SDL_GetError());
    }
    return bytes_read;
}

void* fs_stream_read_all(FSFileStream* stream, size_t* size) {
    if (stream == NULL || stream == NULL) {
        if (size)
            *size = 0;
        return NULL;
    }

    bool success = false;
    size_t curr_pos = fs_stream_tell(stream, &success);
    if (!success) {
        if (size)
            *size = 0;
        return NULL;
    }

    size_t eof = fs_stream_seek(stream, FSSeekFromEnd, 0);
    if (eof == 0) {
        if (size)
            *size = 0;
        return NULL;
    }

    fs_stream_seek(stream, FSSeekFromStart, curr_pos);
    size_t file_size = eof - curr_pos;

    if (file_size == 0) {
        if (size)
            *size = 0;
        return NULL;
    }

    void* buffer = tmalloc(file_size);
    fs_stream_read_n(stream, buffer, file_size);
    if (size)
        *size = file_size;

    return buffer;
}


size_t fs_stream_write(FSFileStream* stream, void* buffer, size_t size) {
    if (stream == NULL || buffer == NULL) {
        return 0;
    }

    size_t bytes_written = SDL_WriteIO(stream->io_stream, buffer, size);
    if (bytes_written == 0) {
        HATE_ERROR("Failed to write to file '%s'. SDL Error: %s", stream->path, SDL_GetError());
    }
    return bytes_written;
}

bool fs_stream_seek(FSFileStream* stream, enum FSSeekFrom from, size_t offset) {
    if (stream == NULL) {
        return false;
    }

    int whence;
    switch (from) {
        case FSSeekFromStart:
            whence = SDL_IO_SEEK_SET;
            break;
        case FSSeekFromCurrent:
            whence = SDL_IO_SEEK_CUR;
            break;
        case FSSeekFromEnd:
            whence = SDL_IO_SEEK_END;
            break;
        default:
            return false;
    }

    bool result = SDL_SeekIO(stream->io_stream, (Sint64) offset, whence);
    if (!result) {
        HATE_ERROR("Failed to seek in file '%s'. SDL Error: %s", stream->path, SDL_GetError());
    }
    return result;
}

size_t fs_stream_tell(FSFileStream* stream, bool* success) {
    if (stream == NULL) {
        if (success)
            *success = false;
        return 0;
    }

    Sint64 position = SDL_TellIO(stream->io_stream);
    if (position < 0) {
        if (success)
            *success = false;
        HATE_ERROR("Failed to get position in file '%s'. SDL Error: %s", stream->path, SDL_GetError());
        return 0;
    }

    if (success)
        *success = true;
    return (size_t) position;
}

bool fs_stream_flush(FSFileStream* stream) {
    if (stream == NULL || stream) {
        return false;
    }

    bool result = SDL_FlushIO(stream->io_stream);
    if (!result) {
        HATE_ERROR("Failed to flush file '%s'. SDL Error: %s", stream->path, SDL_GetError());
    }
    return result;
}

bool fs_stream_close(FSFileStream* stream) {
    if (stream == NULL) {
        return false;
    }

    bool result = SDL_CloseIO(stream->io_stream);
    if (!result) {
        HATE_ERROR("Failed to close file '%s'. SDL Error: %s", stream->path, SDL_GetError());
    }
    tfree(stream->path);
    tfree(stream);
    return result;
}
