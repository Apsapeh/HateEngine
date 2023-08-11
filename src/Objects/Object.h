#pragma once
#include <glm/vec3.hpp>

namespace Old3DEngine {
    class Object {
    protected:
        glm::vec3 position = {0, 0, 0};
        glm::vec3 rotation = {0, 0, 0};
        glm::vec3 scale    = {1.0, 1.0, 1.0};

    public:
        void setPosition(glm::vec3 pos);
        void setPosition(float x, float y, float z);

        glm::vec3 getPosition();
    };
}