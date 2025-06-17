#include <HateEngine/HateEngine.hpp>
#include "HateEngine/Input.hpp"
#include "HateEngine/Log.hpp"
#include "HateEngine/OSDriverInterface.hpp"
#include <json.hpp>

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

using namespace HateEngine;

InputClass::InputClass(OSDriverInterface* driver) {
    this->OSDriver = driver;
}

bool InputClass::isKeyPressed(Key key) {
    return window->isKeyPressed(key);
}

glm::vec2 InputClass::getVector(Key left, Key right, Key up, Key down) {
    glm::vec2 vec;
    vec.x = isKeyPressed(right) - isKeyPressed(left);
    vec.y = isKeyPressed(up) - isKeyPressed(down);
    return vec;
}


glm::vec2 InputClass::getCursorPosition() {
    return window->getCursorPosition();
}

bool InputClass::isMouseButtonPressed(MouseButton button) {
    return window->isMouseButtonPressed(button);
}


std::vector<JoystickHandle> InputClass::getAvailableGamepads() {
    return this->OSDriver->getAvailableGamepads();
}

bool InputClass::isGamepadAvailable(JoystickHandle gamepad) {
    return this->OSDriver->isJoystickAvailable(gamepad) && this->OSDriver->isJoystickGamepad(gamepad);
}

Option<std::string> InputClass::getGamepadMappedName(JoystickHandle gamepad) {
    return this->OSDriver->getGamepadName(gamepad);
}

Option<std::string> InputClass::getGamepadRawName(JoystickHandle gamepad) {
    return this->OSDriver->getJoystickName(gamepad);
}

Result<bool, OSDriverInterface::JoysticErr> InputClass::isGamepadButtonPressed(
        JoystickHandle gamepad, GamepadButtons button
) {
    return this->OSDriver->isGamepadButtonPressed(gamepad, button);
}

Result<float, OSDriverInterface::JoysticErr> InputClass::getGamepadAxis(
        JoystickHandle gamepad, GamepadAxis axis
) {
    return this->OSDriver->getGamepadAxis(gamepad, axis);
}

bool InputClass::isAnyGamepadButtonPressed(GamepadButtons button) {
    const auto gamepads = this->getAvailableGamepads();
    for (const auto& gamepad: gamepads) {
        auto state = isGamepadButtonPressed(gamepad, button);
        if (state.isOk() && state.unwrap())
            return true;
    }
    return false;
}

float InputClass::getAnyGamepadAxis(GamepadAxis axis, float deadzone) {
    const auto gamepads = this->getAvailableGamepads();
    for (const auto& gamepad: gamepads) {
        auto state = getGamepadAxis(gamepad, axis);
        if (state.isOk() and std::abs(state.unwrap()) > deadzone)
            return state.unwrap();
    }
    return 0.0f;
}


////////////////////////////////////     ACTIONS     //////////////////////////////////////////////

bool InputClass::isActionPressed(const std::string& action, float trashzone) {
    if (this->actions_map.find(action) == this->actions_map.end()) {
        HATE_WARNING_F("Action %s not found", action.c_str())
        return false;
    }
    for (const auto key: this->actions_map[action]) {
        if (key.type == ActionKeyType::KeyboardAction) {
            if (isKeyPressed(key.key))
                return true;
        } else if (key.type == ActionKeyType::MouseButtonAction) {
            if (isMouseButtonPressed(key.button))
                return true;
        } else if (key.type == ActionKeyType::AnyGamepadButtonAction) {
            if (isAnyGamepadButtonPressed(key.gamepad_button))
                return true;
        } else if (key.type == ActionKeyType::AnyGamepadAxisAction) {
            float value = getAnyGamepadAxis(key.gamepad_axis, trashzone);
            if (value > trashzone)
                return true;
        }
    }
    return false;
}

float InputClass::getActionPressed(const std::string& action) {
    if (this->actions_map.find(action) == this->actions_map.end()) {
        HATE_WARNING_F("Action %s not found", action.c_str())
        return false;
    }
    float max_value = 0.0f;
    for (const auto key: this->actions_map[action]) {
        if (key.type == ActionKeyType::KeyboardAction) {
            if (isKeyPressed(key.key))
                return 1.0f;
        } else if (key.type == ActionKeyType::MouseButtonAction) {
            if (isMouseButtonPressed(key.button))
                return 1.0f;
        } else if (key.type == ActionKeyType::AnyGamepadButtonAction) {
            if (isAnyGamepadButtonPressed(key.gamepad_button))
                return 1.0f;
        } else if (key.type == ActionKeyType::AnyGamepadAxisAction) {
            float value = getAnyGamepadAxis(key.gamepad_axis);
            if (std::abs(value) > std::abs(max_value)) {
                max_value = value;
            }
        }
    }
    return max_value;
}

glm::vec2 InputClass::getVectorAction(
        const std::string& left, const std::string& right, const std::string& up, const std::string& down
) {
    glm::vec2 result;
    result.x = getActionPressed(right) - getActionPressed(left);
    result.y = getActionPressed(up) - getActionPressed(down);
    return result;
}

