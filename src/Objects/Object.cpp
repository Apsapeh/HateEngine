#include <Old3DEngine/Objects/Object.hpp>
#include "../globalStaticParams.hpp"
//#include <cmath>

//TODO: Добавить дочерние Object, которые наследуют перемещение, вращение и подобное

using namespace Old3DEngine;

void Object::setParentPosition(const glm::vec3 vec) {
    this->parent_position = vec;
    for (auto& obj : bindedObjects)
        obj.second->setParentPosition(getGlobalPosition());
}

void Object::setParentScale(const glm::vec3 vec) {
    this->parent_scale = vec;
    for (auto& obj : bindedObjects)
        obj.second->setParentScale(getGlobalScale());
}

void Object::setParentRotationMatrix(const glm::mat4& mat) {
    this->parent_rotation_matrix = mat;
    for (auto& obj : bindedObjects)
        obj.second->setParentRotationMatrix(getGlobalRotationMatrix());
}

void Object::setPosition(const glm::vec3 value) {
    for (auto& obj : bindedObjects)
        obj.second->setParentPosition(value);
    this->position = value;
}
void Object::setPosition(float x, float y, float z) {
    setPosition({x, y, z});
}

void Object::setRotation(glm::vec3 value) {
    value = glm::radians(value);
    rotation_matrix = glm::eulerAngleXYZ(value.x, value.y, value.z);
    for (auto& obj : bindedObjects)
        obj.second->setParentRotationMatrix(rotation_matrix);

}
void Object::setRotation(float x, float y, float z) {
    setRotation({x, y, z});
}

void Object::setRotationMatrix(glm::mat4 mat) {
    rotation_matrix = mat;
    for (auto& obj : bindedObjects)
        obj.second->setParentRotationMatrix(getGlobalRotationMatrix());
}

void Object::setScale(glm::vec3 value) {
    this->scale = value;
    for (auto& obj : bindedObjects)
        obj.second->setParentScale(getGlobalScale());
}
void Object::setScale(float x, float y, float z) {
    setScale({x, y, z});
}

void Object::setVisible(bool vis) {
    this->visible = vis;
}

void Object::offset(glm::vec3 vec) {
    setPosition(vec + this->position);
}
void Object::offset(float x, float y, float z) {
    offset({x, y, z});
}

void Object::rotate(glm::vec3 vec, bool global) {
    vec = -glm::radians(vec);
    if (global) {
        rotation_matrix = glm::rotate(rotation_matrix, vec.y, {0, 1, 0});
        rotation_matrix = glm::rotate(rotation_matrix, vec.x, {1, 0, 0});
        rotation_matrix = glm::rotate(rotation_matrix, vec.z, {0, 0, 1});
        setRotationMatrix(rotation_matrix);
    }
    else {
        glm::mat4 m(1);
        m = glm::rotate(m, vec.y, {0, 1, 0});
        m = glm::rotate(m, vec.x, {1, 0, 0});
        m = glm::rotate(m, vec.z, {0, 0, 1});
        setRotationMatrix(m * rotation_matrix);
    }

    for (auto& obj : bindedObjects)
        obj.second->setParentRotationMatrix(getGlobalRotationMatrix());
}
void Object::rotate(float x, float y, float z, bool global) {
    rotate({x, y, z}, global);
}

glm::vec3 Object::getPosition() {
    return this->position;
}

glm::vec3 Object::getRotationEuler() {
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

glm::mat4 Object::getRotationMatrix() {
    return this->rotation_matrix;
}

glm::vec3 Object::getScale() {
    return this->scale;
}


glm::vec3 Object::getGlobalPosition() {
    return this->position + this->parent_position;
}

glm::vec3 Object::getGlobalRotationEuler() {
    return getRotationEuler();
}

glm::mat4 Object::getGlobalRotationMatrix() {
    return this->parent_rotation_matrix * this->rotation_matrix;
}

glm::vec3 Object::getGlobalScale() {
    return this->scale * this->parent_scale;
}



bool Object::getVisible() {
    return this->visible;
}


UUID_Generator::UUID Object::bindObj(Object* obj) {
    UUID_Generator::UUID uuid = global_uuid_generator.gen();
    bindedObjects[uuid] = obj;
    obj->setParentScale(this->scale);
    obj->setParentPosition(this->position);
    obj->setParentRotationMatrix(this->rotation_matrix);
    return uuid;
}

bool Object::unbindObj(UUID_Generator::UUID uuid) {
    auto it = bindedObjects.find(uuid);
    if (it != bindedObjects.end()) {
        bindedObjects.erase(it);
        return true;
    }
    return false;
}
