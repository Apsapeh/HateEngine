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
        void setPosition(glm::vec3 vec);
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 vec);
        void setRotation(float x, float y, float z);
        void setScale(glm::vec3 vec);
        void setScale(float x, float y, float z);
        void setVisible(bool);

        void offset(float x, float y, float z);
        void offset(glm::vec3 vec);
        void rotate(float x, float y, float z);
        void rotate(glm::vec3 vec);


        glm::vec3 getPosition();
        glm::vec3 getRotation();
        glm::vec3 getScale();
        bool getVisible();
    };
}