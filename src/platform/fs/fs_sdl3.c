#if defined(HE_FS_SDL3)
    #include "fs.h"
    #include <SDL3/SDL.h>
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
        LOG_ERROR("Failed to get base path. SDL Error: %s", SDL_GetError());
    }
    return base_path;
}

const char* fs_get_user_dir(void) {
    // TODO: Impl user org and app details
    const char* pref_path = SDL_GetPrefPath("", "MyApp");
    if (pref_path == NULL) {
        LOG_ERROR("Failed to get pref path. SDL Error: %s", SDL_GetError());
    }
    return pref_path;
}

boolean fs_exists(const char* path) {
    if (path == NULL) {
        return false;
    }

    SDL_PathInfo info;
    if (SDL_GetPathInfo(path, &info) == 0) {
        return true;
    }

    LOG_ERROR("Failed to get path info. SDL Error: %s", SDL_GetError());
    return false;
}

boolean fs_is_dir(const char* path) {
    if (path == NULL) {
        return false;
    }

    SDL_PathInfo info;
    if (SDL_GetPathInfo(path, &info) == 0) {
        return info.type == SDL_PATHTYPE_DIRECTORY;
    }

    LOG_ERROR("Failed to get path info. SDL Error: %s", SDL_GetError());
    return false;
}

u64 fs_get_file_size(const char* path, boolean* success) {
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
            return (u64) info.size;
        } else {
            LOG_ERROR("Path '%s' is not a file", path);
        }
    } else {
        LOG_ERROR("Failed to get path info. SDL Error: %s", SDL_GetError());
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
        LOG_ERROR("Failed to open file '%s' with mode '%s. SDL Error: %s", path, mode, SDL_GetError());
        return NULL;
    }

    FSFileStream* stream = (FSFileStream*) tmalloc(sizeof(FSFileStream));
    stream->io_stream = io_stream;
    stream->path = tmalloc(strlen(path) + 1);
    strcpy(stream->path, path);

    return (FSFileStream*) stream;
}

u64 fs_stream_size(FSFileStream* stream, boolean* success) {
    if (stream == NULL) {
        if (success)
            *success = false;
        return 0;
    }

    Sint64 size = SDL_GetIOSize(stream->io_stream);
    if (size < 0) {
        if (success)
            *success = false;
        LOG_ERROR("Failed to get size of file '%s'. SDL Error: %s", stream->path, SDL_GetError());
        return 0;
    }

    if (success)
        *success = true;
    return (u64) size;
}

u64 fs_stream_read_n(FSFileStream* stream, void* buffer, u64 size) {
    if (stream == NULL || buffer == NULL) {
        return 0;
    }

    u64 bytes_read = SDL_ReadIO(stream->io_stream, buffer, size);
    if (bytes_read == 0) {
        LOG_ERROR("Failed to read from file '%s'. SDL Error: %s", stream->path, SDL_GetError());
    }
    return bytes_read;
}

void* fs_stream_read_all(FSFileStream* stream, u64* size) {
    if (stream == NULL || stream == NULL) {
        if (size)
            *size = 0;
        return NULL;
    }

    boolean success = false;
    u64 curr_pos = fs_stream_tell(stream, &success);
    if (!success) {
        if (size)
            *size = 0;
        return NULL;
    }

    u64 eof = fs_stream_seek(stream, FS_SEEK_FROM_END, 0);
    if (eof == 0) {
        if (size)
            *size = 0;
        return NULL;
    }

    fs_stream_seek(stream, FS_SEEK_FROM_START, curr_pos);
    u64 file_size = eof - curr_pos;

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


u64 fs_stream_write(FSFileStream* stream, void* buffer, u64 size) {
    if (stream == NULL || buffer == NULL) {
        return 0;
    }

    u64 bytes_written = SDL_WriteIO(stream->io_stream, buffer, size);
    if (bytes_written == 0) {
        LOG_ERROR("Failed to write to file '%s'. SDL Error: %s", stream->path, SDL_GetError());
    }
    return bytes_written;
}

u64 fs_stream_seek(FSFileStream* stream, FSSeekFrom from, u64 offset) {
    if (stream == NULL) {
        return false;
    }

    int whence;
    switch (from) {
        case FS_SEEK_FROM_START:
            whence = SDL_IO_SEEK_SET;
            break;
        case FS_SEEK_FROM_CURRENT:
            whence = SDL_IO_SEEK_CUR;
            break;
        case FS_SEEK_FROM_END:
            whence = SDL_IO_SEEK_END;
            break;
        default:
            return false;
    }

    int64_t result = SDL_SeekIO(stream->io_stream, (Sint64) offset, whence);
    if (result == -1) {
        LOG_ERROR("Failed to seek in file '%s'. SDL Error: %s", stream->path, SDL_GetError());
    }
    return result;
}

u64 fs_stream_tell(FSFileStream* stream, boolean* success) {
    if (stream == NULL) {
        if (success)
            *success = false;
        return 0;
    }

    Sint64 position = SDL_TellIO(stream->io_stream);
    if (position < 0) {
        if (success)
            *success = false;
        LOG_ERROR("Failed to get position in file '%s'. SDL Error: %s", stream->path, SDL_GetError());
        return 0;
    }

    if (success)
        *success = true;
    return (u64) position;
}

boolean fs_stream_flush(FSFileStream* stream) {
    if (stream == NULL || stream) {
        return false;
    }

    boolean result = SDL_FlushIO(stream->io_stream);
    if (!result) {
        LOG_ERROR("Failed to flush file '%s'. SDL Error: %s", stream->path, SDL_GetError());
    }
    return result;
}

boolean fs_stream_close(FSFileStream* stream) {
    if (stream == NULL) {
        return false;
    }

    boolean result = SDL_CloseIO(stream->io_stream);
    if (!result) {
        LOG_ERROR("Failed to close file '%s'. SDL Error: %s", stream->path, SDL_GetError());
    }
    tfree(stream->path);
    tfree(stream);
    return result;
}

#endif
