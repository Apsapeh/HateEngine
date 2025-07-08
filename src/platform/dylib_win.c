#if defined(_WIN32)

#include "dylib.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "platform/memory.h"

const char* dylib_extension = ".dll";

dylib_handle* dylib_open(const char* path) {
    char* full_path = tmalloc(strlen(path) + strlen(dylib_extension) + 1);
    strcpy(full_path, path);
    strcat(full_path, dylib_extension);
    dylib_handle* handle = (dylib_handle*) dylib_open_raw(full_path);
    tfree(full_path);
    return handle;
}

dylib_handle* dylib_open_raw(const char* path) {
    void* handle = (dylib_handle*) LoadLibrary(path);
    if (!handle) {
        printf("Failed to load library: %s\n", GetLastError());
        return NULL;
    }
    return (dylib_handle*) handle;
}

void dylib_close(dylib_handle* handle) {
    FreeLibrary((HMODULE) handle);
}

void* dylib_sym(dylib_handle* handle, const char* name) {
    return GetProcAddress((HMODULE) handle, name);
}

char* dylib_error(void) {
    static char buf[512]; // статический буфер, как у dlerror
    DWORD err = GetLastError();
    if (err == 0) {
        return NULL;
    }

    FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, sizeof(buf), NULL
    );
    return buf;
}

#else
char c99_dummy = 0;
#endif
