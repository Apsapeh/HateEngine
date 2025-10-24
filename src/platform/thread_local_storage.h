#pragma once

#include <types/types.h>

#ifndef HE_NO_NATIVE_TLS // Try to use native __thread or something like that

    #if defined(_MSC_VER) || defined(__GNUC__) || defined(__clang__)
        #if defined(_MSC_VER)
            #define THREAD_LOCAL __declspec(thread)
        #else
            #define THREAD_LOCAL __thread
        #endif

        #define thread_local_storage_impl(type, name)                                                   \
            static THREAD_LOCAL type name;                                                              \
            static THREAD_LOCAL boolean name##IsInited = false;                                         \
            static void name##_try_init(type def) {                                                     \
                if (!name##IsInited) {                                                                  \
                    name = def;                                                                         \
                    name##IsInited = true;                                                              \
                }                                                                                       \
            }                                                                                           \
            static void name##_set_value(type value) {                                                  \
                name = value;                                                                           \
                name##IsInited = true;                                                                  \
            }                                                                                           \
            static type name##_get_value(void) {                                                        \
                return name;                                                                            \
            }                                                                                           \
            static type* name##_get_value_ptr(void) {                                                   \
                return &name;                                                                           \
            }

    #endif

#else // Use custom slow, but cross platform implementation
    #error HE_NO_NATIVE_TLS yet not supported. Will be implemented in the future
#endif
