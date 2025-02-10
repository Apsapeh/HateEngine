#pragma once
#include <glm/gtx/euler_angles.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <string>
#include "../Utilities/UUID.hpp"
#include "../Input.hpp"

namespace HateEngine {
    class OpenGL_1_3;
    class PhysicalBody;

    class Object {
        friend OpenGL_1_3;
        friend PhysicalBody;
        friend class Camera;
        friend class Level;

    private:
        UUID uuid;

    protected:
        bool visible = true;

        struct BindedObj {
            Object* obj;
            bool bind_pos;
            bool bind_rot;
            bool bind_scale;
            bool bind_visible;
        };
        std::unordered_map<UUID, BindedObj> bindedObjects;

        glm::vec3 parent_position = {0, 0, 0};
        glm::vec3 parent_scale = {1.0, 1.0, 1.0};
        glm::mat4 parent_rotation_matrix = {
                1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0,
        };
        bool parent_visible = true;

        glm::vec3 position = {0, 0, 0};
        glm::vec3 scale = {1.0, 1.0, 1.0};
        glm::mat4 rotation_matrix = {
                1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0,
        };

        glm::vec3 global_position = {0, 0, 0};
        glm::mat4 global_rotation_matrix = glm::mat4(1);

        glm::vec3 direction = {0, 0, -1};
        std::string name = "";

        bool binded = false;

        virtual void setParentPosition(const glm::vec3& vec);
        virtual void setParentScale(const glm::vec3& vec);
        virtual void setParentRotationMatrix(const glm::mat4& mat);
        virtual void setParentVisible(bool visible);

        virtual void setPositionRaw(const glm::vec3& vec);
        virtual void setScaleRaw(const glm::vec3& vec);
        virtual void setRotationMatrixRaw(const glm::mat4& mat);

        void updateChildrenPositions();
        void updateChildrenScales();
        void updateChildrenRotations();
        void updateChildrenVisibilities();

        void updateDirection();

        virtual void _enterLevel(class Level* level) {
        }
        virtual void _exitLevel(class Level* level) {
        }
        /*virtual void _process(class Engine* engine, double delta) {
        }
        virtual void _fixedProcess(class Engine* engine, double delta) {
        }
        virtual void _inputEvent(class Engine* engine, InputClass::InputEventInfo event) {
        }*/

    public:
        void setName(std::string name);
        const std::string getName();

        void setPosition(const glm::vec3& vec);
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 vec);
        void setRotation(float x, float y, float z);
        virtual void setRotationMatrix(glm::mat4 mat);
        void setScale(const glm::vec3& vec);
        void setScale(float x, float y, float z);
        void setVisible(bool);

        void lookAt(const glm::vec3& vec);
        void lookAt(float x, float y, float z);

        void offset(const glm::vec3& vec);
        void offset(float x, float y, float z);
        void rotate(glm::vec3 vec, bool global = true);
        void rotate(float x, float y, float z, bool global = true);

        glm::vec3 getPosition() const;
        glm::vec3 getRotationEuler() const;
        glm::mat4 getRotationMatrix() const;
        glm::vec3 getScale() const;
        glm::vec3 getDirection() const;

        glm::vec3 getGlobalPosition() const;
        glm::vec3 getGlobalRotationEuler() const;
        glm::mat4 getGlobalRotationMatrix() const;
        glm::vec3 getGlobalScale() const;
        glm::vec3 getGlobalDirection() const;

        bool getVisible() const;
        bool getGlobalVisible() const;
        UUID getUUID() const;

        /**
         * Bind object as kind
         * \param obj Object to bind
         * \return UUID of binded object
         */
        UUID bindObj(
                Object* obj, bool bind_pos = true, bool bind_rot = true, bool bind_scale = true,
                bool bind_visible = true
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
