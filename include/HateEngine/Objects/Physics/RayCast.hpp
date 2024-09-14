//
// Created by Тимофей Кириченко on 14.09.2024.
//

#pragma once

#include "../Object.hpp"
#include "HateEngine/PhysEngine.hpp"

namespace HateEngine {

    class RayCastCallback : public reactphysics3d::RaycastCallback {
    public:
        virtual reactphysics3d::decimal notifyRaycastHit(const reactphysics3d::RaycastInfo& info);
    };

    class RayCast : public Object {
    private:
        HateEngine::PhysEngine* physEngine = nullptr;
        RayCastCallback rayCastCallback;
        float length = 1.0f;
        //                      legth/2, width/2, height/2
        // glm::vec3 reactRightSize = {.5f, .5f, .5f};

    public:
        RayCast(PhysEngine* physEngine, float length = 1.0f);

        // void changeSize(float width, float height, float length);

        void cast();
        bool isCollide();
    };
} // namespace HateEngine
