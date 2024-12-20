#pragma once
#include "HateEngine/Resources/Texture.hpp"
#include "Object.hpp"
#include "Mesh.hpp"
#include "Physics/RayCast.hpp"
// #include

namespace HateEngine {
    class Decal : public Object {
        glm::vec3 custom_normal = {0, 0, 0};
        bool is_in_position = false;

        glm::vec3 ray_direction = {0, 0, 1};
        Mesh mesh;
        RayCast ray;

    public:
        Decal();
        // Decal(Mesh* mesh);
        void bake();
        void setSize(float width, float height);
        void setTexture(Texture* texture);
        void enableLightShading();
        void disableLightShading();

        void setPhysEngine(PhysEngine* physEngine);

        void enableInPosition(glm::vec3 normal);
        void setNormal(glm::vec3 normal);
        void disableInPosition();
        void setRayDirection(glm::vec3 direction);
        void setRayLength(float length);

        // glm::vec3 getSize
        glm::vec3 getInPositionNormal() const;
        bool isInPosition() const;

        glm::vec3 getRayDirection() const;
        float getRayLength() const;

        Texture* getTexture() const;
        bool isLightShading() const;

        Mesh* getMesh() const;
    };
} // namespace HateEngine
