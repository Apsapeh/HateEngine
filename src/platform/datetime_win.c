/*
    It's GPT generated code, in future, i maybe update this code
*/

#include "platform.h"

#ifdef PLATFORM_WINDOWS

// datetime_windows.c
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Подключаем ваш заголовок; если у вас другое место - подставьте путь.
#include "datetime.h"
#include <types/types.h>

typedef enum {
    METHOD_PRECISE_FILETIME, // GetSystemTimePreciseAsFileTime
    METHOD_QPC, // QueryPerformanceCounter interpolation
    METHOD_TICK64, // GetTickCount64
    METHOD_TICK32, // GetTickCount or timeGetTime
    METHOD_SYSTEMFILETIME // GetSystemTimeAsFileTime (fallback)
} time_method_t;

struct datetime_handle {
    time_method_t method;

    // baseline FILETIME in 100-ns units
    unsigned long long base_filetime100ns; // FILETIME as 64-bit (100ns units)

    // for QPC
    LARGE_INTEGER qpc_freq;
    LARGE_INTEGER qpc_base;

    // for tick
    unsigned long long tick_base_ms; // milliseconds base
    int tick_is_64;

    // function pointers (dynamically loaded)
    VOID(WINAPI* pGetSystemTimePreciseAsFileTime)(LPFILETIME);
    BOOL(WINAPI* pQueryPerformanceCounter)(LARGE_INTEGER*);
    BOOL(WINAPI* pQueryPerformanceFrequency)(LARGE_INTEGER*);
    ULONGLONG(WINAPI* pGetTickCount64)(void);
    DWORD(WINAPI* pGetTickCount)(void);
};

static unsigned long long filetime_to_u64(const FILETIME* ft) {
    return (((unsigned long long) ft->dwHighDateTime) << 32) | ft->dwLowDateTime;
}

static void u64_to_filetime(unsigned long long v, FILETIME* ft) {
    ft->dwLowDateTime = (DWORD) (v & 0xFFFFFFFFULL);
    ft->dwHighDateTime = (DWORD) (v >> 32);
}

// Получить текущее FILETIME (UTC) как 64-бит 100-ns единицы,
// используя выбранный метод (с учётом интерполяции).
static unsigned long long get_current_filetime100ns(struct datetime_handle* h) {
    FILETIME ft;
    unsigned long long now100ns = 0;

    if (h->pGetSystemTimePreciseAsFileTime) {
        // наилучший путь
        h->pGetSystemTimePreciseAsFileTime(&ft);
        now100ns = filetime_to_u64(&ft);
        return now100ns;
    }

    switch (h->method) {
        case METHOD_QPC: {
            LARGE_INTEGER now;
            if (h->pQueryPerformanceCounter(&now) && h->qpc_freq.QuadPart > 0) {
                long long delta = now.QuadPart - h->qpc_base.QuadPart;
                unsigned long long freq = (unsigned long long) h->qpc_freq.QuadPart;
                unsigned long long d = (unsigned long long) (delta < 0 ? -delta : delta);

                unsigned long long elapsed100ns =
                        (d / freq) * 10000000ULL + (d % freq) * 10000000ULL / freq;

                if (delta < 0 && elapsed100ns) {
                    elapsed100ns = (unsigned long long) (-(long long) elapsed100ns);
                }

                now100ns = h->base_filetime100ns + elapsed100ns;
                return now100ns;
            }
            // fallback to system API
            GetSystemTimeAsFileTime(&ft);
            return filetime_to_u64(&ft);
        }
        case METHOD_TICK64: {
            // tick_base_ms and GetTickCount64
            if (h->pGetTickCount64) {
                unsigned long long now_ms = h->pGetTickCount64();
                unsigned long long delta_ms = now_ms - h->tick_base_ms;
                now100ns = h->base_filetime100ns + delta_ms * 10000ULL; // 1 ms = 10,000 * 100ns
                return now100ns;
            }
            // fallthrough
        }
        case METHOD_TICK32: {
            DWORD now32 = h->pGetTickCount ? h->pGetTickCount() : GetTickCount();
            unsigned long long now_ms = (unsigned long long) now32;
            unsigned long long delta_ms;
            if (h->tick_is_64) {
                // improbable path; but keep safe
                delta_ms = now_ms - h->tick_base_ms;
            } else {
                // handle 32-bit wrap: tick_base_ms stored as lower 32 bits
                unsigned long long base32 = (unsigned long long) (DWORD) h->tick_base_ms;
                delta_ms = (unsigned long long) ((DWORD) now_ms - (DWORD) base32);
            }
            now100ns = h->base_filetime100ns + delta_ms * 10000ULL;
            return now100ns;
        }
        default:
            // simplest fallback
            GetSystemTimeAsFileTime(&ft);
            return filetime_to_u64(&ft);
    }
}

