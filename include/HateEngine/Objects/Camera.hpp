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

        float prev_aspect_ratio = 0;
        glm::mat4 projectionMatrix = glm::mat4(1.0f);

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

        glm::mat4 getProjectionMatrix(float aspect_ratio);
        glm::mat4 getViewMatrix() const;

        glm::vec3 getProjectRayFromScreen(glm::vec2 pos, glm::vec2 screen);
    };
} // namespace HateEngine
