#pragma once
#include <unordered_set>

namespace HateEngine {
    template<typename... Args>
    class Signal {
        std::unordered_set<void (*)(Args...)> listeners;

    public:
        void connect(void (*func)(Args...)) {
            listeners.insert(func);
        }

        void disconnect(void (*func)(Args...)) {
            listeners.erase(func);
        }

        void emit(Args... args) {
            for (auto func: listeners) {
                func(args...);
            }
        }
    };
} // namespace HateEngine
