#pragma once

// #include "OpenGL13.hpp"

namespace HateEngine {
    class UpdatableInterface {
    public:
        virtual void update(double delta) = 0;
    };
} // namespace HateEngine
