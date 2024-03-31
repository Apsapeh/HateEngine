#pragma once
#include "Object.hpp"

namespace HateEngine {
    class Camera : public Object {
    private:
        float viewAspect;
        float FOV;
        float renderDist;
        glm::vec3 upVec = {0, 1.0, 0};

    public:
        float yaw = -90;
        float pitch = 0;

        Camera(float view_aspect, float fov, float render_dist);

        void setViewAspect(float view_aspect);
        void setFOV(float fov);
        void setRenderDist(float dist);

        float getViewAspect() const;
        float getFOV() const;
        float getRenderDist() const;
    };
}