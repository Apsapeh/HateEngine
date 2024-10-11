#pragma once

#include "../Resources/Texture.hpp"
#include "CubeMesh.hpp"
#include "Object.hpp"

namespace HateEngine {
    class Camera : public Object {
    private:
        float FOV;
        float renderDist;
        glm::vec3 upVec = {0, 1.0, 0};
        CubeMesh skybox;
        bool skyboxEnabled = false;

        Object root_obj;

    protected:
        void setParentPosition(glm::vec3 vec) override;
        void setParentScale(glm::vec3 vec) override;
        void setParentRotationMatrix(const glm::mat4& mat) override;

    public:
        float yaw = -90;
        float pitch = 0;

        Camera(float fov, float render_dist);
        void setFOV(float fov);
        void setRenderDist(float dist);
        void setSkyBoxTexture(Texture* tex);
        void setSkyBoxEnabled(bool enabled);

        float getFOV() const;
        float getRenderDist() const;
        const CubeMesh* getSkyBox() const;
        bool isSkyBoxEnabled() const;

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

        /*void setRotation(glm::vec3 vec);
        void setRotation(float x, float y, float z);*/
        void setPosition(glm::vec3 vec);
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 vec);
        void setRotation(float x, float y, float z);
        void setRotationMatrix(glm::mat4 mat) override;
        void lookAt(glm::vec3 vec);
        void lookAt(float x, float y, float z);
        void offset(glm::vec3 vec);
        void offset(float x, float y, float z);
        void rotate(glm::vec3 vec, bool global = true);
        void rotate(float x, float y, float z, bool global = true);
    };
} // namespace HateEngine
