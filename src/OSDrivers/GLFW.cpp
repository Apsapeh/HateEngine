#include <GLFW/glfw3.h>
#include <HateEngine/OSDriverInterface.hpp>
#include <HateEngine/Log.hpp>
#include "HateEngine/Input.hpp"
#include "HateEngine/Types/Result.hpp"

using namespace HateEngine;

static Key getEngineKeyFromGLFWKey(int key);
static MouseButton getEngineMouseButtonFromGLFWMouseButton(int btn);
static int getGLFWKeyFromEngineKey(Key key);
static int getGLFWMouseButtonFromEngineMouseButton(MouseButton btn);

////////////////////    OS Driver     //////////////////////////////
static bool glfw_inited = false;

OSDriverInterface::OSDriverInterface() {
    if (not glfw_inited) {
        if (glfwInit() == GLFW_FALSE) {
            const char* msg;
            int error = glfwGetError(&msg);
            HATE_FATAL_F("GLFW initializaiton error: code=%d, msg=%s", error, msg);
        }
        glfw_inited = true;
    }
}

OSDriverInterface::~OSDriverInterface() {
    glfwTerminate();
}

ProcAddr OSDriverInterface::getProcAddress(const char* proc) {
    return glfwGetProcAddress(proc);
}

Result<OSDriverInterface::OSWindow, bool> OSDriverInterface::createWindow(
        int width, int height, const std::string& title, OSDisplay* display
) {
    OSWindow window;
    window.OSDataPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window.OSDataPtr) {
        GLFWwindow* glfw_window = (GLFWwindow*) window.OSDataPtr;
        glfwSetWindowUserPointer(glfw_window, this);
        glfwSetFramebufferSizeCallback(glfw_window, [](GLFWwindow* win, int w, int h) {
            OSDriverInterface* th = static_cast<OSDriverInterface*>(glfwGetWindowUserPointer(win));
            OSWindow window;
            window.OSDataPtr = win;
            th->onFramebufferSizeChanged.emit(window, w, h);
        });

        glfwSetCursorPosCallback(glfw_window, [](GLFWwindow* win, double x, double y) {
            OSDriverInterface* th = static_cast<OSDriverInterface*>(glfwGetWindowUserPointer(win));
            OSWindow window;
            window.OSDataPtr = win;
            InputEventInfo info;
            info.type = InputEventMouseMove;
            info.position = glm::vec2(x, y);
            th->onCursorMoved.emit(window, info);
        });

        glfwSetKeyCallback(
                glfw_window,
                [](GLFWwindow* win, int key, int scancode, int action, int mods) {
                    if (action == GLFW_REPEAT)
                        return;

                    OSDriverInterface* th =
                            static_cast<OSDriverInterface*>(glfwGetWindowUserPointer(win));
                    OSWindow window;
                    window.OSDataPtr = win;

                    InputEventInfo info;
                    info.type = InputEventKey;
                    info.isPressed = action == GLFW_PRESS;
                    info.key = getEngineKeyFromGLFWKey(key);

                    th->onKeyPressed.emit(window, info);
                }
        );

        glfwSetMouseButtonCallback(glfw_window, [](GLFWwindow* win, int button, int action, int mods) {
            if (action == GLFW_REPEAT)
                return;

            OSDriverInterface* th = static_cast<OSDriverInterface*>(glfwGetWindowUserPointer(win));
            OSWindow window;
            window.OSDataPtr = win;

            InputEventInfo info;
            info.type = InputEventMouseButton;
            info.isPressed = action == GLFW_PRESS;
            info.button = getEngineMouseButtonFromGLFWMouseButton(button);

            double xpos, ypos;
            glfwGetCursorPos(win, &xpos, &ypos);
            info.position = glm::vec2(xpos, ypos);

            th->onMouseButtonPressed.emit(window, info);
        });

        glfwSetScrollCallback(glfw_window, [](GLFWwindow* win, double xoffset, double yoffset) {
            OSDriverInterface* th = static_cast<OSDriverInterface*>(glfwGetWindowUserPointer(win));
            OSWindow window;
            window.OSDataPtr = win;

            InputEventInfo info;
            info.type = InputEventMouseScroll;
            info.offset = glm::vec2(xoffset, yoffset);

            th->onMouseWheelScrolled.emit(window, info);
        });
        return Result<OSWindow, bool>::Ok(window);
    } else {
        return Result<OSWindow, bool>::Err(false);
    }
}

