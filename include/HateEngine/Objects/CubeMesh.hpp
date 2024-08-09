#pragma once
#include "Mesh.hpp"

namespace HateEngine {
    class CubeMesh : public Mesh {
    public:
        CubeMesh();
        void setSize(float width, float height, float length);
    };
} // namespace HateEngine
