#pragma once
#include <chrono>
#include <cstdint>

namespace Old3DEngine {
    class UUID_Generator {
    private:
        uint64_t prevTime = 0;
        uint32_t idTimeAdder = 0;

    public:
        struct UUID {
            uint64_t body;
            uint32_t counter;

            bool operator==(UUID other_id) const {
                return (other_id.body == body) and (other_id.counter == counter);
            }
        };

        UUID gen();
    };
}