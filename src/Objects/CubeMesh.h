#pragma once
#include "Mesh.h"

namespace Old3DEngine {
    class CubeMesh : public Mesh {
    public:
        CubeMesh();
        void setSize(float width, float height, float length);
    };
}
