#pragma once

#include <cstdint>
#include <string>
#include "InputEvent.hpp"
#include "Types/Result.hpp"
#include "Types/Option.hpp"
#include "Utilities/Signal.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace HateEngine {
    typedef void (*ProcAddr)(void);
#ifdef __HATE_ENGINE_USE_GLFW
    typedef int JoystickHandle;
#elif __HATE_ENGINE_USE_SDL3
    typedef uint32_t JoystickHandle;
#else
    typedef int JoystickHandle;
#endif

    class OSDriverInterface {
        friend class Engine;

    public:
        /// Mouse cursor modes
        enum MouseMode {
            Normal, ///< Cursor is visible, can leave the window area
            Hidden, ///< Cursor is hiden, can leave the window area
            Disabled, ///< Cursor is hiden, can't leave the window area
            Captured, ///< Cursor is visible, can't leave the window area
            Error,
            Unknown,
        };

        enum JoysticErr {
            IsNotAvailable,
            IsNotGamepad,
            IsNotRawJoystick,
            OutOfIndex,
            UnknownError,
        };

        enum JoystickState {
            Connected,
            Disconnected,
        };

        class OSDisplay {
            friend OSDriverInterface;

        protected:
            void* OSDataPtr = nullptr;
        };

        class OSWindow {
            friend OSDriverInterface;
            friend class Engine;

        protected:
            void* OSDataPtr = nullptr;
            int swapInterval = 0;

#ifdef __HATE_ENGINE_USE_GLFW
            glm::vec2 cachedCursorPos = glm::vec2(0, 0);
            bool cachedKeys[KeyMenu];
            bool cachedMouseButtons[MouseButton8 + 1];
#elif defined(__HATE_ENGINE_USE_SDL3)
            bool shouldBeClosed = false;
            void* OSContextPtr = nullptr;
            // e
#endif

            void init();
            void swapBuffers();

        public:
            enum WindowMode { Window, Fullscreen, WindowedFullscreen, Error };

            OSWindow();
            ~OSWindow();

            bool isShouldBeClosed();
            glm::vec2 getScale();
            void requireClose();
            void setTitle(const std::string& title);
            void setSwapInterval(int interval);
            void setWindowSize(int w, int h);
            void setWindowMode(WindowMode mode, OSDisplay* display = nullptr);
            WindowMode getWindowMode();


            void setMouseMode(MouseMode mode);
            MouseMode getMouseMode();
            int getSwapInterval();

            //// Input ////
            bool isPhysicalKeyPressed(Key key);
            bool isMouseButtonPressed(MouseButton btn);
            glm::vec2 getCursorPosition();


            bool operator==(const OSWindow& other) {
                return this->OSDataPtr == other.OSDataPtr;
            }
        };
        
        class OSJoystick {
          
        };
        
        class OSGamepad {
        public:
            static Option<std::shared_ptr<OSGamepad>> open();
        };


    protected:
// Sorry, I don't know a clearer way to make a platform-specified fields
#ifdef __HATE_ENGINE_USE_GLFW
        struct JoystickData {
            bool isPresent = false;
            bool isGamepad = false;
            std::vector<bool> buttons;
            std::vector<float> axes;

            JoystickData() {
                buttons.reserve(15);
                axes.reserve(6);
            }
        };
        JoystickData joysticksData[16];
#else
        void* OSDataPtr = nullptr;
        std::unordered_map<uint32_t, std::shared_ptr<OSWindow>> windows;
#endif

        // TODO: Change to normal Err type
        Result<std::shared_ptr<OSWindow>, bool> createWindow(
                int width, int height, const std::string& title, OSDisplay* display = nullptr
        );

        void poll();

    public:
        Signal<std::shared_ptr<OSWindow>, int, int> onFramebufferSizeChanged;
        Signal<std::shared_ptr<OSWindow>, InputEventInfo> onCursorMoved;
        Signal<std::shared_ptr<OSWindow>, InputEventInfo> onKeyChanged;
        Signal<std::shared_ptr<OSWindow>, InputEventInfo> onMouseButtonChanged;
        Signal<std::shared_ptr<OSWindow>, InputEventInfo> onMouseWheelScrolled;

        Signal<JoystickHandle, JoystickState> onGamepadNewState;
        Signal<JoystickHandle, JoystickState> onJoystickNewState;
        Signal<JoystickHandle, InputEventInfo> onGamepadButtonChanged;
        Signal<JoystickHandle, InputEventInfo> onGamepadAxisMoved;


        OSDriverInterface();
        ~OSDriverInterface();

        static ProcAddr getProcAddress(const char* proc);

        double getTime();

        bool isShouldBeClosed();
        void requireClose();

        std::vector<JoystickHandle> getAvailableJoysticks();
        std::vector<JoystickHandle> getAvailableGamepads();
        bool isJoystickAvailable(JoystickHandle handle);
        bool isJoystickGamepad(JoystickHandle handle);
        Result<std::vector<bool>, JoysticErr> getJoystickButtonsRaw(JoystickHandle handle);
        Result<std::vector<float>, JoysticErr> getJoystickAxisRaw(JoystickHandle handle);
        Result<bool, JoysticErr> isJoystickButtonPressed(JoystickHandle handle, int button);
        Result<float, JoysticErr> getJoystickAxis(JoystickHandle handle, int axis);
        Result<bool, JoysticErr> isGamepadButtonPressed(JoystickHandle handle, GamepadButtons button);
        Result<float, JoysticErr> getGamepadAxis(JoystickHandle handle, GamepadAxis axis);
        Option<std::string> getJoystickName(JoystickHandle handle);
        Option<std::string> getGamepadName(JoystickHandle handle);

        void makeWindowContextCurrent(OSWindow& window);
    };

} // namespace HateEngine
