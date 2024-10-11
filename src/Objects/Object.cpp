#include <HateEngine/Objects/Object.hpp>
// #include <cmath>


using namespace HateEngine;

void Object::setParentPosition(const glm::vec3 vec) {
    this->parent_position = vec;
    for (auto& obj: bindedObjects)
        if (obj.second.bind_pos)
            obj.second.obj->setParentPosition(getGlobalPosition());
}

void Object::setParentScale(const glm::vec3 vec) {
    this->parent_scale = vec;
    for (auto& obj: bindedObjects)
        if (obj.second.bind_scale)
            obj.second.obj->setParentScale(getGlobalScale());
}

void Object::setParentRotationMatrix(const glm::mat4& mat) {
    this->parent_rotation_matrix = mat;
    for (auto& obj: bindedObjects)
        if (obj.second.bind_rot)
            obj.second.obj->setParentRotationMatrix(getGlobalRotationMatrix());
}

void Object::setPosition(const glm::vec3 value) {
    this->position = value;
    for (auto& obj: bindedObjects)
        if (obj.second.bind_pos)
            obj.second.obj->setParentPosition(getGlobalPosition());
}
void Object::setPosition(float x, float y, float z) {
    setPosition({x, y, z});
}

void Object::setRotation(glm::vec3 value) {
    value = glm::radians(value);
    rotation_matrix = glm::eulerAngleXYZ(value.x, value.y, value.z);
    for (auto& obj: bindedObjects) {
        if (obj.second.bind_rot)
            obj.second.obj->setParentRotationMatrix(getGlobalRotationMatrix());
        if (obj.second.bind_pos)
            obj.second.obj->setParentPosition(getGlobalPosition());
    }
}
void Object::setRotation(float x, float y, float z) {
    setRotation({x, y, z});
}

void Object::setRotationMatrix(glm::mat4 mat) {
    rotation_matrix = mat;
    for (auto& obj: bindedObjects)
        if (obj.second.bind_rot)
            obj.second.obj->setParentRotationMatrix(getGlobalRotationMatrix());
}

void Object::setScale(glm::vec3 value) {
    this->scale = value;
    for (auto& obj: bindedObjects)
        if (obj.second.bind_scale)
            obj.second.obj->setParentScale(getGlobalScale());
}
void Object::setScale(float x, float y, float z) {
    setScale({x, y, z});
}

void Object::setVisible(bool vis) {
    this->visible = vis;
}

void Object::lookAt(glm::vec3 target) {
    glm::vec3 direction = glm::normalize(target - getGlobalPosition());
    glm::vec3 right = glm::normalize(glm::cross({0, 1, 0}, direction));
    glm::vec3 new_up = glm::cross(direction, right);

    glm::mat4 rotation(1);
    rotation[0] = glm::vec4(right, 0);
    rotation[1] = glm::vec4(new_up, 0);
    rotation[2] = glm::vec4(direction, 0);

    setRotationMatrix(rotation);
}

void Object::lookAt(float x, float y, float z) {
    lookAt({x, y, z});
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
    } else {
        glm::mat4 m(1);
        m = glm::rotate(m, vec.y, {0, 1, 0});
        m = glm::rotate(m, vec.x, {1, 0, 0});
        m = glm::rotate(m, vec.z, {0, 0, 1});
        setRotationMatrix(m * rotation_matrix);
    }

    for (auto& obj: bindedObjects) {
        if (obj.second.bind_rot)
            obj.second.obj->setParentRotationMatrix(getGlobalRotationMatrix());
        if (obj.second.bind_pos)
            obj.second.obj->setParentPosition(getGlobalPosition());
    }
}
void Object::rotate(float x, float y, float z, bool global) {
    rotate({x, y, z}, global);
}

glm::vec3 Object::getPosition() const {
    return this->position;
}

