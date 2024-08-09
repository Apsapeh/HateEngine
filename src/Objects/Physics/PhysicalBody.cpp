#include <HateEngine/Objects/Physics/PhysicalBody.hpp>
#include <glm/gtx/quaternion.hpp>

#include "HateEngine/Log.hpp"

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

    this->reactRigidBody->setType((reactphysics3d::BodyType) this->bodyType
                                  // bodyType == StaticBody ? reactphysics3d::BodyType::STATIC :
                                  // bodyType
                                  // == KinematicBody ? reactphysics3d::BodyType::KINEMATIC :
                                  // reactphysics3d::BodyType::DYNAMIC
    );

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

UUID PhysicalBody::addCollisionShapeClone(CollisionShape shape) {
    CollisionShape* new_shape = new CollisionShape(shape);
    //*new_shape = shape;
    // shapes[id] = {new_shape, false};
    // TODO: Add body
    HATE_WARNING("addCollisionShapeClone is not implemented");
    return new_shape->getUUID();
}

UUID PhysicalBody::addCollisionShapeRef(CollisionShape* shape) {
    if (shape->reactShape != nullptr) {
        HATE_WARNING("CollisionShape is already binded to another body");
        return UUID(0);
    }

    shapes[shape->getUUID()] = {shape, true};

    if (this->reactRigidBody != nullptr) {
        HATE_WARNING("addCollisionShapeRef is not implemented");
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

void PhysicalBody::setPosition(glm::vec3 vec) {
    Object::setPosition(vec);

    if (this->reactRigidBody != nullptr) {
        glm::vec3 global_pos = Object::getGlobalPosition();
        reactphysics3d::Vector3 pos(global_pos.z, global_pos.y, global_pos.x);
        reactRigidBody->setTransform(
                reactphysics3d::Transform(pos, reactRigidBody->getTransform().getOrientation())
        );
    }
}
void PhysicalBody::setPosition(float x, float y, float z) {
    setPosition({x, y, z});
}

void PhysicalBody::setRotation(glm::vec3 vec) {
    Object::setRotation(vec);

    if (this->reactRigidBody != nullptr) {
        /*glm::vec3 global_rot = Object::getGlobalRotationEuler();
        reactphysics3d::Quaternion quaternion =
        reactphysics3d::Quaternion::fromEulerAngles( global_rot.z, global_rot.y,
        global_rot.x
        );
        reactRigidBody->setTransform(reactphysics3d::Transform(reactRigidBody->getTransform().getPosition(),
        quaternion));*/
        glm::quat q = glm::quat_cast(this->getGlobalRotationMatrix());
        reactphysics3d::Quaternion qu = reactphysics3d::Quaternion(-q.z, -q.y, -q.x, q.w);
        reactRigidBody->setTransform(
                reactphysics3d::Transform(reactRigidBody->getTransform().getPosition(), qu)
        );
    }
}
void PhysicalBody::setRotation(float x, float y, float z) {
    setRotation({x, y, z});
}

void PhysicalBody::offset(glm::vec3 vec) {
    // TODO: Add body
}
void PhysicalBody::offset(float x, float y, float z) {
    offset({x, y, z});
}

void PhysicalBody::rotate(float x, float y, float z) {
    // FIXME: FIX kind rotation
    Object::rotate(x, y, z);
    if (this->reactRigidBody != nullptr) {
        glm::vec3 rotation = getRotationEuler();
        reactphysics3d::Vector3 pos(parent_position.x, parent_position.y, parent_position.z);
        reactphysics3d::Quaternion quaternion =
                reactphysics3d::Quaternion::fromEulerAngles(rotation.x, rotation.y, rotation.z);
        reactphysics3d::Transform transform(pos, quaternion);
        reactRigidBody->setTransform(transform);
    }
}
void PhysicalBody::rotate(glm::vec3 vec) {
    PhysicalBody::rotate(vec.x, vec.y, vec.z);
}

/*glm::vec3 PhysicalBody::getPosition() const {
    return this->position;
}

glm::vec3 PhysicalBody::getRotationEuler() const {
    glm::vec3 rot;
    glm::extractEulerAngleXYZ(rotation_matrix, rot.x, rot.y, rot.z);
    rot *= -1;

    // Changes the Y rotation detection limit from [-PI/2, PI/2] to [-P, P]
    bool bad = rotation_matrix[0][0] == 0 and rotation_matrix[0][2] == 0;
    if (not bad)
        rot.y = atan2(rotation_matrix[0][0], -rotation_matrix[0][2]);
    else
        rot.y = atan2(rotation_matrix[1][0], -rotation_matrix[1][2]);
    return glm::degrees(rot);
}

glm::mat4 PhysicalBody::getRotationMatrix() const {
    return this->rotation_matrix;
}

glm::vec3 PhysicalBody::getScale() const {
    return this->scale;
}


glm::vec3 PhysicalBody::getGlobalPosition() const {
    reactphysics3d::Vector3 pos =
this->reactRigidBody->getTransform().getPosition(); glm::vec3 fix_pos =
glm::vec3(pos.z, pos.y, pos.x) - this->parent_position;
}

glm::vec3 PhysicalBody::getGlobalRotationEuler() const {
    return getRotationEuler();
}

glm::mat4 PhysicalBody::getGlobalRotationMatrix() const {
    if (not binded)
        return this->rotation_matrix;
    return this->parent_rotation_matrix *  this->rotation_matrix;
}*/
