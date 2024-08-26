//
// Created by Тимофей Кириченко on 30.04.2024.
//

#include "reactphysics3d/collision/shapes/CapsuleShape.h"

#include <HateEngine/Objects/Physics/CapsuleShape.hpp>

using namespace HateEngine;

CapsuleShape::CapsuleShape(float radius, float height, glm::vec3 pos, glm::vec3 rot) :
    CollisionShape(Capsule, pos, rot) {
    changeRadius(radius);
    changeHeight(height);
}

void CapsuleShape::changeRadius(float radius) {
    this->radius = radius;

    if (reactShape != nullptr) {
        ((reactphysics3d::CapsuleShape*) reactShape)->setRadius(radius);
    }
}

void CapsuleShape::changeHeight(float height) {
    this->height = height - this->radius * 2;

    if (reactShape != nullptr) {
        ((reactphysics3d::CapsuleShape*) reactShape)->setHeight(this->height);
    }
}

float CapsuleShape::getRadius() const {
    return radius;
}

float CapsuleShape::getHeight() const {
    return height;
}
