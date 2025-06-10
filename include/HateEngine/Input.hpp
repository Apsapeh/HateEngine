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
        enum ActionKeyType { KEYBOARD, MOUSE };
        struct ActionKey {
            ActionKeyType type;
            union {
                Key key;
                MouseButton button;
            };

            bool operator==(const ActionKey& other) const {
                return type == other.type && (key == other.key || button == other.button);
            }
        };


    private:
        OSDriverInterface::OSWindow* window = nullptr;
        std::unordered_map<std::string, std::vector<ActionKey>> actions_map;

    public:
        InputClass(OSDriverInterface::OSWindow* window);
        bool isKeyPressed(Key key);
        glm::vec2 getVector(Key left, Key right, Key up, Key down);

        glm::vec2 getCursorPosition();
        bool isMouseButtonPressed(MouseButton button);

        bool isActionPressed(std::string action);
        glm::vec2 getVectorAction(std::string left, std::string right, std::string up, std::string down);

        void addKeyToAction(std::string action, Key key);
        void addKeyToAction(std::string action, MouseButton button);
        bool removeKeyFromAction(std::string action, Key key);
        bool removeKeyFromAction(std::string action, MouseButton button);
        bool removeAction(std::string action);
        std::vector<ActionKey> getActionKeys(std::string action);
        bool isKeyInAction(std::string action, Key key);
        bool isKeyInAction(std::string action, MouseButton button);
        // bool isKeyPressed(int key);
        //
        void changeInputWindow(OSDriverInterface::OSWindow* window);
    };
} // namespace HateEngine
