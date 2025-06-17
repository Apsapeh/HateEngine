#pragma once

#include <string>
#include "InputEvent.hpp"
#include "Types/Result.hpp"
#include "Types/Option.hpp"
#include "Utilities/Signal.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <GLFW/glfw3.h>

namespace HateEngine {
    typedef void (*ProcAddr)(void);
#ifdef __HATE_ENGINE_USE_GLFW
    typedef int JoystickHandle;
#else
    typedef int* JoystickHandle;
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
        };

        enum JoysticErr {
            IsNotAvailable,
            IsNotGamepad,
            IsNotRawJoystick,
            OutOfIndex,
            UnknownError,
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
                bool cachedMouseButtons[MouseButton8+1];
            #endif

            void init();
            void swapBuffers();

        public:
            enum WindowMode { Window, Fullscreen, WindowedFullscreen, Error};

            OSWindow();
            ~OSWindow();

            bool isShouldBeClosed();
            glm::vec2 getScale();
            void RequireClose();
            void setTitle(const std::string& title);
            void setSwapInterval(int interval);
            void setWindowSize(int w, int h);
            void setWindowMode(WindowMode mode, OSDisplay* display = nullptr);
            WindowMode getWindowMode();


            void setMouseMode(MouseMode mode);
            MouseMode getMouseMode();
            int getSwapInterval();

            //// Input ////
            bool isKeyPressed(Key key);
            bool isMouseButtonPressed(MouseButton btn);
            glm::vec2 getCursorPosition();


            bool operator==(const OSWindow& other) {
                return this->OSDataPtr == other.OSDataPtr;
            }
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
#endif

        // TODO: Change to normal Err type
        Result<std::shared_ptr<OSWindow>, bool> createWindow(
                int width, int height, const std::string& title, OSDisplay* display = nullptr
        );

        void poll();

    public:
        Signal<std::shared_ptr<OSWindow>, int, int> onFramebufferSizeChanged;
        Signal<std::shared_ptr<OSWindow>, InputEventInfo> onCursorMoved;
        Signal<std::shared_ptr<OSWindow>, InputEventInfo> onKeyPressed;
        Signal<std::shared_ptr<OSWindow>, InputEventInfo> onMouseButtonPressed;
        Signal<std::shared_ptr<OSWindow>, InputEventInfo> onMouseWheelScrolled;


        OSDriverInterface();
        ~OSDriverInterface();

        static ProcAddr getProcAddress(const char* proc);


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

        double getTime();
    };

} // namespace HateEngine
