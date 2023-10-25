#pragma once
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Old3DEngine {
    class Object {
    protected:
        glm::vec3 position = {0, 0, 0};
        //glm::vec3 rotation = {0, 0, 0};
        glm::vec3 scale    = {1.0, 1.0, 1.0};
        glm::mat4 rotation_matrix = {
                1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0,
        };
        bool visible = true;

    public:
        glm::vec3 prev_rot = {0, 0, 0};
        void setPosition(glm::vec3 vec);
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 vec);
        void setRotation(float x, float y, float z);
        void setRotationMatrix(glm::mat4 mat);
        void setScale(glm::vec3 vec);
        void setScale(float x, float y, float z);
        void setVisible(bool);

        void offset(float x, float y, float z);
        void offset(glm::vec3 vec);
        void rotate(float x, float y, float z, bool global=true);
        void rotate(glm::vec3 vec, bool global=true);





        glm::vec3 getPosition();
        glm::vec3 getRotationEuler();
        glm::mat4 getRotationMatrix();
        glm::vec3 getScale();
        bool getVisible();
    };
}