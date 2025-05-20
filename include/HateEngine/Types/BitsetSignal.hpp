#pragma once

#include "Bitset.hpp"
#include "../Utilities/Signal.hpp"

namespace HateEngine {
    template<typename T, typename... S>
    class BitsetSignal : public Bitset<T> {

    public:
        Signal<S...> onChanged;
        BitsetSignal(T data) : Bitset<T>(data) {
        }

        inline void set(int index, bool value) override {
            Bitset<T>::set(index, value);
            onChanged.emit();
        }

        inline void setRaw(T data) override {
            Bitset<T>::setRaw(data);
            onChanged.emit();
        }

        inline void clear() override {
            Bitset<T>::clear();
            onChanged.emit();
        }
    };

} // namespace HateEngine
