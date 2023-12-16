#include <Old3DEngine/Objects/Physics/PhysicalBody.hpp>
#include <glm/gtx/quaternion.hpp>
#include <algorithm>

using namespace Old3DEngine;

PhysicalBody::~PhysicalBody() {
    for (ShapeObject &obj : shapes) {
        if (not obj.is_ref)
            delete obj.shape;
    }
    shapes.clear();
}

void PhysicalBody::Init(reactphysics3d::PhysicsWorld *parrent_world, reactphysics3d::RigidBody *body) {
  is_initialized = true;
  this->reactParentPhysWorld = parrent_world;
  this->reactRigidBody = body;

  // TODO: Add set pref
}

void PhysicalBody::UpdateBinds() {
    reactphysics3d::Quaternion qu = this->reactRigidBody->getTransform().getOrientation();
    // Rotation matrix
    glm::mat4 mat = glm::toMat4(glm::quat(qu.w, -qu.z, -qu.y, -qu.x));
    // Body position Z Y X
    reactphysics3d::Vector3 pos = this->reactRigidBody->getTransform().getPosition();

    for (auto& obj : this->bindedObjects) {
        obj.second->setParentPosition({pos.z, pos.y, pos.x});
        obj.second->setParentRotationMatrix(mat);
    }
}

PhysicalBody::BodyType PhysicalBody::getBodyType() {
    return bodyType;
}


UUID_Generator::UUID PhysicalBody::addCollisionShapeClone(CollisionShape shape) {
    UUID_Generator::UUID id = uuidGenerator_shapes.gen();
    CollisionShape *new_shape = new CollisionShape(shape);
    //*new_shape = shape;
    shapes.push_back({new_shape, id, false});
    return id;
}

UUID_Generator::UUID PhysicalBody::addCollisionShapeRef(CollisionShape *shape) {
    UUID_Generator::UUID id = uuidGenerator_shapes.gen();
    shapes.push_back({shape, id, true});
    return id;
}

bool PhysicalBody::delCollisionShape(UUID_Generator::UUID uuid) {
    std::vector<ShapeObject>::iterator iter = std::find_if(
            shapes.begin(), shapes.end(), [&uuid] (ShapeObject &obj) -> bool {return obj.id == uuid;}
    );
    if (iter != shapes.end()) {
        if (not iter->is_ref)
            delete iter->shape;
        shapes.erase(iter);
        return true;
    }
    return false;
}


const reactphysics3d::PhysicsWorld* PhysicalBody::getParentPhysCommon() {
    return this->reactParentPhysWorld;
}


void setPosition(glm::vec3 vec) {
    // TODO: Add body
}
void setPosition(float x, float y, float z) {
    setPosition({x, y, z});
}

void setRotation(glm::vec3 vec) {
    // TODO: Add body
}
void setRotation(float x, float y, float z) {
    setRotation({x, y, z});
}

void offset(glm::vec3 vec) {
    // TODO: Add body
}
void offset(float x, float y, float z) {
    offset({x, y, z});
}

void PhysicalBody::rotate(float x, float y, float z) {
    Object::rotate(x, y, z);
    if (is_initialized) {
        glm::vec3 rotation = getRotationEuler();
        reactphysics3d::Vector3 pos(parent_position.x, parent_position.y, parent_position.z);
        reactphysics3d::Quaternion quaternion = reactphysics3d::Quaternion::fromEulerAngles(
                rotation.x, rotation.y, rotation.z
        );
        reactphysics3d::Transform transform(pos, quaternion);
        reactRigidBody->setTransform(transform);
    }
}
void PhysicalBody::rotate(glm::vec3 vec) {
    PhysicalBody::rotate(vec.x, vec.y, vec.z);
}

