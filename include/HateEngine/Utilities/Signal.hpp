#pragma once
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include "UUID.hpp"

namespace HateEngine {
    template<typename... Args>
    class Signal {

        std::unordered_map<UUID, std::function<void(Args...)>> listeners;

    public:
        UUID connect(std::function<void(Args...)> func) {
            UUID id;
            listeners[id] = func;
            return id;
        }

        void disconnect(UUID uuid) {
            listeners.erase(uuid);
        }

        void emit(Args... args) {
            for (auto& func: listeners) {
                func.second(args...);
            }
        }
    };
} // namespace HateEngine
