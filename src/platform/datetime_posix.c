// clang-format off

#include "platform.h"

#ifdef PLATFORM_UNIX
#define _POSIX_C_SOURCE 199309L

#include <time.h>
#include <unistd.h>
#include "platform/datetime.h"
#include <platform/memory.h>

#ifndef HE_USE_DATETIME_POSIX_RT
    #ifdef __APPLE__
        #include <Availability.h>
        #if defined(__MAC_10_12) && \
            (__MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_12)
            #define HE_USE_DATETIME_POSIX_RT
        #endif
    #elif defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
        #define HE_USE_DATETIME_POSIX_RT
    #elif defined(_POSIX_VERSION) && (_POSIX_VERSION >= 199309L)
        #define HE_USE_DATETIME_POSIX_RT
    #endif
#endif

//#undef HE_USE_DATETIME_POSIX_RT

#define ___COMPILE_DATETIME
#ifdef HE_USE_DATETIME_POSIX_RT
    struct datetime_handle {
        struct timespec ts;
        struct tm local_tm;
        struct tm utc_tm;
    };
    
    void datetime_update(datetime_handle handle) {
        if (!handle) {
            return;
        }
        
        // Get current time with nanosecond precision
        clock_gettime(CLOCK_REALTIME, &handle->ts);
        
        // Convert to local time
        time_t sec = handle->ts.tv_sec;
        localtime_r(&sec, &handle->local_tm);
        
        // Convert to UTC time
        gmtime_r(&sec, &handle->utc_tm);
    }
    
    u32 datetime_get_nanosecond(datetime_handle handle) {
        if (!handle) {
            return 0;
        }
        return (u32)(handle->ts.tv_nsec);
    }
    
#else
    #include <sys/time.h>

    struct datetime_handle {
        struct timeval tv;
        struct tm local_tm;
        struct tm utc_tm;
    };
    
    void datetime_update(datetime_handle handle) {
        if (!handle) {
            return;
        }
        // Get current time with microsecond precision
        gettimeofday(&handle->tv, NULL);
    
        // Convert to local time
        time_t sec = handle->tv.tv_sec;
        localtime_r(&sec, &handle->local_tm);
        
        // Convert to UTC time
        gmtime_r(&sec, &handle->utc_tm);
    }
    
    u32 datetime_get_nanosecond(datetime_handle handle) {
        if (!handle) {
            return 0;
        }
        // Convert microseconds to nanoseconds
        return (u32)(handle->tv.tv_usec * 1000);
    }
#endif


datetime_handle datetime_new(void) {
    datetime_handle handle = (datetime_handle)tmalloc(sizeof(struct datetime_handle));
    if (!handle) {
        return NULL;
    }
    
    datetime_update(handle);
    return handle;
}

void datetime_free(datetime_handle handle) {
    if (handle) {
        tfree(handle);
    }
}

u16 datetime_get_year(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u16)(handle->local_tm.tm_year + 1900);
}

u8 datetime_get_month(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u8)(handle->local_tm.tm_mon + 1);
}

u8 datetime_get_day(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u8)handle->local_tm.tm_mday;
}

u8 datetime_get_hour(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u8)handle->local_tm.tm_hour;
}

u8 datetime_get_minute(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u8)handle->local_tm.tm_min;
}

u8 datetime_get_second(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u8)handle->local_tm.tm_sec;
}

/* ======================> UTC <====================== */

u16 datetime_get_utc_year(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u16)(handle->utc_tm.tm_year + 1900);
}

u8 datetime_get_utc_month(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u8)(handle->utc_tm.tm_mon + 1);
}

u8 datetime_get_utc_day(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u8)handle->utc_tm.tm_mday;
}

u8 datetime_get_utc_hour(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u8)handle->utc_tm.tm_hour;
}

u8 datetime_get_utc_minute(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u8)handle->utc_tm.tm_min;
}

u8 datetime_get_utc_second(datetime_handle handle) {
    if (!handle) {
        return 0;
    }
    return (u8)handle->utc_tm.tm_sec;
}


#else
static char c99_dummy = 0;
#endif

// clang-format on
