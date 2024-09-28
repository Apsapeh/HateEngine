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
        float length = 1.0f;
        //                      legth/2, width/2, height/2
        // glm::vec3 reactRightSize = {.5f, .5f, .5f};
        void cast(bool once);


    public:
        RayCast(PhysEngine* physEngine, float length = 1.0f);

        // void changeSize(float width, float height, float length);

        bool isCollide(RayCastInfo* rayCastInfo = nullptr);
        std::vector<RayCastInfo> getAllCollisions();
        // void
    };
} // namespace HateEngine
