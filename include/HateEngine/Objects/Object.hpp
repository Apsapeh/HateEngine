#pragma once
#include <glm/gtx/euler_angles.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <vector>
#include "../Utilities/UUID.hpp"

namespace HateEngine {
    class OpenGL15;
    class PhysicalBody;

    class Object {
        friend OpenGL15;
        friend PhysicalBody;
        friend class Camera;
        friend class Model;

    private:
        UUID uuid;

    protected:
        bool visible = true;

        struct BindedObj {
            Object* obj;
            bool bind_pos;
            bool bind_rot;
            bool bind_scale;
        };
        // FIXME
        std::unordered_map<UUID, BindedObj> bindedObjects;

        glm::vec3 parent_position = {0, 0, 0};
        glm::vec3 parent_scale = {1.0, 1.0, 1.0};
        glm::mat4 parent_rotation_matrix = {
                1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0,
        };
        glm::vec3 position = {0, 0, 0};
        glm::vec3 scale = {1.0, 1.0, 1.0};
        glm::mat4 rotation_matrix = {
                1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0,
        };

        bool binded = false;

        virtual void setParentPosition(glm::vec3 vec);
        virtual void setParentScale(glm::vec3 vec);
        virtual void setParentRotationMatrix(const glm::mat4& mat);

    public:
        void setPosition(glm::vec3 vec);
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 vec);
        void setRotation(float x, float y, float z);
        virtual void setRotationMatrix(glm::mat4 mat);
        void setScale(glm::vec3 vec);
        void setScale(float x, float y, float z);
        void setVisible(bool);

        void offset(glm::vec3 vec);
        void offset(float x, float y, float z);
        void rotate(glm::vec3 vec, bool global = true);
        void rotate(float x, float y, float z, bool global = true);

        glm::vec3 getPosition() const;
        glm::vec3 getRotationEuler() const;
        glm::mat4 getRotationMatrix() const;
        glm::vec3 getScale() const;

        glm::vec3 getGlobalPosition() const;
        glm::vec3 getGlobalRotationEuler() const;
        glm::mat4 getGlobalRotationMatrix() const;
        glm::vec3 getGlobalScale() const;

        bool getVisible() const;
        UUID getUUID() const;

        /**
         * Bind object as kind
         * \param obj Object to bind
         * \return UUID of binded object
         */
        UUID bindObj(
                Object* obj, bool bind_pos = true, bool bind_rot = true, bool bind_scale = true
        );

        /**
         * Unbind object by UUID
         * \param uuid UUID of binded object
         * \return True if unbinded successfully, False if the object is not
         * found
         */
        bool unbindObj(UUID uuid);
    };
} // namespace HateEngine
