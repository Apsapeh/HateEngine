// FIXME: Rewrite AI Slop
/*
    It's AI Slop, in future I maybe update this code
*/

#include "platform.h"

#if defined(PLATFORM_WINDOWS)
    #include "mutex.h"
    #include "memory.h"
    #include "log.h"
    #include <windows.h>

// Windows CRITICAL_SECTION is recursive by default
struct mutex_handle {
    CRITICAL_SECTION cs;
    boolean is_recursive;
};

    #define HANDLE_CHECK(return_value)                                                                  \
        do {                                                                                            \
            if (!handle) {                                                                              \
                LOG_ERROR_OR_DEBUG_FATAL_NO_ALLOC("Mutex handle is NULL");                              \
                return return_value;                                                                    \
            }                                                                                           \
        } while (0);

mutex_handle mutex_new(void) {
    return mutex_new_recursive();
}

mutex_handle mutex_new_recursive(void) {
    mutex_handle handle = (mutex_handle) tmalloc(sizeof(struct mutex_handle));
    if (!handle) {
        LOG_ERROR_OR_DEBUG_FATAL_NO_ALLOC("Mutex memory can't be allocated");
        return NULL;
    }

    handle->is_recursive = true;

    // CRITICAL_SECTION is already recursive by default on Windows
    InitializeCriticalSection(&handle->cs);
    return handle;
}

void mutex_free(mutex_handle handle) {
    HANDLE_CHECK();

    // DeleteCriticalSection doesn't return error code
    // It will crash if mutex is locked - this is expected behavior per API contract
    DeleteCriticalSection(&handle->cs);
    tfree(handle);
}

void mutex_lock(mutex_handle handle) {
    HANDLE_CHECK();

    // EnterCriticalSection doesn't fail (except in catastrophic situations)
    EnterCriticalSection(&handle->cs);
}

boolean mutex_try_lock(mutex_handle handle) {
    HANDLE_CHECK(false);

    #if defined(_WIN32_WINNT) && _WIN32_WINNT >= 0x0500 // Windows 2000+
    BOOL result = TryEnterCriticalSection(&handle->cs);
    return result != 0;
    #else
    // Win95/98/NT4 - нет try_lock
    LOG_ERROR_NO_ALLOC("mutex_try_lock not supported on Windows 95/98/NT4");
    return false;
    #endif
}

void mutex_unlock(mutex_handle handle) {
    HANDLE_CHECK();

    // LeaveCriticalSection doesn't return error code
    // It will crash if not locked by current thread - this is expected
    LeaveCriticalSection(&handle->cs);
}

#else
static char c99_dummy = 0;
#endif
