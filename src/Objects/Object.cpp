#include "Object.h"

using namespace Old3DEngine;

void Object::setPosition(glm::vec3 value) {
    this->position = value;
}

void Object::setPosition(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Object::setRotation(glm::vec3 value) {
    this->rotation = value;
}

void Object::setRotation(float x, float y, float z) {
    this->rotation = glm::vec3(x, y, z);
}

void Object::setScale(glm::vec3 value) {
    this->scale = value;
}

void Object::setScale(float x, float y, float z) {
    this->scale = glm::vec3(x, y, z);
}

void Object::setVisible(bool vis) {
    this->visible = vis;
}



glm::vec3 Object::getPosition() {
    return this->position;
}

glm::vec3 Object::getRotation() {
    return this->rotation;
}

glm::vec3 Object::getScale() {
    return this->scale;
}

bool Object::getVisible() {
    return this->visible;
}