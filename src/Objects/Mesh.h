#pragma once
#include <cstdint>
#include <vector>
#include "Object.h"

namespace Old3DEngine {
    // Object -> Mesh
    class Mesh : public Object {
    public:
        std::vector<float> verticies;
        std::vector<uint32_t> indicies ;
        std::vector<float> normals;

        ~Mesh();
    };
}