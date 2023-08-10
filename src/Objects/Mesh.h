#pragma once
#include <cstdint>
#include "Object.h"

namespace Old3DEngine {
    // Object -> Mesh
    class Mesh : public Object {
    public:
        float *verticies;
        uint32_t *indicies;
        float *normals;
        uint32_t verticesSize;
        uint32_t indiciesSize;

    };
}