#pragma once

#include "types/types.h"

// API START

typedef c_str Error;

#define ERROR_ASSERT_INFO(error, ...)                                                                   \
    do {                                                                                                \
        if (error != NULL) {                                                                            \
            LOG_INFO(__VA_ARGS__);                                                                      \
        }                                                                                               \
    } while (0)

#define ERROR_ASSERT_WARN(error, ...)                                                                   \
    do {                                                                                                \
        if (error != NULL) {                                                                            \
            LOG_WARN(__VA_ARGS__);                                                                      \
        }                                                                                               \
    } while (0)

#define ERROR_ASSERT_ERROR(error, ...)                                                                  \
    do {                                                                                                \
        if (error != NULL) {                                                                            \
            LOG_ERROR(__VA_ARGS__);                                                                     \
        }                                                                                               \
    } while (0)

#define ERROR_ASSERT_FATAL(error, ...)                                                                  \
    do {                                                                                                \
        if (error != NULL) {                                                                            \
            LOG_FATAL(__VA_ARGS__);                                                                     \
        }                                                                                               \
    } while (0)


#define ERROR_ARG_CHECK(arg)                                                                            \
    do {                                                                                                \
        if (!(arg)) {                                                                                   \
            LOG_ERROR("Invalid argument (is NULL): " #arg);                                             \
            return ERROR_INVALID_ARGUMENT;                                                              \
        }                                                                                               \
    } while (0)

#define ERROR_ARGS_CHECK_1(a) ERROR_ARG_CHECK(a)
#define ERROR_ARGS_CHECK_2(a, b)                                                                        \
    ERROR_ARG_CHECK(a);                                                                                 \
    ERROR_ARG_CHECK(b)
#define ERROR_ARGS_CHECK_3(a, b, c)                                                                     \
    ERROR_ARG_CHECK(a);                                                                                 \
    ERROR_ARG_CHECK(b);                                                                                 \
    ERROR_ARG_CHECK(c)
#define ERROR_ARGS_CHECK_4(a, b, c, d)                                                                  \
    ERROR_ARG_CHECK(a);                                                                                 \
    ERROR_ARG_CHECK(b);                                                                                 \
    ERROR_ARG_CHECK(c);                                                                                 \
    ERROR_ARG_CHECK(d)


#define ERROR_ASSERT(error, ...) ERROR_ASSERT_FATAL(error, __VA_ARGS__)

#define ERROR_SUCCESS ((Error) NULL)

#define ERROR_NOT_IMPLEMENTED "NotImplemented"
#define ERROR_INVALID_ARGUMENT "InvalidArgument"
#define ERROR_ALREADY_EXISTS "AlreadyExists"
#define ERROR_NOT_FOUND "NotFound"
#define ERROR_INVALID_STATE "InvalidState"
#define ERROR_ALLOCATION_FAILED "AllocationFailed"

// API END

/**
 *
 *
 * @api
 */
void set_error(Error err);


/**
 *
 *
 * @api
 */
Error get_error(void);
