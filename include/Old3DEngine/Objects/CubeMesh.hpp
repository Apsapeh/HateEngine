#pragma once
#include "Mesh.hpp"

namespace Old3DEngine {
    class CubeMesh : public Mesh {
    public:
        CubeMesh();
        void setSize(float width, float height, float length);
    };
}
