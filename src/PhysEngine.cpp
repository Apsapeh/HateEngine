#include <HateEngine/PhysEngine.hpp>
#include <glm/ext.hpp>
#include <string>

#include "HateEngine/Log.hpp"
#include "HateEngine/Objects/Physics/BoxShape.hpp"
#include "HateEngine/Objects/Physics/CapsuleShape.hpp"
#include "HateEngine/Objects/Physics/CollisionShape.hpp"
#include "HateEngine/Objects/Physics/ConvexShape.hpp"
#include "HateEngine/Objects/Physics/PhysicalBody.hpp"
#include "HateEngine/Objects/Physics/SphereShape.hpp"
#include "HateEngine/Objects/Physics/TriggerArea.hpp"
#include "reactphysics3d/collision/OverlapCallback.h"
#include "reactphysics3d/mathematics/Ray.h"

using namespace HateEngine;

reactphysics3d::PhysicsCommon* PhysEngine::physicsCommon = nullptr;

class TriggerCallback : public reactphysics3d::EventListener {
public:
    // Вызывается при начале контакта
    /*void onContact(const CollisionCallback::CallbackData& callbackData) override {
        for (uint32_t i = 0; i < callbackData.getNbContactPairs(); i++) {
            const CollisionCallback::ContactPair& contactPair = callbackData.getContactPair(i);

                HATE_INFO("TRIGGER");
            if (contactPair.getCollider1()->getIsTrigger() ||
                contactPair.getCollider2()->getIsTrigger()) {
                // Проверяем, есть ли столкновение с триггером
                if (contactPair.getEventType() ==
                    CollisionCallback::ContactPair::EventType::ContactStart) {
                    std::cout << "Object entered trigger area!" << std::endl;
                } else if (contactPair.getEventType() ==
                           CollisionCallback::ContactPair::EventType::ContactExit) {
                    std::cout << "Object exited trigger area!" << std::endl;
                }
            }
        }
    }*/

    void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) override {
        for (uint32_t i = 0; i < callbackData.getNbOverlappingPairs(); ++i) {
            const reactphysics3d::OverlapCallback::OverlapPair& pair =
                    callbackData.getOverlappingPair(i);

            if (pair.getEventType() ==
                reactphysics3d::OverlapCallback::OverlapPair::EventType::OverlapStart) {
                if (pair.getCollider1()->getIsTrigger()) {
                    TriggerArea* area = (TriggerArea*) pair.getBody1()->getUserData();
                    area->callOnEnter((PhysicalBody*) pair.getBody2()->getUserData());
                }
                if (pair.getCollider2()->getIsTrigger()) {
                    TriggerArea* area = (TriggerArea*) pair.getBody2()->getUserData();
                    area->callOnEnter((PhysicalBody*) pair.getBody1()->getUserData());
                }
            }

            if (pair.getEventType() ==
                reactphysics3d::OverlapCallback::OverlapPair::EventType::OverlapExit) {
                if (pair.getCollider1()->getIsTrigger()) {
                    TriggerArea* area = (TriggerArea*) pair.getBody1()->getUserData();
                    area->callOnExit((PhysicalBody*) pair.getBody2()->getUserData());
                }
                if (pair.getCollider2()->getIsTrigger()) {
                    TriggerArea* area = (TriggerArea*) pair.getBody2()->getUserData();
                    area->callOnExit((PhysicalBody*) pair.getBody1()->getUserData());
                }
            }
        }
    }
};

PhysEngine::PhysEngine() {
    if (physicsCommon == nullptr) {
        physicsCommon = new reactphysics3d::PhysicsCommon();
    }
    this->physicsWorld = physicsCommon->createPhysicsWorld();
    this->physicsWorld->setEventListener(new TriggerCallback());
    // this->physicsWorld->setIsDebugRenderingEnabled(true);
    // reactphysics3d::Ray ray({0, 0, 0}, {1, 1, 1});
    // Change the number of iterations of the position solver
    // this->physicsWorld->setNbIterationsPositionSolver(16);
    // physicsWorld->setIsDebugRenderingEnabled(true);
}

PhysEngine::~PhysEngine() {
    for (const auto& body_pair: physBodies) {
        if (not body_pair.second.is_ref)
            delete body_pair.second.obj;
    }
    physicsCommon->destroyPhysicsWorld(physicsWorld);
}


void PhysEngine::IteratePhysics(float delta) {
    physicsWorld->update(delta);

    for (const auto& body_pair: physBodies) {
        // PhysicalBody* body = body_pair.second.obj;
        body_pair.second.obj->Update();
    }
}

void PhysEngine::getRayCastCollisions(
        glm::vec3 startPos, glm::vec3 endPos, reactphysics3d::RaycastCallback* callback
) {
    reactphysics3d::Ray ray({startPos.z, startPos.y, startPos.x}, {endPos.z, endPos.y, endPos.x});
    physicsWorld->raycast(ray, callback);
}