void OSDriverInterface::makeWindowContextCurrent(OSDriverInterface::OSWindow& window) {
    glfwMakeContextCurrent((GLFWwindow*) window.OSDataPtr);
}

void OSDriverInterface::setSwapInterval(int interval) {
    glfwSwapInterval(interval);
}

void OSDriverInterface::pollEvents() {
    glfwPollEvents();
}

double OSDriverInterface::getTime() {
    return glfwGetTime();
}


///////////////////    OS Window    //////////////////////

void OSDriverInterface::OSWindow::swapBuffers() {
    glfwSwapBuffers((GLFWwindow*) this->OSDataPtr);
}

bool OSDriverInterface::OSWindow::isShouldBeClosed() {
    return glfwWindowShouldClose((GLFWwindow*) this->OSDataPtr);
}

glm::vec2 OSDriverInterface::OSWindow::getScale() {
    glm::vec2 r;
    glfwGetWindowContentScale((GLFWwindow*) this->OSDataPtr, &r.x, &r.y);
    return r;
}

void OSDriverInterface::OSWindow::RequireClose() {
    glfwSetWindowShouldClose((GLFWwindow*) this->OSDataPtr, true);
}

void OSDriverInterface::OSWindow::setTitle(const std::string& title) {
    glfwSetWindowTitle((GLFWwindow*) this->OSDataPtr, title.c_str());
}

//// Input ////
bool OSDriverInterface::OSWindow::isKeyPressed(Key key) {
    return glfwGetKey((GLFWwindow*) this->OSDataPtr, getGLFWKeyFromEngineKey(key));
}

bool OSDriverInterface::OSWindow::isMouseButtonPressed(MouseButton btn) {
    return glfwGetMouseButton(
            (GLFWwindow*) this->OSDataPtr, getGLFWMouseButtonFromEngineMouseButton(btn)
    );
}

glm::vec2 OSDriverInterface::OSWindow::getCursorPosition() {
    double x, y;
    glfwGetCursorPos((GLFWwindow*) this->OSDataPtr, &x, &y);
    return glm::vec2(x, y);
}


