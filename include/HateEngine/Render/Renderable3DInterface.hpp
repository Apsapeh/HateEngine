#pragma once

//#include "OpenGL15.hpp"
#include "../Objects/Mesh.hpp"

namespace HateEngine {
    class Renderable3DInterface {
    public:
        virtual std::vector<Mesh*> render(class OpenGL15* renderer) = 0;
        //virtual ~RenderableInterface() {}
    };
}