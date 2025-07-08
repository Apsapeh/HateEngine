#if defined(__unix__) || defined(__APPLE__)

#include "dylib.h"
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include "platform/memory.h"

#if defined(__APPLE__)
const char* dylib_extension = ".dylib";
#else
const char* dylib_extension = ".so";
#endif


dylib_handle* dylib_open(const char* path) {
    char* full_path = tmalloc(strlen(path) + strlen(dylib_extension) + 1);
    strcpy(full_path, path);
    strcat(full_path, dylib_extension);
    dylib_handle* handle = dylib_open_raw(full_path);
    tfree(full_path);
    return handle;
}

dylib_handle* dylib_open_raw(const char* path) {
    void* handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        printf("Failed to load library: %s\n", dlerror());
        return NULL;
    }
    return (dylib_handle*) handle;
}

void dylib_close(dylib_handle* handle) {
    dlclose(handle);
}

void* dylib_sym(dylib_handle* handle, const char* name) {
    return dlsym(handle, name);
}

char* dylib_error(void) {
    return dlerror();
}

#endif
