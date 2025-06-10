#pragma once

#include <string>
#include "InputEvent.hpp"
#include "Types/Result.hpp"
#include "Utilities/Signal.hpp"
#include <glm/glm.hpp>

namespace HateEngine {
    typedef void (*ProcAddr)(void);

    class OSDriverInterface {
    public:
        class OSDisplay {
            friend OSDriverInterface;

        protected:
            void* OSDataPtr = nullptr;
        };

        class OSWindow {
            friend OSDriverInterface;

        protected:
            void* OSDataPtr = nullptr;

        public:
            void swapBuffers();
            bool isShouldBeClosed();
            glm::vec2 getScale();
            void RequireClose();
            void setTitle(const std::string& title);

            //// Input ////
            bool isKeyPressed(Key key);
            bool isMouseButtonPressed(MouseButton btn);
            glm::vec2 getCursorPosition();

            bool operator==(const OSWindow& other) {
                return this->OSDataPtr == other.OSDataPtr;
            }
        };


    protected:
        void* OSDataPtr = nullptr;

    public:
        Signal<OSWindow, int, int> onFramebufferSizeChanged;
        Signal<OSWindow, InputEventInfo> onCursorMoved;
        Signal<OSWindow, InputEventInfo> onKeyPressed;
        Signal<OSWindow, InputEventInfo> onMouseButtonPressed;
        Signal<OSWindow, InputEventInfo> onMouseWheelScrolled;


        OSDriverInterface();
        ~OSDriverInterface();

        static ProcAddr getProcAddress(const char* proc);

        // TODO: Change to normal Err type
        Result<OSWindow, bool> createWindow(
                int width, int height, const std::string& title, OSDisplay* display = nullptr
        );
        void makeWindowContextCurrent(OSWindow& window);
        void setSwapInterval(int interval);

        void pollEvents();
        double getTime();
    };
} // namespace HateEngine
