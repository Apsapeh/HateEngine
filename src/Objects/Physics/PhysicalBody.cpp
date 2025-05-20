#include <HateEngine/Objects/Physics/PhysicalBody.hpp>
#include <glm/gtx/quaternion.hpp>
#include <unordered_set>

#include "HateEngine/Log.hpp"
#include "HateEngine/Objects/Object.hpp"
#include "HateEngine/Objects/Physics/CollisionShape.hpp"
#include "reactphysics3d/components/RigidBodyComponents.h"

using namespace HateEngine;

PhysicalBody::PhysicalBody(BodyType bodyType) {
    this->bodyType = bodyType;
}


PhysicalBody::~PhysicalBody() {
}

void PhysicalBody::Init(reactphysics3d::RigidBody* body) {
    this->reactRigidBody = body;

    reactphysics3d::BodyType r_bodyType = reactphysics3d::BodyType::STATIC;

    if (this->bodyType == BodyType::DynamicBody)
        r_bodyType = reactphysics3d::BodyType::DYNAMIC;
    else if (this->bodyType == BodyType::KinematicBody)
        r_bodyType = reactphysics3d::BodyType::KINEMATIC;
    else if (this->bodyType == BodyType::CharacterBody)
        r_bodyType = reactphysics3d::BodyType::CHARACTER;

    this->reactRigidBody->setType(r_bodyType);
    this->reactRigidBody->setUserData(this);

    if (this->bodyType == BodyType::TriggerArea) {
        this->reactRigidBody->setIsAllowedToSleep(false);
    }

    this->reactRigidBody->setLinearVelocity(reactphysics3d::Vector3(
            this->linearVelocity.z, this->linearVelocity.y, this->linearVelocity.x
    ));
    this->reactRigidBody->setAngularVelocity(reactphysics3d::Vector3(
            this->angularVelocity.z, this->angularVelocity.y, this->angularVelocity.x
    ));
    this->reactRigidBody->setAngularLockAxisFactor(reactphysics3d::Vector3(
            this->angularLockAxisFactor.z, this->angularLockAxisFactor.y, this->angularLockAxisFactor.x
    ));
    this->reactRigidBody->applyLocalForceAtCenterOfMass(
            reactphysics3d::Vector3(this->forceLocal.z, this->forceLocal.y, this->forceLocal.x)
    );
    this->reactRigidBody->applyWorldForceAtCenterOfMass(
            reactphysics3d::Vector3(this->forceGlobal.z, this->forceGlobal.y, this->forceGlobal.x)
    );
    this->reactRigidBody->setLinearDamping(this->linearDamping);
    this->reactRigidBody->setAngularDamping(this->angularDamping);
    this->reactRigidBody->setMass(this->mass);
    this->reactRigidBody->setIsActive(this->isActive);
    this->reactRigidBody->enableGravity(this->isGravityEnabled);
    this->reactRigidBody->setIsAllowedToSleep(this->isAllowedToSleep);
}

void PhysicalBody::Update() {
    if (getIsSleeping())
        return;

    reactphysics3d::Quaternion qu = this->reactRigidBody->getTransform().getOrientation();
    // Rotation matrix
    glm::mat4 mat = glm::toMat4(glm::quat(qu.w, -qu.z, -qu.y, -qu.x));
    // Body position Z Y X
    reactphysics3d::Vector3 pos = this->reactRigidBody->getTransform().getPosition();

    glm::vec3 fix_pos = glm::vec3(pos.z, pos.y, pos.x) - this->parent_position;
    glm::mat4 fix_mat = mat / this->parent_rotation_matrix;

    Object::setPositionRaw(fix_pos);
    Object::setRotationMatrixRaw(fix_mat);

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

UUID PhysicalBody::addCollisionShape(CollisionShape* shape) {
    if (shape->reactShape != nullptr) {
        HATE_WARNING("CollisionShape is already binded to another body");
        return UUID(0);
    }

    shapes[shape->getUUID()] = shape;

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
        shapes.erase(uuid);
        return true;
    }
    return false;
}

std::unordered_map<UUID, CollisionShape*> const* PhysicalBody::getShapes() {
    return &shapes;
}

std::unordered_map<PhysicalBody*, std::vector<PhysicalBody::CollisionPoint>> const* const
PhysicalBody::getCollisionPoints() const {
    return &collisionPoints;
}


void PhysicalBody::setParentPosition(const glm::vec3& vec) {
    Object::setParentPosition(vec);
    updatePosition();
}

void PhysicalBody::setParentRotationMatrix(const glm::mat4& mat) {
    Object::setParentRotationMatrix(mat);
    updateRotation();
}

void PhysicalBody::setPositionRaw(const glm::vec3& vec) {
    Object::setPositionRaw(vec);
    updatePosition();
}

void PhysicalBody::setRotationMatrixRaw(const glm::mat4& mat) {
    Object::setRotationMatrixRaw(mat);
    updateRotation();
}


