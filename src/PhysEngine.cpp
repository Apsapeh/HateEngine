#include <HateEngine/PhysEngine.hpp>
#include <glm/ext.hpp>

#include "globalStaticParams.hpp"

using namespace HateEngine;

PhysEngine::PhysEngine() {
    //this->physBodies = phys_bodies_vec;
    physicsWorld = physicsCommon.createPhysicsWorld();
    physicsWorld->setIsDebugRenderingEnabled(true);
}

void PhysEngine::IteratePhysics(float delta) {
    physicsWorld->update(delta);

    for (const auto& body_pair : physBodies) {
        PhysicalBody* body = body_pair.second.obj;
        if (body->getParentPhysCommon() == this->physicsWorld) {
            body->Update();
        }
        else if (not body->is_initialized) {
            // INIT
            glm::vec3 obj_pos = body->getGlobalPosition();
            glm::vec3 obj_rot = glm::radians(body->getRotationEuler());
            reactphysics3d::Vector3 position(obj_pos.z, obj_pos.y, obj_pos.x);
            reactphysics3d::Quaternion quaternion = reactphysics3d::Quaternion::fromEulerAngles(
                    obj_rot.z, obj_rot.y, obj_rot.x
            );
            reactphysics3d::Transform transform(position, quaternion);
            reactphysics3d::RigidBody *phys_body = physicsWorld->createRigidBody(transform);
            body->Init(this->physicsWorld, phys_body);
        }
        else {
            // Warn
        }
    }

}

UUID_Generator::UUID PhysEngine::addObjectClone(const PhysicalBody& object) {
    UUID_Generator::UUID uuid = global_uuid_generator.gen();
    PhysicalBody* new_obj;
    // FIX ME: ADD obj copy
    physBodies[uuid] = {new_obj, false};
    return uuid;
}

UUID_Generator::UUID PhysEngine::addObjectRef(PhysicalBody *object) {
    UUID_Generator::UUID uuid = global_uuid_generator.gen();
    physBodies[uuid] = {object, true};
    return uuid;
}

bool PhysEngine::removeObject(UUID_Generator::UUID uuid) {
    if (physBodies.count(uuid) == 1) {
        if (not physBodies[uuid].is_ref)
            delete physBodies[uuid].obj;
        physBodies.erase(uuid);
        return true;
    }
    return false;
}

