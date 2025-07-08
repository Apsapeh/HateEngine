#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include "InputKeys.hpp"
#include "OSDriverInterface.hpp"

namespace HateEngine {
    class InputClass {
    public:
        enum ActionKeyType {
            KeyboardAction,
            MouseButtonAction,
            AnyGamepadButtonAction,
            AnyGamepadAxisAction
        };
        struct ActionKey {
            ActionKeyType type;
            union {
                Key key;
                MouseButton button;
                GamepadButtons gamepad_button;
                GamepadAxis gamepad_axis;
            };

            bool operator==(const ActionKey& other) const;

            static ActionKey fromKey(Key key);
            static ActionKey fromMouseButton(MouseButton button);
            static ActionKey fromGamepadButton(GamepadButtons button);
            static ActionKey fromGamepadAxis(GamepadAxis axis);
        };


    private:
        OSDriverInterface* OSDriver = nullptr;
        std::shared_ptr<OSDriverInterface::OSWindow> window;
        std::unordered_map<std::string, std::vector<ActionKey>> actions_map;

        void addActionKeyToAction(const std::string& action, ActionKey& action_key);
        bool removeActionKeyFromAction(const std::string& action, ActionKey& action_key);
        bool isActionKeyInAction(const std::string& action, ActionKey& action_key);


    public:
        InputClass(OSDriverInterface* OSDriver);
        bool isPhysicalKeyPressed(Key key);
        glm::vec2 getVector(Key left, Key right, Key up, Key down);

        glm::vec2 getCursorPosition();
        bool isMouseButtonPressed(MouseButton button);

        std::vector<JoystickHandle> getAvailableGamepads();
        bool isGamepadAvailable(JoystickHandle gamepad);
        Option<std::string> getGamepadMappedName(JoystickHandle gamepad);
        Option<std::string> getGamepadRawName(JoystickHandle gamepad);
        Result<bool, OSDriverInterface::JoysticErr> isGamepadButtonPressed(
                JoystickHandle gamepad, GamepadButtons button
        );
        Result<float, OSDriverInterface::JoysticErr> getGamepadAxis(
                JoystickHandle gamepad, GamepadAxis axis
        );
        bool isAnyGamepadButtonPressed(GamepadButtons button);
        float getAnyGamepadAxis(GamepadAxis axis, float deadzone = 0.05f);

        bool isActionPressed(const std::string& action, float trashzone = 0.5f);
        float getActionPressed(const std::string& action);
        glm::vec2 getVectorAction(
                const std::string& left, const std::string& right, const std::string& up,
                const std::string& down
        );

        void addKeyToAction(const std::string& action, Key key);
        void addKeyToAction(const std::string& action, MouseButton button);
        void addKeyToAction(const std::string& action, GamepadButtons button);
        void addKeyToAction(const std::string& action, GamepadAxis axis);
        bool removeKeyFromAction(const std::string& action, Key key);
        bool removeKeyFromAction(const std::string& action, MouseButton button);
        bool removeKeyFromAction(const std::string& action, GamepadButtons button);
        bool removeKeyFromAction(const std::string& action, GamepadAxis axis);
        bool removeAction(const std::string& action);
        std::vector<ActionKey> getActionKeys(const std::string& action);
        bool isKeyInAction(const std::string& action, Key key);
        bool isKeyInAction(const std::string& action, MouseButton button);
        bool isKeyInAction(const std::string& action, GamepadButtons button);
        bool isKeyInAction(const std::string& action, GamepadAxis axis);
        // bool isKeyPressed(int key);
        //
        void changeInputWindow(std::shared_ptr<OSDriverInterface::OSWindow> window);
    };
} // namespace HateEngine
