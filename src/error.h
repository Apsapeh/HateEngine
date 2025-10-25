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


#define ERROR_ARG_CHECK(arg, to_return)                                                                 \
    do {                                                                                                \
        if (!(arg)) {                                                                                   \
            LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (is NULL): " #arg);                                        \
            set_error(ERROR_INVALID_ARGUMENT);                                                          \
            to_return                                                                                   \
        }                                                                                               \
    } while (0)

#define ERROR_ARGS_CHECK_1(a, end_block) ERROR_ARG_CHECK(a, end_block)
#define ERROR_ARGS_CHECK_2(a, b, end_block)                                                             \
    ERROR_ARG_CHECK(a, end_block);                                                                      \
    ERROR_ARG_CHECK(b, end_block)
#define ERROR_ARGS_CHECK_3(a, b, c, end_block)                                                          \
    ERROR_ARG_CHECK(a, end_block);                                                                      \
    ERROR_ARG_CHECK(b, end_block);                                                                      \
    ERROR_ARG_CHECK(c, end_block)
#define ERROR_ARGS_CHECK_4(a, b, c, d, end_block)                                                       \
    ERROR_ARG_CHECK(a, end_block);                                                                      \
    ERROR_ARG_CHECK(b, end_block);                                                                      \
    ERROR_ARG_CHECK(c, end_block);                                                                      \
    ERROR_ARG_CHECK(d, end_block)


#define ERROR_ASSERT(error, ...) ERROR_ASSERT_FATAL(error, __VA_ARGS__)

#define ERROR_SUCCESS ((Error) NULL)

#define ERROR_NOT_IMPLEMENTED "NotImplemented"
#define ERROR_INVALID_ARGUMENT "InvalidArgument"
#define ERROR_ALREADY_EXISTS "AlreadyExists"
#define ERROR_NOT_FOUND "NotFound"
#define ERROR_INVALID_STATE "InvalidState"
#define ERROR_ALLOCATION_FAILED "AllocationFailed"

// API END

boolean __error_check_ptr_arg(void* ptr, c_str name);

/**
 * @brief Set a last error that occurred on the current thread.
 *
 * @api
 */
void set_error(Error err);


/**
 * @brief Retrieve a last error that occurred on the current thread.
 *
 * @api
 */
Error get_error(void);
