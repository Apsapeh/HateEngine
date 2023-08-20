#pragma once
#include <vector>
#include <cstdint>
#include "../Old3DEngine.hpp"

namespace Old3DEngine {
    class OpenGL15 {
    public:
        OpenGL15(std::vector<Engine::SceneObject>* meshes, std::vector<Engine::SceneObject>* lights);
        void Draw();
        //void eraseObjects();

    private:
        std::vector<Engine::SceneObject> *meshes;
        std::vector<Engine::SceneObject> *lights;
        uint8_t maxLightCount = 8;
        float maxLightRenderDist = 25;

        inline void renderLight(std::vector<int> indicies);
        inline std::vector<int> getNearestLights(glm::vec3 position);
    };
}