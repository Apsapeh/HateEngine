#pragma once

#include "../Log.hpp"

#define ResultUnwrap(result, var)                                                                       \
    {                                                                                                   \
        if (result.isOk())                                                                              \
            var = result.getOk();                                                                       \
        else                                                                                            \
            HATE_FATAL("Unwrap Err")                                                                    \
    }

#define ResultUnwrapPtr(result, var)                                                                    \
    {                                                                                                   \
        if (result.isOk())                                                                              \
            var = result.getOkPtr();                                                                    \
        else                                                                                            \
            HATE_FATAL("Unwrap Err")                                                                    \
    }

namespace HateEngine {
    template<typename OK, typename ERR>
    class Result {
        union ValueUnion {
            OK ok;
            ERR err;
        };

        bool value_is_ok;
        ValueUnion value;

        Result<OK, ERR>(ValueUnion v, bool is_ok) : value(v), value_is_ok(is_ok) {
        }

    public:
        static Result<OK, ERR> Ok(OK value) {
            Result<OK, ERR> r(ValueUnion{.ok = value}, true);
            return r;
        }
        static Result<OK, ERR> Err(ERR value) {
            Result<OK, ERR> r(ValueUnion{.err = value}, false);
            return r;
        }

        bool isOk() {
            return value_is_ok;
        }

        bool isErr() {
            return not value_is_ok;
        }

        OK getOk() {
            return this->value.ok;
        }

        ERR getErr() {
            return this->value.err;
        }

        OK getOkPtr() {
            return &this->value.ok;
        }

        ERR getErrPtr() {
            return &this->value.err;
        }

        OK unwrap() {
            if (value_is_ok)
                return this->value.ok;

            HATE_FATAL("Unwrap Err")
        }

        OK unwrapPtr() {
            if (value_is_ok)
                return &this->value.ok;

            HATE_FATAL("Unwrap Err")
        }
    };
} // namespace HateEngine
