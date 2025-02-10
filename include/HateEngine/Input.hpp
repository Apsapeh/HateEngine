#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace HateEngine {
    enum Key {
        KeySpace = 32,
        KeyApostrophe = 39, /* ' */
        KeyComma = 44, /* , */
        KeyMinus = 45, /* - */
        KeyPeriod = 46, /* . */
        KeySlash = 47, /* / */
        Key0 = 48,
        Key1 = 49,
        Key2 = 50,
        Key3 = 51,
        Key4 = 52,
        Key5 = 53,
        Key6 = 54,
        Key7 = 55,
        Key8 = 56,
        Key9 = 57,
        KeySemicolon = 59, /* ; */
        KeyEqual = 61, /* = */
        KeyA = 65,
        KeyB = 66,
        KeyC = 67,
        KeyD = 68,
        KeyE = 69,
        KeyF = 70,
        KeyG = 71,
        KeyH = 72,
        KeyI = 73,
        KeyJ = 74,
        KeyK = 75,
        KeyL = 76,
        KeyM = 77,
        KeyN = 78,
        KeyO = 79,
        KeyP = 80,
        KeyQ = 81,
        KeyR = 82,
        KeyS = 83,
        KeyT = 84,
        KeyU = 85,
        KeyV = 86,
        KeyW = 87,
        KeyX = 88,
        KeyY = 89,
        KeyZ = 90,
        KeyLeftBracket = 91, /* [ */
        KeyBackslash = 92, /* \ */
        KeyRightBracket = 93, /* ] */
        KeyGraveAccent = 96, /* ` */
        KeyWorld1 = 161, /* non-US #1 */
        KeyWorld2 = 162, /* non-US #2 */

        /* Function keys */
        KeyEscape = 256,
        KeyEnter = 257,
        KeyTab = 258,
        KeyBackspace = 259,
        KeyInsert = 260,
        KeyDelete = 261,
        KeyRight = 262,
        KeyLeft = 263,
        KeyDown = 264,
        KeyUp = 265,
        KeyPageUp = 266,
        KeyPageDown = 267,
        KeyHome = 268,
        KeyEnd = 269,
        KeyCapsLock = 280,
        KeyScrollLock = 281,
        KeyNumLock = 282,
        KeyPrintScreen = 283,
        KeyPause = 284,
        KeyF1 = 290,
        KeyF2 = 291,
        KeyF3 = 292,
        KeyF4 = 293,
        KeyF5 = 294,
        KeyF6 = 295,
        KeyF7 = 296,
        KeyF8 = 297,
        KeyF9 = 298,
        KeyF10 = 299,
        KeyF11 = 300,
        KeyF12 = 301,
        KeyF13 = 302,
        KeyF14 = 303,
        KeyF15 = 304,
        KeyF16 = 305,
        KeyF17 = 306,
        KeyF18 = 307,
        KeyF19 = 308,
        KeyF20 = 309,
        KeyF21 = 310,
        KeyF22 = 311,
        KeyF23 = 312,
        KeyF24 = 313,
        KeyF25 = 314,
        KeyNumPad0 = 320,
        KeyNumPad1 = 321,
        KeyNumPad2 = 322,
        KeyNumPad3 = 323,
        KeyNumPad4 = 324,
        KeyNumPad5 = 325,
        KeyNumPad6 = 326,
        KeyNumPad7 = 327,
        KeyNumPad8 = 328,
        KeyNumPad9 = 329,
        KeyNumPadDecimal = 330,
        KeyNumPadDivide = 331,
        KeyNumPadMultiply = 332,
        KeyNumPadSubtract = 333,
        KeyNumPadAdd = 334,
        KeyNumPadEnter = 335,
        KeyNumPadEqual = 336,
        KeyLeftShift = 340,
        KeyLeftControl = 341,
        KeyLeftAlt = 342,
        KeyLeftSuper = 343,
        KeyRightShift = 344,
        KeyRightControl = 345,
        KeyRightAlt = 346,
        KeyRightSuper = 347,
        KeyMenu = 348,
    };

    enum MouseButton {
        MouseButton1 = 0,
        MouseButton2 = 1,
        MouseButton3 = 2,
        MouseButton4 = 3,
        MouseButton5 = 4,
        MouseButton6 = 5,
        MouseButton7 = 6,
        MouseButton8 = 7,
        MouseButtonLeft = MouseButton1,
        MouseButtonRight = MouseButton2,
        MouseButtonMiddle = MouseButton3,
    };

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

        enum InputEventType {
            InputEventKey,
            InputEventMouseButton,
            InputEventMouseMove,
            InputEventMouseScroll
        };

        struct InputEventInfo {
            InputEventType type;
            union {
                Key key;
                MouseButton button;
            };
            int raw_key, scancode, mods = 0;
            bool isPressed;
            glm::vec2 offset = {0, 0};
            glm::vec2 position = {0, 0};
        };

    private:
        class Engine* engine;
        std::unordered_map<std::string, std::vector<ActionKey>> actions_map;

    public:
        InputClass(class Engine*);
        bool isKeyPressed(Key key);
        glm::vec2 getVector(Key left, Key right, Key up, Key down);

        glm::vec2 getCursorPosition();
        bool isMouseButtonPressed(MouseButton button);

        bool isActionPressed(std::string action);
        glm::vec2 getVectorAction(
                std::string left, std::string right, std::string up, std::string down
        );

        void addKeyToAction(std::string action, Key key);
        void addKeyToAction(std::string action, MouseButton button);
        bool removeKeyFromAction(std::string action, Key key);
        bool removeKeyFromAction(std::string action, MouseButton button);
        bool removeAction(std::string action);
        std::vector<ActionKey> getActionKeys(std::string action);
        bool isKeyInAction(std::string action, Key key);
        bool isKeyInAction(std::string action, MouseButton button);
        // bool isKeyPressed(int key);
    };
} // namespace HateEngine
