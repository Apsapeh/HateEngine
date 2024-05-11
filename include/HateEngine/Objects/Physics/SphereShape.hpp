//
// Created by Тимофей Кириченко on 30.04.2024.
//

#pragma once

#include "CollisionShape.hpp"

namespace HateEngine {

class SphereShape : public CollisionShape {
public:
    float radius = 0.5f;

public:
    SphereShape(
        float radius = 0.5f,
        glm::vec3 pos = {0, 0, 0},
        glm::vec3 rot = {0, 0, 0}
    );

    void changeRadius(float radius);
};

} // HateEngine

