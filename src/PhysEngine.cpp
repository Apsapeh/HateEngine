#include <HateEngine/PhysEngine.hpp>
#include <glm/ext.hpp>
#include <string>
#include <unordered_map>

#include "HateEngine/Log.hpp"
#include "HateEngine/Objects/Physics/BoxShape.hpp"
#include "HateEngine/Objects/Physics/CapsuleShape.hpp"
#include "HateEngine/Objects/Physics/CollisionShape.hpp"
#include "HateEngine/Objects/Physics/ConvexShape.hpp"
#include "HateEngine/Objects/Physics/PhysicalBody.hpp"
#include "HateEngine/Objects/Physics/SphereShape.hpp"
#include "HateEngine/Objects/Physics/TriggerArea.hpp"
#include "reactphysics3d/collision/CollisionCallback.h"
#include "reactphysics3d/collision/ContactPair.h"
#include "reactphysics3d/collision/OverlapCallback.h"
#include "reactphysics3d/mathematics/Ray.h"

using namespace HateEngine;

reactphysics3d::PhysicsCommon* PhysEngine::physicsCommon = nullptr;

namespace HateEngine {

    class EventCallback : public reactphysics3d::EventListener {
    public:
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

        /*
            I can't use pointer to PhysicalBody here, because object can be deleted during callback
            Solution: use smart pointer with ref counting
        */
        // std::vector<PhysicalBody*> bodies_on_update;
        std::vector<UUID> bodies_on_update = {};

        std::unordered_map<UUID, PhysicalBody*>* bodies_ptr = nullptr;

        void onContact(const rp3d::CollisionCallback::CallbackData& callbackData) override {
            /*for (auto& body: bodies_on_update) {
                auto it = bodies_ptr->find(body);
                if (it != bodies_ptr->end()) {
                    it->second->collisionPoints.clear();
                }
            }
            bodies_on_update.clear();*/

            for (uint32_t i = 0; i < callbackData.getNbContactPairs(); ++i) {
                const rp3d::CollisionCallback::ContactPair& pair = callbackData.getContactPair(i);

                const auto type = pair.getEventType();
                if (type == rp3d::CollisionCallback::ContactPair::EventType::ContactStart or
                    type == rp3d::CollisionCallback::ContactPair::EventType::ContactStay) {
                    PhysicalBody* body = (PhysicalBody*) pair.getBody1()->getUserData();
                    PhysicalBody* other = (PhysicalBody*) pair.getBody2()->getUserData();
                    // HATE_ERROR_F("Contact start: %u %u", body->getUUID().getU64(),
                    // other->getUUID().getU64());
                    if (body->getIsRequiredCollisionPoints() ||
                        other->getIsRequiredCollisionPoints()) {
                        // bodies_on_update.push_back(body->getUUID());
                        std::vector<PhysicalBody::CollisionPoint>* body_points = nullptr;
                        std::vector<PhysicalBody::CollisionPoint>* other_points = nullptr;
                        if (body->getIsRequiredCollisionPoints())
                            body_points = &(body->collisionPoints[other] = {});
                        if (other->getIsRequiredCollisionPoints())
                            other_points = &(other->collisionPoints[body] = {});

                        // HATE_WARNING_F("Count: %u", pair.getNbContactPoints());
                        for (uint32_t j = 0; j < pair.getNbContactPoints(); ++j) {
                            const rp3d::CollisionCallback::ContactPoint& contactPoint =
                                    pair.getContactPoint(j);

                            const auto& normal = contactPoint.getWorldNormal();
                            const auto& point = contactPoint.getLocalPointOnCollider1();
                            if (body_points)
                                body_points->push_back(
                                        {{point.z, point.y, point.x},
                                         {-normal.z, -normal.y, -normal.x}}
                                );
                            if (other_points)
                                other_points->push_back(
                                        {{point.z, point.y, point.x},
                                         {-normal.z, -normal.y, -normal.x}}
                                );
                        }
                    }

                } else if (type == rp3d::CollisionCallback::ContactPair::EventType::ContactExit) {
                    PhysicalBody* body = (PhysicalBody*) pair.getBody1()->getUserData();
                    PhysicalBody* other = (PhysicalBody*) pair.getBody2()->getUserData();
                    if (body->getIsRequiredCollisionPoints()) {
                        body->collisionPoints.erase(other);
                    }
                    if (other->getIsRequiredCollisionPoints()) {
                        other->collisionPoints.erase(body);
                    }
                }
            }
        }
    };
} // namespace HateEngine

PhysEngine::PhysEngine() {
    if (physicsCommon == nullptr) {
        physicsCommon = new reactphysics3d::PhysicsCommon();
    }
    this->physicsWorld = physicsCommon->createPhysicsWorld();
    this->listener = new EventCallback();
    ((EventCallback*) this->listener)->bodies_ptr = &this->physBodies;
    this->physicsWorld->setEventListener(listener);
}

PhysEngine::~PhysEngine() {
    physicsCommon->destroyPhysicsWorld(physicsWorld);
    delete (EventCallback*) listener;
}


void PhysEngine::IteratePhysics(float delta) {
    physicsWorld->update(delta);

    for (const auto& body_pair: physBodies) {
        // PhysicalBody* body = body_pair.second.obj;
        body_pair.second->Update();
    }
}

void PhysEngine::getRayCastCollisions(
        glm::vec3 startPos, glm::vec3 endPos, reactphysics3d::RaycastCallback* callback,
        uint16_t mask
) {
    reactphysics3d::Ray ray({startPos.z, startPos.y, startPos.x}, {endPos.z, endPos.y, endPos.x});
    physicsWorld->raycast(ray, callback, mask);
}


