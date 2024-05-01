#include "HateEngine/Error.hpp"
#include "reactphysics3d/collision/shapes/BoxShape.h"
#include <HateEngine/Objects/Physics/CollisionShape.hpp>

using namespace HateEngine;

void CollisionShape::setPosition(glm::vec3 vec) {
    Error::throwWarning("setPosition is not supported for CollisionShape");
}
void CollisionShape::setPosition(float x, float y, float z) {
    setPosition({x, y, z});
}

void CollisionShape::setRotation(glm::vec3 vec) {
    Error::throwWarning("setRotation is not supported for CollisionShape");
}
void CollisionShape::setRotation(float x, float y, float z) {
    setRotation({x, y, z});
}

void CollisionShape::offset(glm::vec3 vec) {
    Error::throwWarning("offest is not supported for CollisionShape");
}
void CollisionShape::offset(float x, float y, float z) {
    offset({x, y, z});
}

void CollisionShape::rotate(float x, float y, float z) {
    Error::throwWarning("rotate is not supported for CollisionShape");
}
void CollisionShape::rotate(glm::vec3 vec) {
    CollisionShape::rotate(vec.x, vec.y, vec.z);
}