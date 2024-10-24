#include <HateEngine/Objects/Physics/CollisionShape.hpp>

#include "HateEngine/Log.hpp"
#include "reactphysics3d/collision/shapes/BoxShape.h"

using namespace HateEngine;

CollisionShape::CollisionShape(ShapeEnum type, glm::vec3 pos, glm::vec3 rot) {
    this->shapeType = type;
    this->position = pos;
    rot = glm::radians(rot);
    rotation_matrix = glm::eulerAngleXYZ(rot.x, rot.y, rot.z);
}

CollisionShape::CollisionShape(ShapeEnum type, glm::vec3 pos, glm::mat4 rot) {
    this->shapeType = type;
    this->position = pos;
    rotation_matrix = rot;
}

bool CollisionShape::isInitialized() {
    return this->reactShape != nullptr and this->reactCollider != nullptr;
}

const reactphysics3d::CollisionShape* CollisionShape::getReactShape() {
    return this->reactShape;
}
const reactphysics3d::Collider* CollisionShape::getReactCollider() {
    return this->reactCollider;
}

void CollisionShape::setPosition(glm::vec3 vec) {
    HATE_WARNING("setPosition is not supported for CollisionShape");
}
void CollisionShape::setPosition(float x, float y, float z) {
    setPosition({x, y, z});
}

void CollisionShape::setRotation(glm::vec3 vec) {
    HATE_WARNING("setRotation is not supported for CollisionShape");
}
void CollisionShape::setRotation(float x, float y, float z) {
    setRotation({x, y, z});
}

void CollisionShape::offset(glm::vec3 vec) {
    HATE_WARNING("offest is not supported for CollisionShape");
}
void CollisionShape::offset(float x, float y, float z) {
    offset({x, y, z});
}

void CollisionShape::rotate(float x, float y, float z) {
    HATE_WARNING("rotate is not supported for CollisionShape");
}
void CollisionShape::rotate(glm::vec3 vec) {
    CollisionShape::rotate(vec.x, vec.y, vec.z);
}
