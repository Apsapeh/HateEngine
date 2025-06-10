#pragma once

// #include "OpenGL13.hpp"
#include "../../InputEvent.hpp"

namespace HateEngine {
    class UpdatableInterface {
    public:
        virtual void _process(class Engine* engino, double delta) {
        }
        virtual void _fixedProcess(class Engine* engino, double delta) {
        }
        virtual void _inputEvent(class Engine* engino, const InputEventInfo& event) {
        }
    };
} // namespace HateEngine
