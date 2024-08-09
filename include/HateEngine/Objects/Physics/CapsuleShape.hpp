//
// Created by Тимофей Кириченко on 30.04.2024.
//
#pragma once

#include "CollisionShape.hpp"

namespace HateEngine {

    class CapsuleShape : public CollisionShape {
    public:
        float radius = 0.5f;
        float height = 2.0f;

    public:
        CapsuleShape(
                float radius = 0.5f, float height = 2.0f, glm::vec3 pos = {0, 0, 0},
                glm::vec3 rot = {0, 0, 0}
        );

        void changeRadius(float r);
        void changeHeight(float h);

        float getRadius() const;
        float getHeight() const;
    };

} // namespace HateEngine
