#include "Object.h"

using namespace Old3DEngine;

void Object::setPosition(glm::vec3 pos) {
    position = pos;
}

void Object::setPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

glm::vec3 Object::getPosition() {
    return position;
}
