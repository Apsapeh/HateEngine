#pragma once

#include <glm/glm.hpp>
#include "InputKeys.hpp"

namespace HateEngine {
    enum InputEventType {
        InputEventKey,
        InputEventMouseButton,
        InputEventMouseMove,
        InputEventMouseScroll
    };

    struct InputEventInfo {
        InputEventType type;
        union {
            Key key;
            MouseButton button;
        };
        bool isPressed = false;
        glm::vec2 offset = {0, 0};
        glm::vec2 position = {0, 0};
    };
} // namespace HateEngine
