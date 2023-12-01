#pragma once
#include <vector>
#include <unordered_map>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Utilities/UUID_Generator.hpp"

namespace Old3DEngine {
    class Object {
    public:
        enum ObjectType {
            OBJECT, CAMERA,
            MESH, PARTICLES, LIGHT
        };

    protected:
        glm::vec3 position = {0, 0, 0};
        //glm::vec3 rotation = {0, 0, 0};
        glm::vec3 scale    = {1.0, 1.0, 1.0};
        glm::vec3 relativ_scale = {1.0, 1.0, 1.0};
        glm::mat4 rotation_matrix = {
                1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0,
        };
        bool visible = true;
        std::unordered_map<UUID_Generator::UUID, Object> kinds;

    public:
        glm::vec3 prev_rot = {0, 0, 0};
        void setPosition(glm::vec3 vec);
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 vec);
        void setRotation(float x, float y, float z);
        void setRotationMatrix(glm::mat4 mat);
        void setScale(glm::vec3 vec);
        void setScale(float x, float y, float z);
        void setRelativScale(glm::vec3 vec);
        void setRelativScale(float x, float y, float z);
        void setVisible(bool);

        void offset(float x, float y, float z);
        void offset(glm::vec3 vec);
        void rotate(float x, float y, float z, bool global=true);
        void rotate(glm::vec3 vec, bool global=true);

        glm::vec3 getPosition();
        glm::vec3 getRotationEuler();
        glm::mat4 getRotationMatrix();
        glm::vec3 getScale();
        glm::vec3 getRelativScale();
        bool getVisible();

        /**
         * Copy object and add as kind
         * \param obj Object to copy
         * \return UUID of new kind
         */
        UUID_Generator::UUID addKind(Object obj);

        /**
         * Recursively delete kind by UUID
         * \param uuid UUID of kind
         * \return True if deleted successfully, False if the object is not found
         */
        bool delKind(UUID_Generator::UUID uuid);

        /**
         * Get kind by UUID
         * \param uuid UUID of kind
         * \return Reference to kind
         */
        Object& getKind(UUID_Generator::UUID uuid);
    };
}