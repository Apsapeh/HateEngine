#include <Old3DEngine/PhysEngine.hpp>
#include <glm/ext.hpp>

using namespace Old3DEngine;

PhysEngine::PhysEngine() {
    //this->physBodies = phys_bodies_vec;
    physicsWorld = physicsCommon.createPhysicsWorld();
    physicsWorld->setIsDebugRenderingEnabled(true);
}

void PhysEngine::IteratePhysics(float delta) {
    physicsWorld->update(delta);

    for (physBodyStruct &body : physBodies) {
        if (body.obj->getParentPhysCommon() == this->physicsWorld) {
            body.obj->UpdateBindsPos();
        }
        else if (not body.obj->is_initialized) {
            // INIT
            glm::vec3 obj_pos = body.obj->getPosition();
            glm::vec3 obj_rot = glm::radians(body.obj->getRotationEuler());
            reactphysics3d::Vector3 position(obj_pos.x, obj_pos.y, obj_pos.z);
            reactphysics3d::Quaternion quaternion = reactphysics3d::Quaternion::fromEulerAngles(
                    obj_rot.x, obj_rot.y, obj_rot.z
            );
            reactphysics3d::Transform transform(position, quaternion);
            reactphysics3d::RigidBody *phys_body = physicsWorld->createRigidBody(transform);
            body.obj->Init(this->physicsWorld, phys_body);
        }
        else {
            // Warn
        }
    }

}

UUID_Generator::UUID PhysEngine::addObjectClone(PhysicalBody object) {
    UUID_Generator::UUID uuid = uuidGenerator.gen();
    return uuid;
}

UUID_Generator::UUID PhysEngine::addObjectRef(PhysicalBody *object) {
    UUID_Generator::UUID uuid = uuidGenerator.gen();
    return uuid;
}

bool PhysEngine::removeObject(UUID_Generator::UUID uuid) {
    return false;
}


void init_body() {

}