// Вспомог: инициализация доступных функций и baseline
datetime_handle datetime_new(void) {
    struct datetime_handle* h = (struct datetime_handle*) malloc(sizeof(*h));
    if (!h)
        return NULL;
    memset(h, 0, sizeof(*h));

    HMODULE k32 = GetModuleHandleA("kernel32.dll");
    HMODULE winmm = NULL;

    // Try to load function pointers
    if (k32) {
        // GetSystemTimePreciseAsFileTime (Windows 8+)
        h->pGetSystemTimePreciseAsFileTime =
                (void(WINAPI*)(LPFILETIME)) GetProcAddress(k32, "GetSystemTimePreciseAsFileTime");
        h->pQueryPerformanceCounter =
                (BOOL(WINAPI*)(LARGE_INTEGER*)) GetProcAddress(k32, "QueryPerformanceCounter");
        h->pQueryPerformanceFrequency =
                (BOOL(WINAPI*)(LARGE_INTEGER*)) GetProcAddress(k32, "QueryPerformanceFrequency");
        h->pGetTickCount64 = (ULONGLONG(WINAPI*)(void)) GetProcAddress(k32, "GetTickCount64");
        h->pGetTickCount = (DWORD(WINAPI*)(void)) GetProcAddress(k32, "GetTickCount");
    } else {
        h->pGetSystemTimePreciseAsFileTime = NULL;
        h->pQueryPerformanceCounter = NULL;
        h->pQueryPerformanceFrequency = NULL;
        h->pGetTickCount64 = NULL;
        h->pGetTickCount = NULL;
    }

    // baseline FILETIME always from GetSystemTimeAsFileTime (present everywhere)
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    h->base_filetime100ns = filetime_to_u64(&ft);

    // Try to configure most precise available method
    if (h->pGetSystemTimePreciseAsFileTime) {
        h->method = METHOD_PRECISE_FILETIME;
        // nothing else needed
    } else if (h->pQueryPerformanceCounter && h->pQueryPerformanceFrequency) {
        // Try to initialize QPC baseline
        LARGE_INTEGER freq, now;
        if (h->pQueryPerformanceFrequency(&freq) && freq.QuadPart > 0) {
            if (h->pQueryPerformanceCounter(&now)) {
                h->method = METHOD_QPC;
                h->qpc_freq = freq;
                h->qpc_base = now;
                // base_filetime already set above (system time at this moment)
            } else {
                // can't read QPC now; fallback to ticks
                h->pQueryPerformanceCounter = NULL;
                h->method = METHOD_TICK64; // try tick64 etc below
            }
        } else {
            h->method = METHOD_TICK64;
        }
    } else {
        // no QPC; try tick64 or tick32/timeGetTime
        if (h->pGetTickCount64) {
            h->method = METHOD_TICK64;
            h->tick_base_ms = h->pGetTickCount64();
            h->tick_is_64 = 1;
        } else if (h->pGetTickCount) {
            h->method = METHOD_TICK32;
            h->tick_base_ms = (unsigned long long) h->pGetTickCount();
            h->tick_is_64 = 0;
        } else {
            // some very old systems may not have GetTickCount exported? fallback
            // try timeGetTime from winmm
            winmm = LoadLibraryA("winmm.dll");
            if (winmm) {
                FARPROC p = GetProcAddress(winmm, "timeGetTime");
                if (p) {
                    // timeGetTime has same semantics as GetTickCount
                    h->pGetTickCount = (DWORD(WINAPI*)(void)) p;
                    h->method = METHOD_TICK32;
                    h->tick_base_ms = (unsigned long long) h->pGetTickCount();
                    h->tick_is_64 = 0;
                } else {
                    h->method = METHOD_SYSTEMFILETIME;
                }
            } else {
                h->method = METHOD_SYSTEMFILETIME;
            }
        }
    }

    return (datetime_handle) h;
}

