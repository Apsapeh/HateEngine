#include <HateEngine/Objects/Physics/RayCast.hpp>
#include <glm/gtx/quaternion.hpp>

#include "HateEngine/Log.hpp"
#include "HateEngine/Objects/Physics/CollisionShape.hpp"
#include "HateEngine/PhysEngine.hpp"
#include "glm/fwd.hpp"
#include "reactphysics3d/mathematics/Vector3.h"

using namespace HateEngine;

reactphysics3d::decimal RayCastCallback::notifyRaycastHit(const reactphysics3d::RaycastInfo& info) {


    /*HATE_DEBUG_F(
            "Hit point : %f %f %f \n", info.worldPoint.z, info.worldPoint.y, info.worldPoint.x
    );*/

    RayCastInfo rayCastInfo;
    rayCastInfo.worldPoint = glm::vec3(info.worldPoint.z, info.worldPoint.y, info.worldPoint.x);

    rayCastInfo.worldNormal = glm::vec3(info.worldNormal.z, info.worldNormal.y, info.worldNormal.x);

    rayCastInfo.fraction = (float) info.hitFraction;

    rayCastInfo.body = (PhysicalBody*) info.body->getUserData();

    rayCastInfo.shape = (CollisionShape*) info.collider->getUserData();

    /*for (const auto& body_obj: physEngine->physBodies) {
        PhysicalBody* body = body_obj.second.obj;
        if (body->reactRigidBody == info.body) {
            rayCastInfo.body = body;
            for (const auto& shape_obj: *body->getShapes()) {
                if (shape_obj.second.shape->getReactCollider() == info.collider) {
                    rayCastInfo.shape = shape_obj.second.shape;
                    break;
                }
            }
            break;
        }
    }*/


    hits.push_back(rayCastInfo);

    // Return a fraction of 1.0 to gather all hits
    if (oneCast)
        return reactphysics3d::decimal(0.0f);
    return reactphysics3d::decimal(1.0f);
}

RayCast::RayCast(PhysEngine* physEngine, float length) {
    this->physEngine = physEngine;
    this->length = length;
    this->rayCastCallback.physEngine = physEngine;
}

RayCast::RayCast(float length) {
    this->length = length;
}

void RayCast::changeLength(float length) {
    this->length = length;
}

void RayCast::cast(bool once) {
    if (physEngine == nullptr) {
        HATE_ERROR_F("RayCast [%llu]: PhysEngine is null\n", this->getUUID().getU64());
        return;
    }

    glm::vec3 global_pos = this->getGlobalPosition();
    glm::vec3 dir;

    if (this->isCustomDirection)
        dir = this->customDirection * length;
    else
        dir = this->getGlobalDirection() * length;


    glm::vec3 startPos = {global_pos.x, global_pos.y, global_pos.z};
    glm::vec3 endPos = {global_pos.x + dir.x, global_pos.y + dir.y, global_pos.z + dir.z};

    this->rayCastCallback.oneCast = once;
    this->rayCastCallback.hits.clear();
    physEngine->getRayCastCollisions(startPos, endPos, &this->rayCastCallback, this->collisionMask);
}

bool RayCast::isCollide(RayCastInfo* rayCastInfo) {
    this->cast(true);
    if (this->rayCastCallback.hits.empty())
        return false;

    if (rayCastInfo != nullptr) {
        *rayCastInfo = this->rayCastCallback.hits[0];
    }

    return true;
}

std::vector<RayCastInfo> RayCast::getAllCollisions() {
    this->cast(false);
    return this->rayCastCallback.hits;
}

std::vector<RayCastInfo> RayCast::getSortedByDistanceAllCollisions(glm::vec3 point_to) {
    this->cast(false);
    std::sort(
            this->rayCastCallback.hits.begin(), this->rayCastCallback.hits.end(),
            [point_to](const RayCastInfo& a, const RayCastInfo& b) {
                return glm::distance(a.worldPoint, point_to) < glm::distance(b.worldPoint, point_to);
            }
    );
    return this->rayCastCallback.hits;
}

void RayCast::setPhysEngine(PhysEngine* physEngine) {
    this->physEngine = physEngine;
}

void RayCast::enableCustomDirection(glm::vec3 direction) {
    this->customDirection = direction;
    this->isCustomDirection = true;
}

glm::vec3 RayCast::disableCustomDirection() {
    this->isCustomDirection = false;
    return this->customDirection;
}

bool RayCast::isCustomDirectionEnabled() const {
    return this->isCustomDirection;
}

void RayCast::setCollisionMaskBit(uint8_t mask, bool state) {
    if (mask > 15) {
        HATE_WARNING_F("CollisonShape [%llu]: mask cannot be greater than 15", this->getUUID().getU64());
        return;
    }

    if (state)
        this->collisionMask |= 1 << mask;
    else
        this->collisionMask &= ~(1 << mask);
}

bool RayCast::getCollisionMaskBit(uint8_t mask) {
    return this->collisionMask & (1 << mask);
}

std::vector<uint8_t> RayCast::getEnabledCollisionMaskBits() {
    std::vector<uint8_t> result;
    for (int i = 0; i < 16; i++) {
        if (this->collisionMask & (1 << i))
            result.push_back(i);
    }
    return result;
}

float RayCast::getLength() const {
    return this->length;
}
