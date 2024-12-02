#include <HateEngine/Objects/Physics/CollisionShape.hpp>

#include "HateEngine/Log.hpp"

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


void CollisionShape::setCollisionCategory(uint8_t category) {
    if (category > 15) {
        HATE_WARNING_F(
                "CollisonShape [%llu]: category cannot be greater than 15", this->getUUID().getU64()
        );
        return;
    }

    this->collisionCategory = 1 << category;

    if (this->reactCollider != nullptr)
        this->reactCollider->setCollisionCategoryBits(this->collisionCategory);
}

void CollisionShape::setCollisionMaskBit(uint8_t mask, bool state) {
    if (mask > 15) {
        HATE_WARNING_F(
                "CollisonShape [%llu]: mask cannot be greater than 15", this->getUUID().getU64()
        );
        return;
    }

    if (state)
        this->collisionMask |= 1 << mask;
    else
        this->collisionMask &= ~(1 << mask);

    if (this->reactCollider != nullptr)
        this->reactCollider->setCollideWithMaskBits(this->collisionMask);
}


uint8_t CollisionShape::getCollisionCategory() {
    uint16_t tmp = this->collisionCategory;
    for (int i = 0; i < 16; i++) {
        if (tmp & 1) {
            return i;
        }
        tmp >>= 1;
    }
    return 0;
}

bool CollisionShape::getCollisionMaskBit(uint8_t mask) {
    return this->collisionMask & (1 << mask);
}

std::vector<uint8_t> CollisionShape::getEnabledCollisionMaskBits() {
    std::vector<uint8_t> result;
    for (int i = 0; i < 16; i++) {
        if (this->collisionMask & (1 << i))
            result.push_back(i);
    }
    return result;
}
