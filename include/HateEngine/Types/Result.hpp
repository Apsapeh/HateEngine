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
    // template<typename OK, typename ERR>
    // class Result {
    //     union ValueUnion {
    //         OK ok;
    //         ERR err;

    //         ValueUnion() {};
    //         ~ValueUnion() {};
    //     };

    //     bool value_is_ok;
    //     ValueUnion value;

    //     Result<OK, ERR>(ValueUnion v, bool is_ok) : value(std::move(v)), value_is_ok(is_ok) {
    //     }

    // public:
    //     static Result<OK, ERR> Ok(OK value) {
    //         Result<OK, ERR> r(ValueUnion{.ok = std::move(value)}, true);
    //         return r;
    //     }
    //     static Result<OK, ERR> Err(ERR value) {
    //         Result<OK, ERR> r(ValueUnion{.err = std::move(value)}, false);
    //         return r;
    //     }

    //     ~Result() {
    //             if (value_is_ok) {
    //                 value.ok.~T();
    //             } else {
    //                 value.err.~E();
    //             }
    //         }

    //     bool isOk() {
    //         return value_is_ok;
    //     }

    //     bool isErr() {
    //         return not value_is_ok;
    //     }

    //     OK getOk() {
    //         return this->value.ok;
    //     }

    //     ERR getErr() {
    //         return this->value.err;
    //     }

    //     OK getOkPtr() {
    //         return &this->value.ok;
    //     }

    //     ERR getErrPtr() {
    //         return &this->value.err;
    //     }

    //     OK unwrap() {
    //         if (value_is_ok)
    //             return this->value.ok;

    //         HATE_FATAL("Unwrap Err")
    //     }

    //     OK unwrapPtr() {
    //         if (value_is_ok)
    //             return &this->value.ok;

    //         HATE_FATAL("Unwrap Err")
    //     }
    // };
    //

    template<typename T, typename E>
    class Result {
        union {
            T ok;
            E err;
        };
        bool is_ok;

        Result(T value, int) : is_ok(true) {
            new (&ok) T(std::move(value));
        }

        Result(E error, int*) : is_ok(false) {
            new (&err) E(std::move(error));
        }

    public:
        // Constructors
        static Result Ok(T value) {
            return Result(std::move(value), 0);
        }

        static Result Err(E error) {
            return Result(std::move(error), nullptr);
        }

        // Destructor
        ~Result() {
            if (is_ok)
                ok.~T();
            else
                err.~E();
        }

        // Copy constructor
        Result(const Result& other) : is_ok(other.is_ok) {
            if (is_ok)
                new (&ok) T(other.ok);
            else
                new (&err) E(other.err);
        }

        // Move constructor
        Result(Result&& other) noexcept : is_ok(other.is_ok) {
            if (is_ok)
                new (&ok) T(std::move(other.ok));
            else
                new (&err) E(std::move(other.err));
        }

        // Copy assignment
        Result& operator=(const Result& other) {
            if (this != &other) {
                this->~Result();
                is_ok = other.is_ok;
                if (is_ok)
                    new (&ok) T(other.ok);
                else
                    new (&err) E(other.err);
            }
            return *this;
        }

        // Move assignment
        Result& operator=(Result&& other) noexcept {
            if (this != &other) {
                this->~Result();
                is_ok = other.is_ok;
                if (is_ok)
                    new (&ok) T(std::move(other.ok));
                else
                    new (&err) E(std::move(other.err));
            }
            return *this;
        }

        // Accessors
        bool isOk() const {
            return is_ok;
        }
        bool isErr() const {
            return !is_ok;
        }

        T& unwrap() {
            return ok;
        }
        const T& unwrap() const {
            return ok;
        }

        E& unwrapErr() {
            return err;
        }
        const E& unwrapErr() const {
            return err;
        }

        T takeOk() {
            if (!is_ok)
                HATE_FATAL("Called takeOk() on Err");
            T temp = std::move(ok);
            ok.~T(); // вручную уничтожаем, чтобы избежать двойного вызова в деструкторе
            is_ok = false; // помечаем, что в union больше нет активного члена
            return temp;
        }

        E takeErr() {
            if (!is_ok)
                HATE_FATAL("Called takeErr() on Ok");
            E temp = std::move(err);
            err.~E(); // вручную уничтожаем, чтобы избежать двойного вызова в деструкторе
            is_ok = true; // помечаем, что в union больше нет активного члена
            return temp;
        }
    };

    template<typename T, typename E>
    inline Result<T, E> Ok(T value) {
        return Result<T, E>::Ok(std::move(value));
    }

    template<typename T, typename E>
    inline Result<T, E> Err(E error) {
        return Result<T, E>::Err(std::move(error));
    }
} // namespace HateEngine
