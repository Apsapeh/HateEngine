#include <HateEngine/HateEngine.hpp>

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

using namespace HateEngine;

Engine::InputClass::InputClass(Engine *eng) {
    engine = eng;
}

bool Engine::InputClass::isKeyPressed(int key) {
    return glfwGetKey(engine->window, key);
}

glm::vec2 Engine::InputClass::getVector(int left, int right, int up, int down) {
    glm::vec2 vec;
    vec.x = isKeyPressed(right) - isKeyPressed(left) ;
    //vec.x += isKeyPressed(right);
    vec.y = isKeyPressed(up) - isKeyPressed(down);
    return vec;
}