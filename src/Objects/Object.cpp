#include <Old3DEngine/Objects/Object.hpp>
#include "../globalStaticParams.hpp"
//#include <cmath>

//TODO: Добавить дочерние Object, которые наследуют перемещение, вращение и подобное

using namespace Old3DEngine;

void Object::setPosition(glm::vec3 value) {
    this->position = value;
}

void Object::setPosition(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Object::setRotation(glm::vec3 value) {
    //this->rotation = value;
    value = glm::radians(value);
    rotation_matrix = glm::eulerAngleXYZ(value.x, value.y, value.z);
}

void Object::setRotation(float x, float y, float z) {
    //this->rotation = glm::vec3(x, y, z);
    setRotation({x, y, z});
}

void Object::setRotationMatrix(glm::mat4 mat) {
    rotation_matrix = mat;
}

void Object::setScale(glm::vec3 value) {
    this->scale = value;
}

void Object::setScale(float x, float y, float z) {
    this->scale = glm::vec3(x, y, z);
}

void Object::setRelativScale(glm::vec3 value) {
    this->relativ_scale = value;
}

void Object::setRelativScale(float x, float y, float z) {
    this->relativ_scale = glm::vec3(x, y, z);
}

void Object::setVisible(bool vis) {
    this->visible = vis;
}

void Object::offset(float x, float y, float z) {
    position += glm::vec3(x, y, z);
}
void Object::offset(glm::vec3 vec) {
    position += vec;
}
void Object::rotate(float x, float y, float z, bool global) {
    rotate({x, y, z}, global);
}
void Object::rotate(glm::vec3 vec, bool global) {
    vec = -glm::radians(vec);
    if (global) {
        rotation_matrix = glm::rotate(rotation_matrix, vec.y, {0, 1, 0});
        rotation_matrix = glm::rotate(rotation_matrix, vec.x, {1, 0, 0});
        rotation_matrix = glm::rotate(rotation_matrix, vec.z, {0, 0, 1});
    }
    else {
        glm::mat4 m(1);
        m = glm::rotate(m, vec.y, {0, 1, 0});
        m = glm::rotate(m, vec.x, {1, 0, 0});
        m = glm::rotate(m, vec.z, {0, 0, 1});
        rotation_matrix = m * rotation_matrix;
    }
}

glm::vec3 Object::getPosition() {
    return this->position;
}
#include <glm/ext.hpp>
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

glm::vec3 Object::getRelativScale() {
    return relativ_scale;
}



bool Object::getVisible() {
    return this->visible;
}



UUID_Generator::UUID Object::addKind(Object obj) {
    UUID_Generator::UUID uuid = global_uuid_generator.gen();
    kinds[uuid] = std::move(obj);
    return uuid;
}

bool Object::delKind(UUID_Generator::UUID uuid) {
    auto it = kinds.find(uuid);
    if (it != kinds.end()) {
        kinds.erase(it);
        return true;
    }
    return false;
}

Object& Object::getKind(UUID_Generator::UUID uuid) {
    return kinds[uuid];
}
