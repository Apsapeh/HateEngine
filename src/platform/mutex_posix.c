#if defined(__linux__) || defined(__gnu_linux__)
    #ifndef _GNU_SOURCE
        #define _GNU_SOURCE
    #endif
#elif defined(__unix__) || defined(__APPLE__)
    #ifndef _XOPEN_SOURCE
        #define _XOPEN_SOURCE 700
    #endif
#endif

#include "platform.h"

#if defined(PLATFORM_UNIX)
    #include "mutex.h"
    #include "memory.h"
    #include <pthread.h>
    #include <errno.h>
    #include <assert.h>
    #include "log.h"

struct mutex_handle {
    pthread_mutex_t mutex;
};

    #define HANDLE_CHECK(return_value)                                                                  \
        do {                                                                                            \
            if (!handle) {                                                                              \
                LOG_ERROR_OR_DEBUG_FATAL_NO_ALLOC("Invalid argument (is NULL): handle");                \
                return return_value;                                                                    \
            }                                                                                           \
        } while (0);

mutex_handle mutex_new(void) {
    mutex_handle handle = (mutex_handle) tmalloc(sizeof(struct mutex_handle));
    HANDLE_CHECK(NULL);

    // Initialize with default attributes
    int code = pthread_mutex_init(&handle->mutex, NULL);
    if (code != 0) {
        tfree(handle);
        LOG_ERROR_NO_ALLOC("Mutex can't be initialized with Pthread code: %d");
        return NULL;
    }

    return handle;
}

mutex_handle mutex_new_recursive(void) {
    mutex_handle handle = (mutex_handle) tmalloc(sizeof(struct mutex_handle));
    HANDLE_CHECK(NULL);

    // Setup recursive mutex attributes
    pthread_mutexattr_t attr;
    int code = pthread_mutexattr_init(&attr);
    if (code != 0) {
        tfree(handle);
        LOG_ERROR_NO_ALLOC("Mutex attributes can't be initialized with Pthread code: %d");
        return NULL;
    }

    code = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    if (code != 0) {
        pthread_mutexattr_destroy(&attr);
        tfree(handle);
        LOG_ERROR_NO_ALLOC("Mutex attributes can't be setted with Pthread code: %d");
        return NULL;
    }

    // Initialize with recursive attributes
    code = pthread_mutex_init(&handle->mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    if (code != 0) {
        tfree(handle);
        LOG_ERROR_NO_ALLOC("Mutex can't be initialized with Pthread code: %d");
        return NULL;
    }

    return handle;
}

void mutex_free(mutex_handle handle) {
    HANDLE_CHECK();

    int code = pthread_mutex_destroy(&handle->mutex);
    if (code == EBUSY) {
        LOG_ERROR_OR_DEBUG_FATAL_NO_ALLOC("Mutex can't be freed - is locked");
    }

    tfree(handle);
}

void mutex_lock(mutex_handle handle) {
    HANDLE_CHECK();

    int code = pthread_mutex_lock(&handle->mutex);
    if (code != 0) {
        LOG_ERROR_NO_ALLOC("Mutex lock failed with Pthread error code: %d", code);
    }
}

boolean mutex_try_lock(mutex_handle handle) {
    HANDLE_CHECK(false);

    int code = pthread_mutex_trylock(&handle->mutex);

    if (code == 0) {
        return true; // successfully locked
    } else if (code == EBUSY) {
        return false; // already locked
    } else {
        // Unexpected error
        LOG_ERROR_NO_ALLOC("Mutex try lock failed with Pthread error code: %d", code);
        return false;
    }
}

void mutex_unlock(mutex_handle handle) {
    HANDLE_CHECK();

    int code = pthread_mutex_unlock(&handle->mutex);
    if (code == EPERM) {
        LOG_ERROR_OR_DEBUG_FATAL_NO_ALLOC("Mutex can't be unlocked - is locked by other thread");
    } else if (code != 0) {
        LOG_ERROR("Mutex lock failed with Pthread error code: %d", code);
    }
}


#else
static char c99_dummy = 0;
#endif
