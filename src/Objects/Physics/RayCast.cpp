#include <HateEngine/Objects/Physics/RayCast.hpp>
#include <glm/gtx/quaternion.hpp>

#include "HateEngine/Log.hpp"
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

    for (const auto& body_obj: physEngine->physBodies) {
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
    }


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

void RayCast::cast(bool once) {
    if (physEngine == nullptr) {
        HATE_ERROR_F("RayCast [%llu]: PhysEngine is null\n", this->getUUID().getU64());
        return;
    }

    glm::vec3 global_pos = this->getGlobalPosition();
    glm::vec3 global_rot = this->getRotationEuler();
    float yaw = glm::radians(global_rot.y);
    float pitch = glm::radians(global_rot.x);
    glm::vec3 dir;
    dir.x = cos(pitch) * cos(yaw);
    dir.y = sin(pitch);
    dir.z = cos(pitch) * sin(yaw);
    dir = glm::normalize(dir) * length;

    glm::vec3 startPos = {global_pos.x, global_pos.y, global_pos.z};
    glm::vec3 endPos = {global_pos.x - dir.x, global_pos.y - dir.y, global_pos.z - dir.z};
    HATE_WARNING_F("Yaw : %f Pitch : %f", global_rot.y, global_rot.x);
    HATE_WARNING_F("DIr : %f %f %f", dir.x, dir.y, dir.z);

    this->rayCastCallback.oneCast = once;
    this->rayCastCallback.hits.clear();
    physEngine->getRayCastCollisions(startPos, endPos, &this->rayCastCallback);
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
