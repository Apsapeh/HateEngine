#pragma once

#include "error.h"
#include "../log.h"

// Макрос для объявления именованного типа Result
#define DECLARE_RESULT(name, T)                                                                         \
    typedef struct {                                                                                    \
        union {                                                                                         \
            T ok;                                                                                       \
            Error err;                                                                                  \
        } value;                                                                                        \
    } Result_##name

// Макросы для именованных типов
#define OK(val, ResultType)                                                                             \
    ((Result_##ResultType) {.value.ok = (val), .value.err = {ErrorDomainNoError, 0}})

#define ERR(domain, error_code, ResultType)                                                             \
    ((Result_##ResultType) {.value.err = {(domain), (error_code)}})

// Проверка успешности результата
#define IS_OK(result) ((result).value.err.domain == ErrorDomainNoError)
#define ASSERT_OK(result)                                                                               \
    {                                                                                                   \
        if (!IS_OK(result)) {                                                                           \
            LOG_FATAL("Assertion failed: %s", #result);                                                 \
        }                                                                                               \
    }
#define IS_ERR(result) ((result).value.err.domain != ErrorDomainNoError)

// Извлечение значения (небезопасно - нужно проверить IS_OK перед использованием)
#define UNWRAP(result) ((result).value.ok)
#define UNWRAP_ERR(result) ((result).value.err)

// Безопасное извлечение с значением по умолчанию
#define UNWRAP_OR(result, default_val) (IS_OK(result) ? UNWRAP(result) : (default_val))

// Макрос для удобной работы с Result
#define MATCH(result, ok_block, err_block)                                                              \
    do {                                                                                                \
        if (IS_OK(result)) {                                                                            \
            ok_block                                                                                    \
        } else {                                                                                        \
            err_block                                                                                   \
        }                                                                                               \
    } while (0)


// DECLARE_RESULT(Int, const char*);
// DECLARE_RESULT(ConstCharPtr, const char*);
DECLARE_RESULT(ConstCharPtr, const char*);
