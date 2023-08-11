#pragma once
#include <cstdint>
#include "Object.h"

namespace Old3DEngine {
    // Object -> Mesh
    class Mesh : public Object {
    public:
        float *verticies = nullptr;
        uint32_t *indicies = nullptr;
        float *normals = nullptr;
        uint32_t verticesSize;
        uint32_t indiciesSize;

        ~Mesh();
    };
}