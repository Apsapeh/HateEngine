#pragma once
#include <vector>
#include <cstdint>
#include <mutex>
#include "../Objects/Mesh.h"
#include "../Utilities/UUID_Generator.h"

namespace Old3DEngine {
    class OpenGL15 {
    public:
        struct OGLObject {
            Mesh *mesh;
            UUID_Generator::UUID id;
            bool is_ref;
        };

        void Draw();
        UUID_Generator::UUID addObjectClone(Mesh mesh);
        UUID_Generator::UUID addObjectRef(Mesh *mesh);
        bool removeObject(UUID_Generator::UUID id);
        void eraseObjects();

    private:
        UUID_Generator uuid_generator;
        std::vector<OGLObject> meshes;
        std::mutex meshesVecMutex;
    };
}