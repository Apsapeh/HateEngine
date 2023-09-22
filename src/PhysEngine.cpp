#include <Old3DEngine/PhysEngine.hpp>

using namespace Old3DEngine;

PhysEngine::PhysEngine() {
    //this->physBodies = phys_bodies_vec;
    physicsWorld = physicsCommon.createPhysicsWorld();
}

void PhysEngine::IteratePhysics(float delta) {
    physicsWorld->update(delta);
    std::cout << delta << "\n";

    for (physBodyStruct &body : physBodies) {
        if (body.obj->getParentPhysCommon() == &this->physicsCommon) {
            body.obj->UpdateBindsPos();
        }
        else if (not body.obj->is_initialized) {
            // INIT
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