const reactphysics3d::PhysicsWorld* PhysEngine::getPhysicsWorld() const {
    return physicsWorld;
}

void PhysEngine::setIsGravityEnabled(bool isGravityEnabled) {
    this->physicsWorld->setIsGravityEnabled(isGravityEnabled);
}

void PhysEngine::setGravity(const glm::vec3& gravity) {
    this->physicsWorld->setGravity({gravity.z, gravity.y, gravity.x});
}

void PhysEngine::setIterationsPositionSolver(uint32_t count) {
    this->physicsWorld->setNbIterationsPositionSolver(count);
}

void PhysEngine::setIterationsVelocitySolver(uint32_t count) {
    this->physicsWorld->setNbIterationsVelocitySolver(count);
}


bool PhysEngine::isGravityEnabled() {
    return this->physicsWorld->isGravityEnabled();
}

glm::vec3 PhysEngine::getGravity() {
    reactphysics3d::Vector3 gravity = this->physicsWorld->getGravity();
    return {gravity.z, gravity.y, gravity.x};
}

uint32_t PhysEngine::getIterationsPositionSolver() {
    return this->physicsWorld->getNbIterationsPositionSolver();
}

uint32_t PhysEngine::getNbIterationsVelocitySolver() {
    return this->physicsWorld->getNbIterationsVelocitySolver();
}

UUID PhysEngine::addObject(PhysicalBody* object) {
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
        CollisionShape::ShapeEnum shape_type = shape_pair.second->shapeType;

        if (shape_pair.second->isInitialized()) {
            HATE_WARNING("CollisionShape is already binded to another body");
            continue;
        }

        if (shape_type == CollisionShape::Box) {
            BoxShape* shape = (BoxShape*) shape_pair.second;
            react_shape = physicsCommon->createBoxShape(
                    {shape->reactRightSize.x, shape->reactRightSize.y, shape->reactRightSize.z}
            );
            shape->reactShape = react_shape;
        } else if (shape_type == CollisionShape::Sphere) {
            SphereShape* shape = (SphereShape*) shape_pair.second;
            react_shape = physicsCommon->createSphereShape(shape->getRadius());
            shape->reactShape = react_shape;
        } else if (shape_type == CollisionShape::Capsule) {
            CapsuleShape* shape = (CapsuleShape*) shape_pair.second;
            react_shape = physicsCommon->createCapsuleShape(shape->getRadius(), shape->getHeight());
            shape->reactShape = react_shape;
        } else if (shape_type == CollisionShape::Convex) {
            ConvexShape* shape = (ConvexShape*) shape_pair.second;
            reactphysics3d::PolyhedronMesh* PolyhedronMesh =
                    physicsCommon->createPolyhedronMesh(&shape->vertexArray);
            react_shape = physicsCommon->createConvexMeshShape(PolyhedronMesh);
            shape->reactShape = react_shape;
        } else {
            HATE_WARNING(
                    "CollisionShape [" + std::to_string(shape_pair.second->getUUID().getU64()) +
                    "] type is not implemented"
            );
            continue;
        }

        CollisionShape* shape = shape_pair.second;
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

        shape->reactCollider->getMaterial().setFrictionCoefficient(shape->friction);
        shape->reactCollider->getMaterial().setBounciness(shape->bounciness);

        shape->reactCollider->setCollisionCategoryBits(shape->collisionCategory);
        shape->reactCollider->setCollideWithMaskBits(shape->collisionMask);


        if (object->getBodyType() == PhysicalBody::TriggerArea) {
            shape->reactCollider->setIsTrigger(true);
        }
    }

    physBodies[object->getUUID()] = object;
    return object->getUUID();
}

bool PhysEngine::removeObject(UUID uuid) {
    if (physBodies.count(uuid) == 1) {
        physicsWorld->destroyRigidBody(physBodies[uuid]->reactRigidBody);
        physBodies[uuid]->reactRigidBody = nullptr;

        for (const auto& shape_pair: physBodies[uuid]->shapes) {
            if (shape_pair.second->reactShape != nullptr) {
                CollisionShape::ShapeEnum shape_type = shape_pair.second->shapeType;

                if (shape_type == CollisionShape::Box) {
                    BoxShape* shape = (BoxShape*) shape_pair.second;
                    physicsCommon->destroyBoxShape((reactphysics3d::BoxShape*) shape->reactShape);
                    shape->reactShape = nullptr;
                    shape->reactCollider = nullptr;
                } else if (shape_type == CollisionShape::Sphere) {
                    SphereShape* shape = (SphereShape*) shape_pair.second;
                    physicsCommon->destroySphereShape(
                            (reactphysics3d::SphereShape*) shape->reactShape
                    );
                    shape->reactShape = nullptr;
                    shape->reactCollider = nullptr;
                } else if (shape_type == CollisionShape::Capsule) {
                    CapsuleShape* shape = (CapsuleShape*) shape_pair.second;
                    physicsCommon->destroyCapsuleShape(
                            (reactphysics3d::CapsuleShape*) shape->reactShape
                    );
                    shape->reactShape = nullptr;
                    shape->reactCollider = nullptr;
                } else if (shape_type == CollisionShape::Convex) {
                    ConvexShape* shape = (ConvexShape*) shape_pair.second;
                    physicsCommon->destroyConvexMeshShape(
                            (reactphysics3d::ConvexMeshShape*) shape->reactShape
                    );
                    shape->reactShape = nullptr;
                    shape->reactCollider = nullptr;
                }
            }
        }

        physBodies.erase(uuid);
        return true;
    }
    return false;
}
