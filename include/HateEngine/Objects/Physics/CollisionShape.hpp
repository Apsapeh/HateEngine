#pragma once
#include <cstdint>
#include <reactphysics3d/reactphysics3d.h>
#include <vector>
#include "../Object.hpp"

namespace HateEngine {
    class CollisionShape : public Object {
        friend class PhysEngine;

    private:
        uint16_t collisionCategory = 1;
        uint16_t collisionMask = -1;

        float friction = 0.0f;
        float bounciness = 0.0f;

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

        void setFriction(float friction);
        void setBounciness(float bounciness);

        float getFriction();
        float getBounciness();

        /**
         * @brief Set the collision category [0..15]
         * @param category [0..15]
         */
        void setCollisionCategory(uint8_t category);

        /**
         * @brief Set the collision mask bit [0..15] on which the body can collide with other bodies
         *
         * @param bit [0..15]
         * @param state
         */
        void setCollisionMaskBit(uint8_t bit, bool state);

        uint8_t getCollisionCategory();
        bool getCollisionMaskBit(uint8_t bit);
        std::vector<uint8_t> getEnabledCollisionMaskBits();

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
