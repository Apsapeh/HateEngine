#pragma once
#include <glm/vec3.hpp>

namespace Old3DEngine {
    class Object {
    protected:
        glm::vec3 position = {0, 0, 0};
        glm::vec3 rotation = {0, 0, 0};
        glm::vec3 scale    = {1.0, 1.0, 1.0};
        bool visible = true;

    public:
        void setPosition(glm::vec3 pos);
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 pos);
        void setRotation(float x, float y, float z);
        void setScale(glm::vec3 pos);
        void setScale(float x, float y, float z);
        void setVisible(bool);

        glm::vec3 getPosition();
        glm::vec3 getRotation();
        glm::vec3 getScale();
        bool getVisible();
    };
}