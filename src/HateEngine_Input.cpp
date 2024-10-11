#include <HateEngine/HateEngine.hpp>

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

using namespace HateEngine;

InputClass::InputClass(Engine* eng) {
    engine = eng;
}

bool InputClass::isKeyPressed(Key key) {
    return glfwGetKey(engine->window, key);
}

glm::vec2 InputClass::getVector(Key left, Key right, Key up, Key down) {
    glm::vec2 vec;
    vec.x = isKeyPressed(right) - isKeyPressed(left);
    // vec.x += isKeyPressed(right);
    vec.y = isKeyPressed(up) - isKeyPressed(down);
    return vec;
}
