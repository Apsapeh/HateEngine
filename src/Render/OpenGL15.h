#pragma once
#include <vector>
#include <cstdint>
#include "../Utilities/UUID_Generator.h"

namespace Old3DEngine {
    class OpenGL15 {
    public:
        struct OGLObject {
            float *buffer;
            uint32_t buffer_size;
            UUID_Generator::UUID id;
        };

        void Draw();
        UUID_Generator::UUID addObject(float *buffer, uint32_t buffer_size);
        bool removeObject(UUID_Generator::UUID id);

    public:
        UUID_Generator uuid_generator;
        std::vector<OGLObject> objects;
    };
}