void datetime_free(datetime_handle handler) {
    if (!handler)
        return;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    free(h);
}

void datetime_update(datetime_handle handler) {
    // in this implementation we don't store a cached SystemTime struct,
    // getters will compute from baseline+elapsed when called.
    (void) handler; // no-op (kept for API compatibility)
}

// Helper: internal get current FILETIME and convert to SYSTEMTIME (UTC)
static void get_systemtime_from_handle_utc(struct datetime_handle* h, SYSTEMTIME* st) {
    unsigned long long now100ns = get_current_filetime100ns(h);
    FILETIME ft;
    u64_to_filetime(now100ns, &ft);
    FileTimeToSystemTime(&ft, st);
}

// Helper: get local SYSTEMTIME
static void get_systemtime_from_handle_local(struct datetime_handle* h, SYSTEMTIME* st_local) {
    unsigned long long now100ns = get_current_filetime100ns(h);
    FILETIME ft_utc, ft_local;
    u64_to_filetime(now100ns, &ft_utc);
    // convert to local FILETIME (works on all Win32)
    if (!FileTimeToLocalFileTime(&ft_utc, &ft_local)) {
        // fallback: use utc directly
        ft_local = ft_utc;
    }
    FileTimeToSystemTime(&ft_local, st_local);
}

// Get local fields
u16 datetime_get_year(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_local(h, &st);
    return (u16) st.wYear;
}

u8 datetime_get_month(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_local(h, &st);
    return (u8) st.wMonth;
}

u8 datetime_get_day(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_local(h, &st);
    return (u8) st.wDay;
}

u8 datetime_get_hour(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_local(h, &st);
    return (u8) st.wHour;
}

u8 datetime_get_minute(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_local(h, &st);
    return (u8) st.wMinute;
}

u8 datetime_get_second(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_local(h, &st);
    return (u8) st.wSecond;
}

u32 datetime_get_nanosecond(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    unsigned long long now100ns = get_current_filetime100ns(h);
    // nanoseconds within current second:
    // filetime is 100-ns ticks since 1601. Compute remainder modulo 10^7 (100ns units per second)
    unsigned long long ticks100ns_in_sec = now100ns % 10000000ULL; // 10 million * 100ns = 1 second
    unsigned long long ns = ticks100ns_in_sec * 100ULL; // convert 100ns units to ns
    return (u32) ns; // fits into u32 (<1e9)
}

/* ========== UTC getters ========== */

u16 datetime_get_utc_year(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_utc(h, &st);
    return (u16) st.wYear;
}

u8 datetime_get_utc_month(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_utc(h, &st);
    return (u8) st.wMonth;
}

u8 datetime_get_utc_day(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_utc(h, &st);
    return (u8) st.wDay;
}

u8 datetime_get_utc_hour(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_utc(h, &st);
    return (u8) st.wHour;
}

u8 datetime_get_utc_minute(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_utc(h, &st);
    return (u8) st.wMinute;
}

u8 datetime_get_utc_second(datetime_handle handler) {
    if (!handler)
        return 0;
    struct datetime_handle* h = (struct datetime_handle*) handler;
    SYSTEMTIME st;
    get_systemtime_from_handle_utc(h, &st);
    return (u8) st.wSecond;
}


#else
static char c99_dummy = 0;
#endif
