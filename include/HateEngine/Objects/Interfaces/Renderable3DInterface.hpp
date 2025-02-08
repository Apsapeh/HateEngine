#pragma once

// #include "OpenGL13.hpp"

#include "../../Render/RenderInterface.hpp"
namespace HateEngine {
    class Renderable3DInterface {
    public:
        virtual void render(RenderInterface* renderer) = 0;
        // virtual ~RenderableInterface() {}
        virtual bool isCorrectTransparency() {
            return false;
        };
    };
} // namespace HateEngine
