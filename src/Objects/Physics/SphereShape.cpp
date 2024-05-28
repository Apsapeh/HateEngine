//
// Created by Тимофей Кириченко on 30.04.2024.
//

#include <HateEngine/Objects/Physics/SphereShape.hpp>

using namespace HateEngine;

SphereShape::SphereShape(float radius, glm::vec3 pos, glm::vec3 rot) : CollisionShape(Sphere, pos, rot) {
    changeRadius(radius);
}

void SphereShape::changeRadius(float radius) {
    this->radius = radius;

    if (reactShape != nullptr) {
        ((reactphysics3d::SphereShape*)reactShape)->setRadius(radius);
    }
}

float SphereShape::getRadius() const {
    return radius;
}