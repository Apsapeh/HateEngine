//
// Created by Тимофей Кириченко on 14.09.2024.
//

#pragma once

#include "../Object.hpp"
#include "HateEngine/PhysEngine.hpp"
#include "PhysicalBody.hpp"
#include "CollisionShape.hpp"
#include "reactphysics3d/collision/RaycastInfo.h"

namespace HateEngine {

    struct RayCastInfo {
        glm::vec3 worldPoint;
        glm::vec3 worldNormal;
        float fraction;
        PhysicalBody* body = nullptr;
        CollisionShape* shape = nullptr;
    };

    class RayCastCallback : public reactphysics3d::RaycastCallback {
    public:
        bool oneCast = true;
        std::vector<RayCastInfo> hits = {};
        PhysEngine* physEngine = nullptr;
        virtual reactphysics3d::decimal notifyRaycastHit(const reactphysics3d::RaycastInfo& info);
    };


    class RayCast : public Object {
    private:
        HateEngine::PhysEngine* physEngine = nullptr;
        RayCastCallback rayCastCallback;
        uint16_t collisionMask = -1;
        float length = 1.0f;
        glm::vec3 customDirection = {0, 0, 0};
        bool isCustomDirection = false;
        //                      legth/2, width/2, height/2
        // glm::vec3 reactRightSize = {.5f, .5f, .5f};
        void cast(bool once);


    public:
        RayCast(PhysEngine* physEngine, float length = 1.0f);
        RayCast(float length = 1.0f);
        void changeLength(float length);

        bool isCollide(RayCastInfo* rayCastInfo = nullptr);
        std::vector<RayCastInfo> getAllCollisions();
        std::vector<RayCastInfo> getSortedByDistanceAllCollisions(glm::vec3 point_to);

        void setPhysEngine(PhysEngine* physEngine);

        void enableCustomDirection(glm::vec3 direction);
        glm::vec3 disableCustomDirection();
        bool isCustomDirectionEnabled() const;

        /**
         * @brief Set the collision mask bit [0..15] on which the body can collide with other bodies
         *
         * @param bit [0..15]
         * @param state
         */
        void setCollisionMaskBit(uint8_t bit, bool state);

        bool getCollisionMaskBit(uint8_t bit);
        std::vector<uint8_t> getEnabledCollisionMaskBits();

        float getLength() const;
    };
} // namespace HateEngine