void PhysicalBody::setLinearVelocity(float x, float y, float z) {
    this->linearVelocity = {x, y, z};
    if (this->reactRigidBody != nullptr)
        reactRigidBody->setLinearVelocity(reactphysics3d::Vector3(z, y, x));
}
void PhysicalBody::setLinearVelocity(glm::vec3 vec) {
    setLinearVelocity(vec.x, vec.y, vec.z);
}

void PhysicalBody::setAngularVelocity(float x, float y, float z) {
    this->angularVelocity = {x, y, z};
    if (this->reactRigidBody != nullptr)
        reactRigidBody->setAngularVelocity(reactphysics3d::Vector3(z, y, x));
}
void PhysicalBody::setAngularVelocity(glm::vec3 vec) {
    setAngularVelocity(vec.x, vec.y, vec.z);
}

void PhysicalBody::setAngularLockAxisFactor(float x, float y, float z) {
    this->angularLockAxisFactor = {x, y, z};
    if (this->reactRigidBody != nullptr)
        reactRigidBody->setAngularLockAxisFactor(reactphysics3d::Vector3(z, y, x));
}

void PhysicalBody::setAngularLockAxisFactor(glm::vec3 vec) {
    setAngularLockAxisFactor(vec.x, vec.y, vec.z);
}

void PhysicalBody::applyForceLocal(float x, float y, float z) {
    this->forceLocal = {x, y, z};
    if (this->reactRigidBody != nullptr)
        reactRigidBody->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(z, y, x));
}

void PhysicalBody::applyForceLocal(const glm::vec3& vec) {
    applyForceLocal(vec.x, vec.y, vec.z);
}

void PhysicalBody::applyForceGlobal(float x, float y, float z) {
    this->forceGlobal = {x, y, z};
    if (this->reactRigidBody != nullptr)
        reactRigidBody->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(z, y, x));
}

void PhysicalBody::applyForceGlobal(const glm::vec3& vec) {
    applyForceGlobal(vec.x, vec.y, vec.z);
}

void PhysicalBody::setLinearDamping(float damping) {
    this->linearDamping = damping;
    if (this->reactRigidBody != nullptr)
        reactRigidBody->setLinearDamping(damping);
}

void PhysicalBody::setAngularDamping(float damping) {
    this->angularDamping = damping;
    if (this->reactRigidBody != nullptr)
        reactRigidBody->setAngularDamping(damping);
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

void PhysicalBody::setIsGravityEnabled(bool isGravityEnabled) {
    this->isGravityEnabled = isGravityEnabled;
    if (this->reactRigidBody != nullptr)
        reactRigidBody->enableGravity(isGravityEnabled);
}

void PhysicalBody::setIsAllowedToSleep(bool isAllowedToSleep) {
    this->isAllowedToSleep = isAllowedToSleep;
    if (this->reactRigidBody != nullptr)
        reactRigidBody->setIsAllowedToSleep(isAllowedToSleep);
}

void PhysicalBody::updateLocalCenterOfMassFromColliders() {
    if (this->reactRigidBody != nullptr)
        reactRigidBody->updateLocalCenterOfMassFromColliders();
}

void PhysicalBody::resetForce() {
    if (this->reactRigidBody != nullptr)
        reactRigidBody->resetForce();
}


void PhysicalBody::setIsRequiredCollisionPoints(bool required) {
    isRequiredCollisionPoints = required;
}


glm::vec3 PhysicalBody::getLinearVelocity() const {
    if (this->reactRigidBody != nullptr) {
        reactphysics3d::Vector3 vel = reactRigidBody->getLinearVelocity();
        return {vel.z, vel.y, vel.x};
    }
    return linearVelocity;
}

glm::vec3 PhysicalBody::getAngularVelocity() const {
    if (this->reactRigidBody != nullptr) {
        reactphysics3d::Vector3 vel = reactRigidBody->getAngularVelocity();
        return {vel.z, vel.y, vel.x};
    }
    return angularVelocity;
}


glm::vec3 PhysicalBody::getAngularLockAxisFactor() const {
    if (this->reactRigidBody != nullptr) {
        reactphysics3d::Vector3 vel = reactRigidBody->getAngularLockAxisFactor();
        return {vel.z, vel.y, vel.x};
    }
    return this->angularLockAxisFactor;
}

float PhysicalBody::getLinearDamping() const {
    if (this->reactRigidBody != nullptr)
        return reactRigidBody->getLinearDamping();
    return this->linearDamping;
}

float PhysicalBody::getAngularDamping() const {
    if (this->reactRigidBody != nullptr)
        return reactRigidBody->getAngularDamping();
    return this->angularDamping;
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

bool PhysicalBody::getIsGravityEnabled() const {
    if (this->reactRigidBody != nullptr)
        return reactRigidBody->isGravityEnabled();
    return this->isGravityEnabled;
}

bool PhysicalBody::getIsAllowedToSleep() const {
    if (this->reactRigidBody != nullptr)
        return reactRigidBody->isAllowedToSleep();
    return this->isAllowedToSleep;
}

bool PhysicalBody::getIsSleeping() const {
    if (this->reactRigidBody != nullptr)
        return reactRigidBody->isSleeping();
    return false;
}

bool PhysicalBody::getIsRequiredCollisionPoints() const {
    return isRequiredCollisionPoints;
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
