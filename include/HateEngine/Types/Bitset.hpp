#pragma once

namespace HateEngine {
    template<typename T>
    class Bitset {
    protected:
        T data;

    public:
        Bitset(T data) : data(data) {
        }

        virtual inline void set(int index, bool value) {
            data = (data & ~(1 << index)) | (value << index);
        }

        inline bool get(int index) const {
            return (data >> index) & 1;
        }

        virtual inline void setRaw(T data) {
            this->data = data;
        }

        inline T getRaw() const {
            return this->data;
        }

        virtual inline void clear() {
            this->data = 0;
        }

        inline bool operator[](int index) {
            return this->get(index);
        }
    };

} // namespace HateEngine