////////////////////////////    Key mapping    ///////////////////////////////////
static const int glfwKeyFromEnum[] = {
        -1, // KeyInvalid
        GLFW_KEY_SPACE, // KeySpace
        GLFW_KEY_COMMA, // KeyComma
        GLFW_KEY_MINUS, // KeyMinus
        GLFW_KEY_PERIOD, // KeyPeriod
        GLFW_KEY_SLASH, // KeySlash
        GLFW_KEY_0,
        GLFW_KEY_1,
        GLFW_KEY_2,
        GLFW_KEY_3,
        GLFW_KEY_4,
        GLFW_KEY_5,
        GLFW_KEY_6,
        GLFW_KEY_7,
        GLFW_KEY_8,
        GLFW_KEY_9, // Key0 - Key9
        GLFW_KEY_SEMICOLON, // KeySemicolon
        GLFW_KEY_EQUAL, // KeyEqual
        GLFW_KEY_A,
        GLFW_KEY_B,
        GLFW_KEY_C,
        GLFW_KEY_D,
        GLFW_KEY_E,
        GLFW_KEY_F,
        GLFW_KEY_G,
        GLFW_KEY_H,
        GLFW_KEY_I,
        GLFW_KEY_J,
        GLFW_KEY_K,
        GLFW_KEY_L,
        GLFW_KEY_M,
        GLFW_KEY_N,
        GLFW_KEY_O,
        GLFW_KEY_P,
        GLFW_KEY_Q,
        GLFW_KEY_R,
        GLFW_KEY_S,
        GLFW_KEY_T,
        GLFW_KEY_U,
        GLFW_KEY_V,
        GLFW_KEY_W,
        GLFW_KEY_X,
        GLFW_KEY_Y,
        GLFW_KEY_Z, // KeyU - KeyZ
        GLFW_KEY_LEFT_BRACKET, // KeyLeftBracket
        GLFW_KEY_BACKSLASH, // KeyBackslash
        GLFW_KEY_RIGHT_BRACKET, // KeyRightBracket
        GLFW_KEY_GRAVE_ACCENT, // KeyGraveAccent
        GLFW_KEY_WORLD_1, // KeyWorld1
        GLFW_KEY_WORLD_2, // KeyWorld2
        GLFW_KEY_ESCAPE, // KeyEscape
        GLFW_KEY_ENTER, // KeyEnter
        GLFW_KEY_TAB, // KeyTab
        GLFW_KEY_BACKSPACE, // KeyBackspace
        GLFW_KEY_INSERT, // KeyInsert
        GLFW_KEY_DELETE, // KeyDelete
        GLFW_KEY_RIGHT, // KeyRight
        GLFW_KEY_LEFT, // KeyLeft
        GLFW_KEY_DOWN, // KeyDown
        GLFW_KEY_UP, // KeyUp
        GLFW_KEY_PAGE_UP, // KeyPageUp
        GLFW_KEY_PAGE_DOWN, // KeyPageDown
        GLFW_KEY_HOME, // KeyHome
        GLFW_KEY_END, // KeyEnd
        GLFW_KEY_CAPS_LOCK, // KeyCapsLock
        GLFW_KEY_SCROLL_LOCK, // KeyScrollLock
        GLFW_KEY_NUM_LOCK, // KeyNumLock
        GLFW_KEY_PRINT_SCREEN, // KeyPrintScreen
        GLFW_KEY_PAUSE, // KeyPause
        GLFW_KEY_F1,
        GLFW_KEY_F2,
        GLFW_KEY_F3,
        GLFW_KEY_F4,
        GLFW_KEY_F5,
        GLFW_KEY_F6,
        GLFW_KEY_F7,
        GLFW_KEY_F8,
        GLFW_KEY_F9,
        GLFW_KEY_F10,
        GLFW_KEY_F11,
        GLFW_KEY_F12,
        GLFW_KEY_F13,
        GLFW_KEY_F14,
        GLFW_KEY_F15,
        GLFW_KEY_F16,
        GLFW_KEY_F17,
        GLFW_KEY_F18,
        GLFW_KEY_F19,
        GLFW_KEY_F20,
        GLFW_KEY_F21,
        GLFW_KEY_F22,
        GLFW_KEY_F23,
        GLFW_KEY_F24,
        GLFW_KEY_F25,
        GLFW_KEY_KP_0,
        GLFW_KEY_KP_1,
        GLFW_KEY_KP_2,
        GLFW_KEY_KP_3,
        GLFW_KEY_KP_4,
        GLFW_KEY_KP_5,
        GLFW_KEY_KP_6,
        GLFW_KEY_KP_7,
        GLFW_KEY_KP_8,
        GLFW_KEY_KP_9,
        GLFW_KEY_KP_DECIMAL,
        GLFW_KEY_KP_DIVIDE,
        GLFW_KEY_KP_MULTIPLY,
        GLFW_KEY_KP_SUBTRACT,
        GLFW_KEY_KP_ADD,
        GLFW_KEY_KP_ENTER,
        GLFW_KEY_KP_EQUAL,
        GLFW_KEY_LEFT_SHIFT,
        GLFW_KEY_LEFT_CONTROL,
        GLFW_KEY_LEFT_ALT,
        GLFW_KEY_LEFT_SUPER,
        GLFW_KEY_RIGHT_SHIFT,
        GLFW_KEY_RIGHT_CONTROL,
        GLFW_KEY_RIGHT_ALT,
        GLFW_KEY_RIGHT_SUPER,
        GLFW_KEY_MENU
};

#define CASE(glfw_key, enum_key)                                                                        \
    case glfw_key:                                                                                      \
        return enum_key