glm::vec3 Object::getRotationEuler() const {
    // XXX Z coord may be wrong
    glm::vec3 rot;
    float tmp_1, tmp_2;

    glm::extractEulerAngleXZY(rotation_matrix, rot.x, tmp_1, tmp_2); // x
    // glm::extractEulerAngleYXZ(rotation_matrix, rot.y, tmp_1, tmp_2);
    glm::extractEulerAngleZXY(rotation_matrix, rot.z, tmp_1, rot.y); // y, z = 0

    rot *= -1;

    rot = glm::degrees(rot);
    rot.y += 90.0f;

    return rot;
}

glm::mat4 Object::getRotationMatrix() const { //
    return this->rotation_matrix;
}

glm::vec3 Object::getScale() const {
    return this->scale;
}

glm::vec3 Object::getGlobalPosition() const {
    if (not binded)
        return this->position;

    glm::vec4 rotatedOffset = this->parent_rotation_matrix * glm::vec4(this->position, 1.0f);
    return this->parent_position + glm::vec3(rotatedOffset);
}

glm::vec3 Object::getGlobalRotationEuler() const {
    /*glm::vec3 rot;
    glm::mat4 global_rotation = getGlobalRotationMatrix();
    glm::extractEulerAngleXYZ(global_rotation, rot.x, rot.y, rot.z);
    rot *= -1;

    // Changes the Y rotation detection limit from [-PI/2, PI/2] to [-P, P]
    bool bad = global_rotation[0][0] == 0 and global_rotation[0][2] == 0;
    if (not bad)
        rot.y = atan2(global_rotation[0][0], -global_rotation[0][2]);
    else
        rot.y = atan2(global_rotation[1][0], -global_rotation[1][2]);

    return glm::degrees(rot);*/
    glm::vec3 rot;
    float tmp_1, tmp_2;
    // glm::extractEulerAngleYXZ(rotation_matrix, rot.y, rot.x, rot.z); //
    // return global rotation glm::extractEulerAngleZXY(rotation_matrix, rot.z,
    // rot.x, rot.y);

    glm::mat4 global_rotation = getGlobalRotationMatrix();
    glm::extractEulerAngleXZY(global_rotation, rot.x, tmp_1, tmp_2); // x
    // glm::extractEulerAngleYXZ(rotation_matrix, rot.y, tmp_1, tmp_2);
    glm::extractEulerAngleZXY(global_rotation, rot.z, tmp_1, rot.y); // y, z = 0

    rot *= -1;

    // Changes the Y rotation detection limit from [-PI/2, PI/2] to [-P, P]
    /*bool bad = rotation_matrix[0][0] == 0 and rotation_matrix[0][2] == 0;
    if (not bad)
        rot.y = atan2(rotation_matrix[0][0], -rotation_matrix[0][2]);
    else
        rot.y = atan2(rotation_matrix[1][0], -rotation_matrix[1][2]);*/
    rot = glm::degrees(rot);
    rot.y += 90.0f;

    /*if (rot.y < 0)
        rot.y += 360.0f;*/

    return rot;
}

glm::mat4 Object::getGlobalRotationMatrix() const {
    if (not binded)
        return this->rotation_matrix;
    return this->parent_rotation_matrix * this->rotation_matrix;
}

glm::vec3 Object::getGlobalScale() const {
    return this->scale * this->parent_scale;
}

bool Object::getVisible() const {
    return this->visible;
}

UUID Object::getUUID() const {
    return this->uuid;
}

UUID Object::bindObj(Object* obj, bool bind_pos, bool bind_rot, bool bind_scale) {
    bindedObjects[obj->getUUID()] = {obj, bind_pos, bind_rot, bind_scale};
    obj->binded = true;
    if (bind_pos)
        obj->setParentPosition(this->position);
    if (bind_rot)
        obj->setParentRotationMatrix(this->rotation_matrix);
    if (bind_scale)
        obj->setParentScale(this->scale);
    return obj->getUUID();
}

bool Object::unbindObj(UUID _uuid) {
    auto it = bindedObjects.find(_uuid);
    if (it != bindedObjects.end()) {
        bindedObjects[_uuid].obj->binded = false;
        bindedObjects.erase(it);
        return true;
    }
    return false;
}
