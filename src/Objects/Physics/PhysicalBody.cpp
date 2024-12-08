#include <HateEngine/Objects/Physics/PhysicalBody.hpp>
#include <glm/gtx/quaternion.hpp>
#include <unordered_set>

#include "HateEngine/Log.hpp"
#include "reactphysics3d/components/RigidBodyComponents.h"

using namespace HateEngine;

PhysicalBody::PhysicalBody(BodyType bodyType) {
    this->bodyType = bodyType;
}


PhysicalBody::~PhysicalBody() {
    for (auto& obj: shapes) {
        if (not obj.second.is_ref)
            delete obj.second.shape;
    }
    shapes.clear();
}

void PhysicalBody::Init(reactphysics3d::RigidBody* body) {
    this->reactRigidBody = body;

    reactphysics3d::BodyType r_bodyType = reactphysics3d::BodyType::STATIC;

    if (this->bodyType == BodyType::DynamicBody)
        r_bodyType = reactphysics3d::BodyType::DYNAMIC;
    else if (this->bodyType == BodyType::KinematicBody)
        r_bodyType = reactphysics3d::BodyType::KINEMATIC;

    this->reactRigidBody->setType(r_bodyType);
    this->reactRigidBody->setUserData(this);

    if (this->bodyType == BodyType::TriggerArea) {
        this->reactRigidBody->setIsAllowedToSleep(false);
    }

    this->reactRigidBody->setMass(this->mass);
    this->reactRigidBody->setIsActive(this->isActive);
    this->reactRigidBody->setLinearVelocity(reactphysics3d::Vector3(
            this->linearVelocity.z, this->linearVelocity.y, this->linearVelocity.x
    ));
    this->reactRigidBody->setAngularLockAxisFactor(reactphysics3d::Vector3(
            this->angularLockAxisFactor.z, this->angularLockAxisFactor.y,
            this->angularLockAxisFactor.x
    ));
    // TODO: Add set pref
}

void PhysicalBody::Update() {
    reactphysics3d::Quaternion qu = this->reactRigidBody->getTransform().getOrientation();
    // Rotation matrix
    glm::mat4 mat = glm::toMat4(glm::quat(qu.w, -qu.z, -qu.y, -qu.x));
    // Body position Z Y X
    reactphysics3d::Vector3 pos = this->reactRigidBody->getTransform().getPosition();

    glm::vec3 fix_pos = glm::vec3(pos.z, pos.y, pos.x) - this->parent_position;
    glm::mat4 fix_mat = mat / this->parent_rotation_matrix;

    Object::setPosition(fix_pos);
    Object::setRotationMatrix(fix_mat);

    // std::cout << "[" << uuid.getU64() << "]: " << fix_pos.x << " " <<
    // fix_pos.y << " " << fix_pos.z << std::endl;

    if (binded) {
        if (this->prev_parent_position != this->parent_position) {
            this->prev_parent_position = this->parent_position;
            glm::vec3 global_pos = Object::getGlobalPosition();
            reactphysics3d::Vector3 pos(global_pos.z, global_pos.y, global_pos.x);
            reactRigidBody->setTransform(
                    reactphysics3d::Transform(pos, reactRigidBody->getTransform().getOrientation())
            );
        }

        if (this->prev_parent_rotation_matrix != this->parent_rotation_matrix) {
            this->prev_parent_rotation_matrix = this->parent_rotation_matrix;
            glm::quat q = glm::quat_cast(this->getGlobalRotationMatrix());
            reactphysics3d::Quaternion qu = reactphysics3d::Quaternion(-q.z, -q.y, -q.x, q.w);
            reactRigidBody->setTransform(
                    reactphysics3d::Transform(reactRigidBody->getTransform().getPosition(), qu)
            );
        }
    }
}

PhysicalBody::BodyType PhysicalBody::getBodyType() const {
    return bodyType;
}

void PhysicalBody::updatePosition() {
    if (this->reactRigidBody != nullptr) {
        glm::vec3 global_pos = Object::getGlobalPosition();
        reactphysics3d::Vector3 pos(global_pos.z, global_pos.y, global_pos.x);
        reactRigidBody->setTransform(
                reactphysics3d::Transform(pos, reactRigidBody->getTransform().getOrientation())
        );
    }
}

void PhysicalBody::updateRotation() {
    if (this->reactRigidBody != nullptr) {
        glm::quat q = glm::quat_cast(this->getGlobalRotationMatrix());
        // FIXME: Я не знаю, тут наверное зря были обратные углы, сделал в норму
        // reactphysics3d::Quaternion qu = reactphysics3d::Quaternion(-q.z, -q.y, -q.x, q.w);
        reactphysics3d::Quaternion qu = reactphysics3d::Quaternion(q.z, q.y, q.x, q.w);
        reactRigidBody->setTransform(
                reactphysics3d::Transform(reactRigidBody->getTransform().getPosition(), qu)
        );
    }
}

