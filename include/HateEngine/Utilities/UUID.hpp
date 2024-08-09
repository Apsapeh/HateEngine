#pragma once
#include <chrono>
#include <cstdint>
#include <functional>
#include <mutex>
#include <string>

namespace HateEngine {
    class UUID {
        friend struct std::hash<UUID>;

    private:
        static uint64_t counter;
        static std::mutex mutex;
        uint64_t id;

    public:
        UUID();
        UUID(const uint64_t& id);

        const uint64_t getU64() const;

        bool operator==(const UUID& other) const;
    };
} // namespace HateEngine

// Hash function for UUID
template<>
struct std::hash<HateEngine::UUID> {
    std::size_t operator()(const HateEngine::UUID& k) const {
        return std::hash<uint64_t>()(k.id);
    }
};