static Key getEngineKeyFromGLFWKey(int key) {
    switch (key) {
        CASE(GLFW_KEY_SPACE, KeySpace);
        CASE(GLFW_KEY_APOSTROPHE, KeyApostrophe);
        CASE(GLFW_KEY_COMMA, KeyComma);
        CASE(GLFW_KEY_MINUS, KeyMinus);
        CASE(GLFW_KEY_PERIOD, KeyPeriod);
        CASE(GLFW_KEY_SLASH, KeySlash);
        CASE(GLFW_KEY_0, Key0);
        CASE(GLFW_KEY_1, Key1);
        CASE(GLFW_KEY_2, Key2);
        CASE(GLFW_KEY_3, Key3);
        CASE(GLFW_KEY_4, Key4);
        CASE(GLFW_KEY_5, Key5);
        CASE(GLFW_KEY_6, Key6);
        CASE(GLFW_KEY_7, Key7);
        CASE(GLFW_KEY_8, Key8);
        CASE(GLFW_KEY_9, Key9);
        CASE(GLFW_KEY_SEMICOLON, KeySemicolon);
        CASE(GLFW_KEY_EQUAL, KeyEqual);
        CASE(GLFW_KEY_A, KeyA);
        CASE(GLFW_KEY_B, KeyB);
        CASE(GLFW_KEY_C, KeyC);
        CASE(GLFW_KEY_D, KeyD);
        CASE(GLFW_KEY_E, KeyE);
        CASE(GLFW_KEY_F, KeyF);
        CASE(GLFW_KEY_G, KeyG);
        CASE(GLFW_KEY_H, KeyH);
        CASE(GLFW_KEY_I, KeyI);
        CASE(GLFW_KEY_J, KeyJ);
        CASE(GLFW_KEY_K, KeyK);
        CASE(GLFW_KEY_L, KeyL);
        CASE(GLFW_KEY_M, KeyM);
        CASE(GLFW_KEY_N, KeyN);
        CASE(GLFW_KEY_O, KeyO);
        CASE(GLFW_KEY_P, KeyP);
        CASE(GLFW_KEY_Q, KeyQ);
        CASE(GLFW_KEY_R, KeyR);
        CASE(GLFW_KEY_S, KeyS);
        CASE(GLFW_KEY_T, KeyT);
        CASE(GLFW_KEY_U, KeyU);
        CASE(GLFW_KEY_V, KeyV);
        CASE(GLFW_KEY_W, KeyW);
        CASE(GLFW_KEY_X, KeyX);
        CASE(GLFW_KEY_Y, KeyY);
        CASE(GLFW_KEY_Z, KeyZ);
        CASE(GLFW_KEY_LEFT_BRACKET, KeyLeftBracket);
        CASE(GLFW_KEY_BACKSLASH, KeyBackslash);
        CASE(GLFW_KEY_RIGHT_BRACKET, KeyRightBracket);
        CASE(GLFW_KEY_GRAVE_ACCENT, KeyGraveAccent);
        CASE(GLFW_KEY_WORLD_1, KeyWorld1);
        CASE(GLFW_KEY_WORLD_2, KeyWorld2);
        CASE(GLFW_KEY_ESCAPE, KeyEscape);
        CASE(GLFW_KEY_ENTER, KeyEnter);
        CASE(GLFW_KEY_TAB, KeyTab);
        CASE(GLFW_KEY_BACKSPACE, KeyBackspace);
        CASE(GLFW_KEY_INSERT, KeyInsert);
        CASE(GLFW_KEY_DELETE, KeyDelete);
        CASE(GLFW_KEY_RIGHT, KeyRight);
        CASE(GLFW_KEY_LEFT, KeyLeft);
        CASE(GLFW_KEY_DOWN, KeyDown);
        CASE(GLFW_KEY_UP, KeyUp);
        CASE(GLFW_KEY_PAGE_UP, KeyPageUp);
        CASE(GLFW_KEY_PAGE_DOWN, KeyPageDown);
        CASE(GLFW_KEY_HOME, KeyHome);
        CASE(GLFW_KEY_END, KeyEnd);
        CASE(GLFW_KEY_CAPS_LOCK, KeyCapsLock);
        CASE(GLFW_KEY_SCROLL_LOCK, KeyScrollLock);
        CASE(GLFW_KEY_NUM_LOCK, KeyNumLock);
        CASE(GLFW_KEY_PRINT_SCREEN, KeyPrintScreen);
        CASE(GLFW_KEY_PAUSE, KeyPause);
        CASE(GLFW_KEY_F1, KeyF1);
        CASE(GLFW_KEY_F2, KeyF2);
        CASE(GLFW_KEY_F3, KeyF3);
        CASE(GLFW_KEY_F4, KeyF4);
        CASE(GLFW_KEY_F5, KeyF5);
        CASE(GLFW_KEY_F6, KeyF6);
        CASE(GLFW_KEY_F7, KeyF7);
        CASE(GLFW_KEY_F8, KeyF8);
        CASE(GLFW_KEY_F9, KeyF9);
        CASE(GLFW_KEY_F10, KeyF10);
        CASE(GLFW_KEY_F11, KeyF11);
        CASE(GLFW_KEY_F12, KeyF12);
        CASE(GLFW_KEY_F13, KeyF13);
        CASE(GLFW_KEY_F14, KeyF14);
        CASE(GLFW_KEY_F15, KeyF15);
        CASE(GLFW_KEY_F16, KeyF16);
        CASE(GLFW_KEY_F17, KeyF17);
        CASE(GLFW_KEY_F18, KeyF18);
        CASE(GLFW_KEY_F19, KeyF19);
        CASE(GLFW_KEY_F20, KeyF20);
        CASE(GLFW_KEY_F21, KeyF21);
        CASE(GLFW_KEY_F22, KeyF22);
        CASE(GLFW_KEY_F23, KeyF23);
        CASE(GLFW_KEY_F24, KeyF24);
        CASE(GLFW_KEY_F25, KeyF25);
        CASE(GLFW_KEY_KP_0, KeyNumPad0);
        CASE(GLFW_KEY_KP_1, KeyNumPad1);
        CASE(GLFW_KEY_KP_2, KeyNumPad2);
        CASE(GLFW_KEY_KP_3, KeyNumPad3);
        CASE(GLFW_KEY_KP_4, KeyNumPad4);
        CASE(GLFW_KEY_KP_5, KeyNumPad5);
        CASE(GLFW_KEY_KP_6, KeyNumPad6);
        CASE(GLFW_KEY_KP_7, KeyNumPad7);
        CASE(GLFW_KEY_KP_8, KeyNumPad8);
        CASE(GLFW_KEY_KP_9, KeyNumPad9);
        CASE(GLFW_KEY_KP_DECIMAL, KeyNumPadDecimal);
        CASE(GLFW_KEY_KP_DIVIDE, KeyNumPadDivide);
        CASE(GLFW_KEY_KP_MULTIPLY, KeyNumPadMultiply);
        CASE(GLFW_KEY_KP_SUBTRACT, KeyNumPadSubtract);
        CASE(GLFW_KEY_KP_ADD, KeyNumPadAdd);
        CASE(GLFW_KEY_KP_ENTER, KeyNumPadEnter);
        CASE(GLFW_KEY_KP_EQUAL, KeyNumPadEqual);
        CASE(GLFW_KEY_LEFT_SHIFT, KeyLeftShift);
        CASE(GLFW_KEY_LEFT_CONTROL, KeyLeftControl);
        CASE(GLFW_KEY_LEFT_ALT, KeyLeftAlt);
        CASE(GLFW_KEY_LEFT_SUPER, KeyLeftSuper);
        CASE(GLFW_KEY_RIGHT_SHIFT, KeyRightShift);
        CASE(GLFW_KEY_RIGHT_CONTROL, KeyRightControl);
        CASE(GLFW_KEY_RIGHT_ALT, KeyRightAlt);
        CASE(GLFW_KEY_RIGHT_SUPER, KeyRightSuper);
        CASE(GLFW_KEY_MENU, KeyMenu);
        default:
            return KeyMenu; // или другой "invalid" ключ
    }
}

static MouseButton getEngineMouseButtonFromGLFWMouseButton(int btn) {
    return static_cast<MouseButton>(btn);
}


static int getGLFWKeyFromEngineKey(Key key) {
    return glfwKeyFromEnum[key];
}

static int getGLFWMouseButtonFromEngineMouseButton(MouseButton btn) {
    return static_cast<int>(btn);
}
