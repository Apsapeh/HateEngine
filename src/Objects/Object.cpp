#include <HateEngine/Objects/Object.hpp>
#include "../globalStaticParams.hpp"
//#include <cmath>

//TODO: Добавить дочерние Object, которые наследуют перемещение, вращение и подобное

using namespace HateEngine;

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
        obj.second->setParentPosition(getGlobalPosition());
    this->position = value;
}
void Object::setPosition(float x, float y, float z) {
    setPosition({x, y, z});
}

void Object::setRotation(glm::vec3 value) {
    value = glm::radians(value);
    rotation_matrix = glm::eulerAngleXYZ(value.x, value.y, value.z);
    for (auto& obj : bindedObjects)
        obj.second->setParentRotationMatrix(getGlobalRotationMatrix());

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

glm::vec3 Object::getPosition() const {
    return this->position;
}

glm::vec3 Object::getRotationEuler() const {
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

glm::mat4 Object::getRotationMatrix() const {
    return this->rotation_matrix;
}

glm::vec3 Object::getScale() const {
    return this->scale;
}


glm::vec3 Object::getGlobalPosition() const {
    if (not binded)
        return this->position;

    // Создаем матрицу трансформации
    glm::mat4 transform = glm::mat4(1.0f);

    // Смещаем объект на расстояние от точки вращения
    transform = glm::translate(transform,  this->position + this->parent_position);

    // Вращаем объект вокруг начала координат
    //transform = glm::rotate(transform, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    //transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    //transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = transform * parent_rotation_matrix;

    // Смещаем объект обратно
    transform = glm::translate(transform, -(this->position + this->parent_position));

    // Применяем трансформацию к точке
    glm::vec4 transformedPoint = transform * glm::vec4(this->parent_position, 1.0f);

    return glm::vec3(transformedPoint);

    //return this->position + this->parent_position;
}

glm::vec3 Object::getGlobalRotationEuler() const {
    return getRotationEuler();
}

glm::mat4 Object::getGlobalRotationMatrix() const {
    return this->parent_rotation_matrix * this->rotation_matrix;
}

glm::vec3 Object::getGlobalScale() const {
    return this->scale * this->parent_scale;
}



bool Object::getVisible() const{
    return this->visible;
}


UUID_Generator::UUID Object::bindObj(Object* obj) {
    UUID_Generator::UUID uuid = global_uuid_generator.gen();
    bindedObjects[uuid] = obj;
    obj->binded = true;
    obj->setParentScale(this->scale);
    obj->setParentPosition(this->position);
    obj->setParentRotationMatrix(this->rotation_matrix);
    return uuid;
}

bool Object::unbindObj(UUID_Generator::UUID uuid) {
    auto it = bindedObjects.find(uuid);
    if (it != bindedObjects.end()) {
        bindedObjects[uuid]->binded = false;
        bindedObjects.erase(it);
        return true;
    }
    return false;
}
