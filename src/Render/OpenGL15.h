#pragma once
#include <vector>
#include <cstdint>
#include "../Old3DEngine.h"

namespace Old3DEngine {
    class OpenGL15 {
    public:
        OpenGL15(std::vector<Engine::SceneObject>*);
        void Draw();
        //void eraseObjects();

    private:
        std::vector<Engine::SceneObject> *meshes;
    };
}