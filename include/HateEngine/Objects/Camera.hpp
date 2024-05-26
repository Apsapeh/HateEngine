#pragma once

#include "Object.hpp"
#include "../Resources/Texture.hpp"
#include "CubeMesh.hpp"

namespace HateEngine {
    class Camera : public Object {
    private:
        float FOV;
        float renderDist;
        glm::vec3 upVec = {0, 1.0, 0};
        CubeMesh skybox;
        bool skyboxEnabled = false;

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
    };
}