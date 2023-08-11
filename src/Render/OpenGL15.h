#pragma once
#include <vector>
#include <cstdint>
#include <mutex>
#include "../Old3DEngine.h"
#include "../Objects/Mesh.h"
#include "../Utilities/UUID_Generator.h"

namespace Old3DEngine {
    class OpenGL15 {
    public:
        OpenGL15(std::vector<Engine::SceneObject>*);
        void Draw();
        void eraseObjects();

    private:
        std::vector<Engine::SceneObject> *meshes;
        std::mutex meshesVecMutex;
    };
}