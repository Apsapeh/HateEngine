#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <vector>
#include "../Object.hpp"

namespace HateEngine {
    class CollisionShape : public Object {
    private:
        bool collisionLayers[16] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        bool collisionMasks[16] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    public:
        enum ShapeEnum { Sphere, Capsule, Box, Convex };

        ShapeEnum shapeType;
        reactphysics3d::CollisionShape* reactShape = nullptr;
        reactphysics3d::Collider* reactCollider = nullptr;

        CollisionShape(ShapeEnum type, glm::vec3 pos, glm::vec3 rot);
        CollisionShape(ShapeEnum type, glm::vec3 pos, glm::mat4 rot);

        bool isInitialized();

        const reactphysics3d::CollisionShape* getReactShape();
        const reactphysics3d::Collider* getReactCollider();

        void setPosition(glm::vec3 vec);
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 vec);
        void setRotation(float x, float y, float z);

        void offset(float x, float y, float z);
        void offset(glm::vec3 vec);
        void rotate(float x, float y, float z);
        void rotate(glm::vec3 vec);

        /**
         * Sets the collision layer on which a body can collide with other
         * bodies
         * @param layers Array of layer numbers (0..15)
         * @param state Layer state array (true, false)
         */
        // void setCollisionLayers(std::vector<uint8_t>& layers,
        // std::vector<bool> state);

        /**
         * Sets the collision layer on which other bodies can collide with the
         * body
         * @param masks Array of layer numbers (0..15)
         * @param state Layer state array (true, false)
         */
        // void setCollisionMasks(std::vector<uint8_t>& masks, std::vector<bool>
        // state);
    };
} // namespace HateEngine
