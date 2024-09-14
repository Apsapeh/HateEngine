#include <HateEngine/Objects/Physics/RayCast.hpp>
#include <glm/gtx/quaternion.hpp>

#include "HateEngine/Log.hpp"
#include "HateEngine/PhysEngine.hpp"
#include "reactphysics3d/mathematics/Vector3.h"

using namespace HateEngine;

reactphysics3d::decimal RayCastCallback::notifyRaycastHit(const reactphysics3d::RaycastInfo& info) {
 


        HATE_DEBUG_F("Hit point : %f %f %f \n", info.worldPoint.z, info.worldPoint.y, info.worldPoint.x);
 
        // Return a fraction of 1.0 to gather all hits
        return reactphysics3d::decimal(0.0);
    }

RayCast::RayCast(PhysEngine* physEngine, float length) {
    this->physEngine = physEngine;
    this->length = length;
}

void RayCast::cast() {
    if (physEngine == nullptr) {
        HATE_ERROR_F("RayCast [%llu]: PhysEngine is null\n", this->getUUID().getU64());
        return;
    }

    glm::vec3 global_pos = this->getGlobalPosition();
    reactphysics3d::Vector3 startPos = {global_pos.z, global_pos.y, global_pos.x};
    reactphysics3d::Vector3 endPos = {global_pos.z + length, global_pos.y, global_pos.x};

    physEngine->getRayCastCollisions({0, 1, 0}, {10, 1, 0}, &this->rayCastCallback);
}

bool RayCast::isCollide() {
    return false;
}