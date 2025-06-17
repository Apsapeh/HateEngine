#pragma once

#include <glm/glm.hpp>
#include "InputKeys.hpp"

namespace HateEngine {
    enum InputEventType {
        InputEventKey,
        InputEventMouseButton,
        InputEventMouseMove,
        InputEventMouseScroll,
        InputEventGamepadButton,
        InputEventGamepadAxis,
    };

    struct InputEventInfo {
        InputEventType type;
        union {
            Key key;
            MouseButton button;
            GamepadButtons gamepadButton;
            GamepadAxis gamepadAxis;
        };
        bool isPressed = false;
        union {
            glm::vec2 offset;
            glm::vec2 position;
            float value;
        };
    };
} // namespace HateEngine
