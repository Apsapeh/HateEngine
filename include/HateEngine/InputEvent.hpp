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
        glm::vec2 offset = glm::vec2(0.0f);
        glm::vec2 position = glm::vec2(0.0f);
        float value = 0.0f;
    };
} // namespace HateEngine
