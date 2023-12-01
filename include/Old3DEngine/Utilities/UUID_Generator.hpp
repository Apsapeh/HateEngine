#pragma once
#include <chrono>
#include <cstdint>
#include <mutex>
#include <functional>

namespace Old3DEngine {
    class UUID_Generator {
    private:
        uint64_t prevTime = 0;
        uint32_t idTimeAdder = 0;
        std::mutex getMutex;

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

// Hash function for UUID
template <> struct std::hash<Old3DEngine::UUID_Generator::UUID>
{
    std::size_t operator()(const Old3DEngine::UUID_Generator::UUID& k) const
    {
        using std::size_t;
        using std::hash;
        using std::string;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:

        return (hash<uint64_t>()(k.body) ^ (hash<uint32_t>()(k.counter) << 1)) >> 1;
    }
};