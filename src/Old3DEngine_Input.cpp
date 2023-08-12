#include "Old3DEngine.h"

using namespace Old3DEngine;

Engine::InputClass::InputClass(Engine *eng) {
    engine = eng;
}

bool Engine::InputClass::isKeyPressed(int key) {
    return glfwGetKey(engine->window, key);
}

glm::vec2 Engine::InputClass::getVector(int left, int right, int up, int down) {
    glm::vec2 vec;
    vec.x -= (float)isKeyPressed(left);
    vec.x += (float)isKeyPressed(right);
    vec.y -= (float)isKeyPressed(up);
    vec.y += (float)isKeyPressed(down);
    return vec;
}
