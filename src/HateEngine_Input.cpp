#include <HateEngine/HateEngine.hpp>
#include "HateEngine/Log.hpp"

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

bool InputClass::isActionPressed(std::string action) {
    if (this->actions_map.find(action) == this->actions_map.end()) {
        HATE_WARNING_F("Action %s not found", action.c_str())
        return false;
    }
    for (auto key : this->actions_map[action])
        if (isKeyPressed(key))
            return true;
    return false;
}

glm::vec2 InputClass::getVectorAction(std::string left, std::string right, std::string up, std::string down) {
    glm::vec2 vec;
    vec.x = isActionPressed(right) - isActionPressed(left);
    vec.y = isActionPressed(up) - isActionPressed(down);
    return vec;
}

void InputClass::addKeyToAction(std::string action, Key key) {
    if (this->actions_map.find(action) == this->actions_map.end())
        this->actions_map[action] = std::vector<Key>();
    this->actions_map[action].push_back(key);
}

bool InputClass::removeKeyFromAction(std::string action, Key key) {
    if (this->actions_map.find(action) == this->actions_map.end())
        return false;
    auto it = std::find(this->actions_map[action].begin(), this->actions_map[action].end(), key);
    if (it != this->actions_map[action].end()) {
        this->actions_map[action].erase(it);
        return true;
    }
    return false;
}

bool InputClass::removeAction(std::string action) {
    if (this->actions_map.find(action) == this->actions_map.end())
        return false;
    this->actions_map.erase(action);
    return true;
}

std::vector<Key> InputClass::getActionKeys(std::string action) {
    if (this->actions_map.find(action) == this->actions_map.end())
        return std::vector<Key>();
    return this->actions_map[action];
}