UUID PhysicalBody::addCollisionShapeClone(CollisionShape shape) {
    // CollisionShape* new_shape = new CollisionShape(shape);
    //*new_shape = shape;
    //  shapes[id] = {new_shape, false};
    //  TODO: Add body
    HATE_WARNING("addCollisionShapeClone is not implemented");
    return shape.getUUID();
}

UUID PhysicalBody::addCollisionShapeRef(CollisionShape* shape) {
    if (shape->reactShape != nullptr) {
        HATE_WARNING("CollisionShape is already binded to another body");
        return UUID(0);
    }

    shapes[shape->getUUID()] = {shape, true};

    if (this->reactRigidBody != nullptr) {
        HATE_WARNING_F(
                "PhysicalBody@addCollisionShapeRef() [%llu]: body is already initialized",
                this->getUUID().getU64()
        );
        /*reactphysics3d::Transform transform = reactRigidBody->getTransform();
        reactphysics3d::Vector3 pos = transform.getPosition();
        reactphysics3d::Quaternion qu = transform.getOrientation();
        reactphysics3d::Transform new_transform(pos, qu);
        reactRigidBody->setTransform(new_transform);*/
    }
    return shape->getUUID();
}

bool PhysicalBody::delCollisionShape(UUID uuid) {
    if (shapes.count(uuid) == 1) {
        if (!shapes[uuid].is_ref)
            delete shapes[uuid].shape;
        shapes.erase(uuid);
        return true;
    }
    return false;
}

std::unordered_map<UUID, PhysicalBody::ShapeObject> const* PhysicalBody::getShapes() {
    return &shapes;
}


void PhysicalBody::setPosition(glm::vec3 vec) {
    Object::setPosition(vec);
    updatePosition();
}
void PhysicalBody::setPosition(float x, float y, float z) {
    setPosition({x, y, z});
}

void PhysicalBody::setRotation(glm::vec3 vec) {
    Object::setRotation(vec);
    updateRotation();
}
void PhysicalBody::setRotation(float x, float y, float z) {
    setRotation({x, y, z});
}

void PhysicalBody::offset(glm::vec3 vec) {
    Object::offset(vec);
    updatePosition();
}
void PhysicalBody::offset(float x, float y, float z) {
    offset({x, y, z});
}

void PhysicalBody::rotate(float x, float y, float z) {
    Object::rotate(x, y, z);
    updateRotation();
}
void PhysicalBody::rotate(glm::vec3 vec) {
    PhysicalBody::rotate(vec.x, vec.y, vec.z);
}


void PhysicalBody::setLinearVelocity(float x, float y, float z) {
    this->linearVelocity = {x, y, z};
    if (this->reactRigidBody != nullptr)
        reactRigidBody->setLinearVelocity(reactphysics3d::Vector3(z, y, x));
}
void PhysicalBody::setLinearVelocity(glm::vec3 vec) {
    setLinearVelocity(vec.x, vec.y, vec.z);
}

void PhysicalBody::setAngularLockAxisFactor(float x, float y, float z) {
    this->angularLockAxisFactor = {x, y, z};
    if (this->reactRigidBody != nullptr)
        reactRigidBody->setAngularLockAxisFactor(reactphysics3d::Vector3(z, y, x));
}

void PhysicalBody::setAngularLockAxisFactor(glm::vec3 vec) {
    setAngularLockAxisFactor(vec.x, vec.y, vec.z);
}

void PhysicalBody::setMass(float mass) {
    this->mass = mass;
    if (this->reactRigidBody != nullptr)
        reactRigidBody->setMass(mass);
}

void PhysicalBody::setIsActive(bool isActive) {
    this->isActive = isActive;
    if (this->reactRigidBody != nullptr)
        reactRigidBody->setIsActive(isActive);
}


glm::vec3 PhysicalBody::getLinearVelocity() const {
    if (this->reactRigidBody != nullptr) {
        reactphysics3d::Vector3 vel = reactRigidBody->getLinearVelocity();
        return {vel.z, vel.y, vel.x};
    }
    return linearVelocity;
}


glm::vec3 PhysicalBody::getAngularLockAxisFactor() const {
    if (this->reactRigidBody != nullptr) {
        reactphysics3d::Vector3 vel = reactRigidBody->getAngularLockAxisFactor();
        return {vel.z, vel.y, vel.x};
    }
    return this->angularLockAxisFactor;
}

float PhysicalBody::getMass() const {
    if (this->reactRigidBody != nullptr)
        return reactRigidBody->getMass();
    return this->mass;
}

bool PhysicalBody::getIsActive() const {
    if (this->reactRigidBody != nullptr)
        return reactRigidBody->isActive();
    return this->isActive;
}


void PhysicalBody::addGroup(const std::string& group) {
    groups.insert(group);
}

void PhysicalBody::removeGroup(const std::string& group) {
    if (this->hasGroup(group))
        groups.erase(group);
}

bool PhysicalBody::hasGroup(const std::string& group) {
    return groups.count(group);
}

std::unordered_set<std::string> PhysicalBody::getGroups() {
    return groups;
}
