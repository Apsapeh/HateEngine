#include <HateEngine/HateEngine.hpp>
#include "HateEngine/Input.hpp"
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

glm::vec2 InputClass::getCursorPosition() {
    double x, y;
    glfwGetCursorPos(engine->window, &x, &y);
    return glm::vec2(x, y);
}

bool InputClass::isMouseButtonPressed(MouseButton button) {
    return glfwGetMouseButton(engine->window, button);
}

bool InputClass::isActionPressed(std::string action) {
    if (this->actions_map.find(action) == this->actions_map.end()) {
        HATE_WARNING_F("Action %s not found", action.c_str())
        return false;
    }
    for (auto key: this->actions_map[action]) {
        if (key.type == ActionKeyType::KEYBOARD) {
            if (isKeyPressed(key.key))
                return true;
        } else if (key.type == ActionKeyType::MOUSE) {
            if (isMouseButtonPressed(key.button))
                return true;
        }
    }
    return false;
}

glm::vec2 InputClass::getVectorAction(
        std::string left, std::string right, std::string up, std::string down
) {
    glm::vec2 vec;
    vec.x = isActionPressed(right) - isActionPressed(left);
    vec.y = isActionPressed(up) - isActionPressed(down);
    return vec;
}

void InputClass::addKeyToAction(std::string action, Key key) {
    if (this->actions_map.find(action) == this->actions_map.end())
        this->actions_map[action] = std::vector<ActionKey>();
    ActionKey ak;
    ak.type = ActionKeyType::KEYBOARD;
    ak.key = key;
    this->actions_map[action].push_back(ak);
}

void InputClass::addKeyToAction(std::string action, MouseButton button) {
    if (this->actions_map.find(action) == this->actions_map.end())
        this->actions_map[action] = std::vector<ActionKey>();
    ActionKey ak;
    ak.type = ActionKeyType::MOUSE;
    ak.button = button;
    this->actions_map[action].push_back(ak);
}

bool InputClass::removeKeyFromAction(std::string action, Key key) {
    if (this->actions_map.find(action) == this->actions_map.end())
        return false;
    ActionKey ak;
    ak.type = ActionKeyType::KEYBOARD;
    ak.key = key;
    auto it = std::find(this->actions_map[action].begin(), this->actions_map[action].end(), ak);
    if (it != this->actions_map[action].end()) {
        this->actions_map[action].erase(it);
        return true;
    }
    return false;
}

bool InputClass::removeKeyFromAction(std::string action, MouseButton button) {
    if (this->actions_map.find(action) == this->actions_map.end())
        return false;
    ActionKey ak;
    ak.type = ActionKeyType::MOUSE;
    ak.button = button;
    auto it = std::find(this->actions_map[action].begin(), this->actions_map[action].end(), ak);
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

std::vector<InputClass::ActionKey> InputClass::getActionKeys(std::string action) {
    if (this->actions_map.find(action) == this->actions_map.end())
        return std::vector<ActionKey>();
    return this->actions_map[action];
}

bool InputClass::isKeyInAction(std::string action, Key key) {
    if (this->actions_map.count(action)) {
        ActionKey ak;
        ak.type = ActionKeyType::KEYBOARD;
        ak.key = key;
        auto it = std::find(this->actions_map[action].begin(), this->actions_map[action].end(), ak);
        if (it != this->actions_map[action].end())
            return true;
    }
    return false;
}

bool InputClass::isKeyInAction(std::string action, MouseButton button) {
    if (this->actions_map.count(action)) {
        ActionKey ak;
        ak.type = ActionKeyType::MOUSE;
        ak.button = button;
        auto it = std::find(this->actions_map[action].begin(), this->actions_map[action].end(), ak);
        if (it != this->actions_map[action].end())
            return true;
    }
    return false;
}