void InputClass::addActionKeyToAction(const std::string& action, ActionKey& action_key) {
    if (this->actions_map.find(action) == this->actions_map.end())
        this->actions_map[action] = std::vector<ActionKey>();
    this->actions_map[action].push_back(action_key);
}

bool InputClass::removeActionKeyFromAction(const std::string& action, ActionKey& action_key) {
    if (this->actions_map.find(action) == this->actions_map.end())
        return false;
    auto it = std::find(this->actions_map[action].begin(), this->actions_map[action].end(), action_key);
    if (it != this->actions_map[action].end()) {
        this->actions_map[action].erase(it);
        return true;
    }
    return false;
}

bool InputClass::isActionKeyInAction(const std::string& action, ActionKey& action_key) {
    if (this->actions_map.count(action)) {
        auto it = std::find(
                this->actions_map[action].begin(), this->actions_map[action].end(), action_key
        );
        if (it != this->actions_map[action].end())
            return true;
    }
    return false;
}

void InputClass::addKeyToAction(const std::string& action, Key key) {
    ActionKey action_key = ActionKey::fromKey(key);
    this->addActionKeyToAction(action, action_key);
}

void InputClass::addKeyToAction(const std::string& action, MouseButton button) {
    ActionKey action_key = ActionKey::fromMouseButton(button);
    this->addActionKeyToAction(action, action_key);
}

void InputClass::addKeyToAction(const std::string& action, GamepadButtons button) {
    ActionKey action_key = ActionKey::fromGamepadButton(button);
    this->addActionKeyToAction(action, action_key);
}

void InputClass::addKeyToAction(const std::string& action, GamepadAxis axis) {
    ActionKey action_key = ActionKey::fromGamepadAxis(axis);
    this->addActionKeyToAction(action, action_key);
}

bool InputClass::removeKeyFromAction(const std::string& action, Key key) {
    ActionKey action_key = ActionKey::fromKey(key);
    return this->removeActionKeyFromAction(action, action_key);
}

bool InputClass::removeKeyFromAction(const std::string& action, MouseButton button) {
    ActionKey action_key = ActionKey::fromMouseButton(button);
    return this->removeActionKeyFromAction(action, action_key);
}

bool InputClass::removeKeyFromAction(const std::string& action, GamepadButtons button) {
    ActionKey action_key = ActionKey::fromGamepadButton(button);
    return this->removeActionKeyFromAction(action, action_key);
}

bool InputClass::removeKeyFromAction(const std::string& action, GamepadAxis axis) {
    ActionKey action_key = ActionKey::fromGamepadAxis(axis);
    return this->removeActionKeyFromAction(action, action_key);
}

bool InputClass::removeAction(const std::string& action) {
    if (this->actions_map.find(action) == this->actions_map.end())
        return false;
    this->actions_map.erase(action);
    return true;
}

std::vector<InputClass::ActionKey> InputClass::getActionKeys(const std::string& action) {
    if (this->actions_map.find(action) == this->actions_map.end())
        return std::vector<ActionKey>();
    return this->actions_map[action];
}

bool InputClass::isKeyInAction(const std::string& action, Key key) {
    ActionKey action_key = ActionKey::fromKey(key);
    return this->isActionKeyInAction(action, action_key);
}

bool InputClass::isKeyInAction(const std::string& action, MouseButton button) {
    ActionKey action_key = ActionKey::fromMouseButton(button);
    return this->isActionKeyInAction(action, action_key);
}

bool InputClass::isKeyInAction(const std::string& action, GamepadButtons button) {
    ActionKey action_key = ActionKey::fromGamepadButton(button);
    return this->isActionKeyInAction(action, action_key);
}

bool InputClass::isKeyInAction(const std::string& action, GamepadAxis axis) {
    ActionKey action_key = ActionKey::fromGamepadAxis(axis);
    return this->isActionKeyInAction(action, action_key);
}

void InputClass::changeInputWindow(std::shared_ptr<OSDriverInterface::OSWindow> win) {
    this->window = win;
}


/////////////   ActionKey   //////////////

bool InputClass::ActionKey::operator==(const ActionKey& other) const {
    return type == other.type && (key == other.key || button == other.button);
}

InputClass::ActionKey InputClass::ActionKey::fromKey(Key key) {
    ActionKey ak;
    ak.type = InputClass::KeyboardAction;
    ak.key = key;
    return ak;
}

InputClass::ActionKey InputClass::ActionKey::fromMouseButton(MouseButton button) {
    ActionKey ak;
    ak.type = InputClass::MouseButtonAction;
    ak.button = button;
    return ak;
}

InputClass::ActionKey InputClass::ActionKey::fromGamepadButton(GamepadButtons button) {
    ActionKey ak;
    ak.type = InputClass::AnyGamepadButtonAction;
    ak.gamepad_button = button;
    return ak;
}

InputClass::ActionKey InputClass::ActionKey::fromGamepadAxis(GamepadAxis axis) {
    ActionKey ak;
    ak.type = InputClass::AnyGamepadAxisAction;
    ak.gamepad_axis = axis;
    return ak;
}
