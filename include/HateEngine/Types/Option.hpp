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

    template<typename T>
    class Option {
        union {
            T some;
            bool none_placeholder; // Placeholder for T without default constructor
        };
        bool is_some;

        Option(T value) : is_some(true) {
            new (&some) T(std::move(value));
        }

        Option() : is_some(false), none_placeholder(false) {
        }

    public:
        // Constructors
        static Option Some(T value) {
            return Option(std::move(value));
        }

        static Option None() {
            return Option();
        }

        // Destructor
        ~Option() {
            if (is_some)
                some.~T();
        }

        // Copy constructor
        Option(const Option& other) : is_some(other.is_some) {
            if (is_some)
                new (&some) T(other.some);
            else
                none_placeholder = false;
        }

        // Move constructor
        Option(Option&& other) noexcept : is_some(other.is_some) {
            if (is_some)
                new (&some) T(std::move(other.some));
            else
                none_placeholder = false;
        }

        // Copy assignment
        Option& operator=(const Option& other) {
            if (this != &other) {
                this->~Result();
                is_some = other.is_some;
                if (is_some)
                    new (&some) T(other.some);
                else
                    none_placeholder = false;
            }
            return *this;
        }

        // Move assignment
        Option& operator=(Option&& other) noexcept {
            if (this != &other) {
                this->~Result();
                is_some = other.is_some;
                if (is_some)
                    new (&some) T(std::move(other.some));
                else
                    none_placeholder = false;
            }
            return *this;
        }

        // Accessors
        bool isSome() const {
            return is_some;
        }
        bool isNone() const {
            return !is_some;
        }

        T& unwrap() {
            return some;
        }
        const T& unwrap() const {
            return some;
        }

        T takeSome() {
            if (!is_some)
                HATE_FATAL("Called takeSome() on None");
            T temp = std::move(some);
            some.~T(); // вручную уничтожаем, чтобы избежать двойного вызова в деструкторе
            is_some = false; // помечаем, что в union больше нет активного члена
            return temp;
        }
    };

    template<typename T>
    inline Option<T> Some(T value) {
        return Option<T>::Some(std::move(value));
    }

    template<typename T>
    inline Option<T> None() {
        return Option<T>::None();
    }
} // namespace HateEngine