const reactphysics3d::PhysicsWorld* PhysEngine::getPhysicsWorld() const {
    return physicsWorld;
}

UUID PhysEngine::addObjectClone(const PhysicalBody& object) {
    PhysicalBody* new_obj = nullptr;
    // FIX ME: ADD obj copy
    physBodies[new_obj->getUUID()] = {new_obj, false};
    HATE_WARNING("addObjectClone is not implemented for PhysicalBody");
    return new_obj->getUUID();
}

UUID PhysEngine::addObjectRef(PhysicalBody* object) {
    if (object->reactRigidBody != nullptr) {
        HATE_ERROR(
                "PhysicalBody [" + std::to_string(object->getUUID().getU64()) +
                "] is already added to another PhysEngine"
        );
        return UUID(0);
    }

    glm::vec3 obj_pos = object->getGlobalPosition();
    // glm::vec3 obj_rot = glm::radians(body->getRotationEuler());
    glm::quat obj_rot_mat = glm::quat_cast(object->getGlobalRotationMatrix());
    reactphysics3d::Vector3 position(obj_pos.z, obj_pos.y, obj_pos.x);
    /*reactphysics3d::Quaternion quaternion =
    reactphysics3d::Quaternion::fromEulerAngles( obj_rot.z, obj_rot.y, obj_rot.x
    );*/
    reactphysics3d::Quaternion quaternion =
            reactphysics3d::Quaternion(obj_rot_mat.z, obj_rot_mat.y, obj_rot_mat.x, obj_rot_mat.w);
    reactphysics3d::Transform transform(position, quaternion);
    reactphysics3d::RigidBody* phys_body = physicsWorld->createRigidBody(transform);
    object->Init(phys_body);

    for (const auto& shape_pair: object->shapes) {
        reactphysics3d::CollisionShape* react_shape;
        CollisionShape::ShapeEnum shape_type = shape_pair.second.shape->shapeType;

        if (shape_pair.second.shape->isInitialized()) {
            HATE_WARNING("CollisionShape is already binded to another body");
            continue;
        }

        if (shape_type == CollisionShape::Box) {
            BoxShape* shape = (BoxShape*) shape_pair.second.shape;
            react_shape = physicsCommon->createBoxShape(
                    {shape->reactRightSize.x, shape->reactRightSize.y, shape->reactRightSize.z}
            );
            shape->reactShape = react_shape;
        } else if (shape_type == CollisionShape::Sphere) {
            SphereShape* shape = (SphereShape*) shape_pair.second.shape;
            react_shape = physicsCommon->createSphereShape(shape->getRadius());
            shape->reactShape = react_shape;
        } else if (shape_type == CollisionShape::Capsule) {
            CapsuleShape* shape = (CapsuleShape*) shape_pair.second.shape;
            react_shape = physicsCommon->createCapsuleShape(shape->getRadius(), shape->getHeight());
            shape->reactShape = react_shape;
        } else if (shape_type == CollisionShape::Convex) {
            ConvexShape* shape = (ConvexShape*) shape_pair.second.shape;
            reactphysics3d::PolyhedronMesh* PolyhedronMesh =
                    physicsCommon->createPolyhedronMesh(&shape->vertexArray);
            react_shape = physicsCommon->createConvexMeshShape(PolyhedronMesh);
            shape->reactShape = react_shape;
        } else {
            HATE_WARNING(
                    "CollisionShape [" +
                    std::to_string(shape_pair.second.shape->getUUID().getU64()) +
                    "] type is not implemented"
            );
            continue;
        }

        CollisionShape* shape = shape_pair.second.shape;
        glm::vec3 shape_pos = shape->getPosition();
        // glm::vec3 obj_rot = glm::radians(body->getRotationEuler());
        glm::quat shape_rot_mat = glm::quat_cast(shape->getRotationMatrix());
        reactphysics3d::Vector3 position(shape_pos.z, shape_pos.y, shape_pos.x);
        reactphysics3d::Quaternion quaternion = reactphysics3d::Quaternion(
                -shape_rot_mat.z, -shape_rot_mat.y, -shape_rot_mat.x, shape_rot_mat.w
        );
        reactphysics3d::Transform transform(position, quaternion);
        shape->reactCollider = phys_body->addCollider(react_shape, transform);
        shape->reactCollider->setUserData(shape);

        // shape->reactCollider->setC


        if (object->getBodyType() == PhysicalBody::TriggerArea) {
            shape->reactCollider->setIsTrigger(true);
        }
    }

    physBodies[object->getUUID()] = {object, true};
    return object->getUUID();
}

bool PhysEngine::removeObject(UUID uuid) {
    if (physBodies.count(uuid) == 1) {
        if (not physBodies[uuid].is_ref)
            delete physBodies[uuid].obj;
        physBodies.erase(uuid);
        return true;
    }
    return false;
}
