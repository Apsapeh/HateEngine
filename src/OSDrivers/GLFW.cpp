#ifdef __HATE_ENGINE_USE_GLFW
#include "HateEngine/HateEngine.hpp"


#include <GLFW/glfw3.h>
#include <HateEngine/OSDriverInterface.hpp>
#include <HateEngine/Log.hpp>
#include <memory>
#include "HateEngine/Input.hpp"
#include "HateEngine/Types/Result.hpp"
#include <array>
#include <thread>

using namespace HateEngine;

static Key getEngineKeyFromGLFWKey(int key);
static MouseButton getEngineMouseButtonFromGLFWMouseButton(int btn);
static int getGLFWGamepadAxisFromEngineJoystickAxis(GamepadAxis axis);

////////////////////    OS Driver     //////////////////////////////
static bool glfw_inited = false;

OSDriverInterface::OSDriverInterface() {
    if (not glfw_inited) {
        if (glfwInit() == GLFW_FALSE) {
            const char* msg;
            int error = glfwGetError(&msg);
            HATE_FATAL_F("GLFW initialization error: code=%d, msg=%s", error, msg);
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

Result<std::shared_ptr<OSDriverInterface::OSWindow>, bool> OSDriverInterface::createWindow(
        int width, int height, const std::string& title, OSDisplay* display
) {
    std::shared_ptr<OSWindow> window = std::make_shared<OSWindow>();
    window->OSDataPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (window->OSDataPtr) {
        GLFWwindow* glfw_window = (GLFWwindow*) window->OSDataPtr;
        makeWindowContextCurrent(*window);
        glfwSetWindowUserPointer(
                glfw_window, new std::pair<OSDriverInterface*, std::shared_ptr<OSWindow>>(this, window)
        );
        glfwSetFramebufferSizeCallback(glfw_window, [](GLFWwindow* win, int w, int h) {
            std::pair<OSDriverInterface*, std::shared_ptr<OSWindow>>* data =
                    static_cast<std::pair<OSDriverInterface*, std::shared_ptr<OSWindow>>*>(
                            glfwGetWindowUserPointer(win)
                    );
            OSDriverInterface* th = data->first;
            std::shared_ptr<OSWindow> window = data->second;
            th->onFramebufferSizeChanged.emit(window, w, h);
        });

        glfwSetCursorPosCallback(glfw_window, [](GLFWwindow* win, double x, double y) {
            auto* data = static_cast<std::pair<OSDriverInterface*, std::shared_ptr<OSWindow>>*>(
                    glfwGetWindowUserPointer(win)
            );
            OSDriverInterface* th = data->first;
            std::shared_ptr<OSWindow> window = data->second;
            InputEventInfo info;
            info.type = InputEventMouseMove;
            info.position = glm::vec2(x, y);

            // Caching because glfwGetCursorPos not thread safe
            window->cachedCursorPos = info.position;

            th->onCursorMoved.emit(window, info);
        });

        glfwSetKeyCallback(
                glfw_window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
                    if (action == GLFW_REPEAT)
                        return;

                    auto* data = static_cast<std::pair<OSDriverInterface*, std::shared_ptr<OSWindow>>*>(
                            glfwGetWindowUserPointer(win)
                    );
                    OSDriverInterface* th = data->first;
                    std::shared_ptr<OSWindow> window = data->second;

                    InputEventInfo info;
                    info.type = InputEventKey;
                    info.isPressed = action == GLFW_PRESS;
                    info.key = getEngineKeyFromGLFWKey(key);

                    // Caching keys because glfwGetKey not thread safe
                    window->cachedKeys[info.key - 1] = info.isPressed;

                    th->onKeyChanged.emit(window, info);
                }
        );

        glfwSetMouseButtonCallback(glfw_window, [](GLFWwindow* win, int button, int action, int mods) {
            if (action == GLFW_REPEAT)
                return;

            auto* data = static_cast<std::pair<OSDriverInterface*, std::shared_ptr<OSWindow>>*>(
                    glfwGetWindowUserPointer(win)
            );
            OSDriverInterface* th = data->first;
            std::shared_ptr<OSWindow> window = data->second;

            InputEventInfo info;
            info.type = InputEventMouseButton;
            info.isPressed = action == GLFW_PRESS;
            info.button = getEngineMouseButtonFromGLFWMouseButton(button);

            window->cachedMouseButtons[info.button] = info.isPressed;

            double xpos, ypos;
            glfwGetCursorPos(win, &xpos, &ypos);
            info.position = glm::vec2(xpos, ypos);

            th->onMouseButtonChanged.emit(window, info);
        });

        glfwSetScrollCallback(glfw_window, [](GLFWwindow* win, double xoffset, double yoffset) {
            std::pair<OSDriverInterface*, std::shared_ptr<OSWindow>>* data =
                    static_cast<std::pair<OSDriverInterface*, std::shared_ptr<OSWindow>>*>(
                            glfwGetWindowUserPointer(win)
                    );
            OSDriverInterface* th = data->first;
            std::shared_ptr<OSWindow> window = data->second;

            InputEventInfo info;
            info.type = InputEventMouseScroll;
            info.offset = glm::vec2(xoffset, yoffset);

            th->onMouseWheelScrolled.emit(window, info);
        });

        return Result<std::shared_ptr<OSWindow>, bool>::Ok(window);
    } else {
        return Result<std::shared_ptr<OSWindow>, bool>::Err(false);
    }
}

void OSDriverInterface::makeWindowContextCurrent(OSDriverInterface::OSWindow& window) {
    glfwMakeContextCurrent((GLFWwindow*) window.OSDataPtr);
}

void OSDriverInterface::poll() {
    glfwPollEvents();

    // Update joystick data
    static_assert(GLFW_JOYSTICK_1 == 0, "GLFW API is updated: GLFW_JOYSTICK_1 != 0");
    GLFWgamepadstate GState;
    for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; ++i) {
        auto& jdata = this->joysticksData[i];
        bool last_is_present = jdata.isPresent;
        if (not glfwJoystickPresent(i)) {
            if (jdata.isPresent) {
                if (jdata.isGamepad)
                    onGamepadNewState.emit(i, Disconnected);
                else
                    onJoystickNewState.emit(i, Disconnected);
            }
            jdata.isPresent = false;
            jdata.isGamepad = false;
            continue;
        }

        jdata.isPresent = true;

        if (glfwJoystickIsGamepad(i)) {
            if (not last_is_present) {
                onGamepadNewState.emit(i, Connected);
            }

            jdata.isGamepad = true;
            glfwGetGamepadState(i, &GState);

            for (int b = 0; b <= sizeof(GState.buttons) / sizeof(GState.buttons[0]); ++b) {
                if (b < jdata.buttons.size() and jdata.buttons[b] != GState.buttons[b]) {
                    InputEventInfo info;
                    info.type = InputEventGamepadButton;
                    info.gamepadButton = static_cast<GamepadButtons>(b);
                    info.isPressed = GState.buttons[b];
                    onGamepadButtonChanged.emit(i, info);
                }
            }

            static_assert(
                    GamepadAxisRightTrigger == 13,
                    "Enum GamepadAxis was changed, need to update this function"
            );
            for (int a = 0; a <= sizeof(GState.axes) / sizeof(GState.axes[0]); ++a) {
                if (a < jdata.axes.size() and jdata.axes[a] != GState.axes[a]) {
                    InputEventInfo info;
                    info.type = InputEventGamepadAxis;
                    info.gamepadAxis = static_cast<GamepadAxis>(a + GamepadAxisLeftX);
                    info.value = GState.axes[a];
                    onGamepadButtonChanged.emit(i, info);
                }
            }

            jdata.buttons.assign(GState.buttons, GState.buttons + 15);
            jdata.axes.assign(GState.axes, GState.axes + 6);
        } else {
            if (not last_is_present) {
                onJoystickNewState.emit(i, Connected);
            }
            jdata.isGamepad = false;

            int count;
            const auto buttons = glfwGetJoystickButtons(i, &count);
            jdata.buttons.assign(buttons, buttons + count);
            const auto axes = glfwGetJoystickAxes(i, &count);
            jdata.axes.assign(axes, axes + count);
        }
    }
}

double OSDriverInterface::getTime() {
    return glfwGetTime();
}


std::vector<JoystickHandle> OSDriverInterface::getAvailableJoysticks() {
    std::vector<JoystickHandle> result;
    result.reserve(16);

    static_assert(GLFW_JOYSTICK_1 == 0, "GLFW API is updated: GLFW_JOYSTICK_1 != 0");
    int i = 0;
    for (const auto& jdata: this->joysticksData) {
        if (jdata.isPresent)
            result.push_back(i);
        ++i;
    }
    return result;
}
std::vector<JoystickHandle> OSDriverInterface::getAvailableGamepads() {
    std::vector<JoystickHandle> result;
    result.reserve(16);

    static_assert(GLFW_JOYSTICK_1 == 0, "GLFW API is updated: GLFW_JOYSTICK_1 != 0");
    int i = 0;
    for (const auto& jdata: this->joysticksData) {
        if (jdata.isPresent and jdata.isGamepad)
            result.push_back(i);
        ++i;
    }
    return result;
}

bool OSDriverInterface::isJoystickAvailable(JoystickHandle handle) {
    return this->joysticksData[handle].isPresent;
}

bool OSDriverInterface::isJoystickGamepad(JoystickHandle handle) {
    return this->joysticksData[handle].isGamepad;
}

Result<std::vector<bool>, OSDriverInterface::JoysticErr> OSDriverInterface::getJoystickButtonsRaw(
        JoystickHandle handle
) {
    const auto& jdata = this->joysticksData[handle];
    if (not jdata.isPresent)
        return Err<std::vector<bool>, JoysticErr>(IsNotAvailable);
    if (jdata.isGamepad)
        return Err<std::vector<bool>, JoysticErr>(IsNotRawJoystick);
    return Ok<std::vector<bool>, JoysticErr>(jdata.buttons);
}

Result<std::vector<float>, OSDriverInterface::JoysticErr> OSDriverInterface::getJoystickAxisRaw(
        JoystickHandle handle
) {
    const auto& jdata = this->joysticksData[handle];
    if (not jdata.isPresent)
        return Err<std::vector<float>, JoysticErr>(IsNotAvailable);
    if (jdata.isGamepad)
        return Err<std::vector<float>, JoysticErr>(IsNotRawJoystick);
    return Ok<std::vector<float>, JoysticErr>(jdata.axes);
}

Result<bool, OSDriverInterface::JoysticErr> OSDriverInterface::isJoystickButtonPressed(
        JoystickHandle handle, int button
) {
    const auto& jdata = this->joysticksData[handle];
    if (not jdata.isPresent)
        return Err<bool, JoysticErr>(IsNotAvailable);
    if (jdata.isGamepad)
        return Err<bool, JoysticErr>(IsNotRawJoystick);
    if (jdata.buttons.size() >= button)
        return Err<bool, JoysticErr>(OutOfIndex);
    return Ok<bool, JoysticErr>(jdata.buttons[button]);
}

Result<float, OSDriverInterface::JoysticErr> OSDriverInterface::getJoystickAxis(
        JoystickHandle handle, int axis
) {
    const auto& jdata = this->joysticksData[handle];
    if (not jdata.isPresent)
        return Err<float, JoysticErr>(IsNotAvailable);
    if (jdata.isGamepad)
        return Err<float, JoysticErr>(IsNotRawJoystick);
    if (jdata.axes.size() >= axis)
        return Err<float, JoysticErr>(OutOfIndex);
    return Ok<float, JoysticErr>(jdata.axes[axis]);
}

Result<bool, OSDriverInterface::JoysticErr> OSDriverInterface::isGamepadButtonPressed(
        JoystickHandle handle, GamepadButtons button
) {
    const auto& jdata = this->joysticksData[handle];
    if (not jdata.isPresent)
        return Err<bool, JoysticErr>(IsNotAvailable);
    if (not jdata.isGamepad)
        return Err<bool, JoysticErr>(IsNotGamepad);
    return Ok<bool, JoysticErr>(jdata.buttons[button]);
}

Result<float, OSDriverInterface::JoysticErr> OSDriverInterface::getGamepadAxis(
        JoystickHandle handle, GamepadAxis axis
) {
    const auto& jdata = this->joysticksData[handle];
    if (not jdata.isPresent)
        return Err<float, JoysticErr>(IsNotAvailable);
    if (not jdata.isGamepad)
        return Err<float, JoysticErr>(IsNotGamepad);
    float value = jdata.axes[getGLFWGamepadAxisFromEngineJoystickAxis(axis)];
    if ((axis == GamepadAxisLeftXLeft or axis == GamepadAxisRightXLeft) and value < 0.0f) {
        return Ok<float, JoysticErr>(-value);
    } else if ((axis == GamepadAxisLeftXRight or axis == GamepadAxisRightXRight) and value > 0.0f) {
        return Ok<float, JoysticErr>(value);
    } else if ((axis == GamepadAxisLeftYUp or axis == GamepadAxisRightYUp) and value < 0.0f) {
        return Ok<float, JoysticErr>(-value);
    } else if ((axis == GamepadAxisLeftYDown or axis == GamepadAxisRightYDown) and value > 0.0f) {
        return Ok<float, JoysticErr>(value);
    } else if (axis == GamepadAxisLeftY or axis == GamepadAxisRightY) {
        return Ok<float, JoysticErr>(-value);
    } else if (axis >= GamepadAxisLeftX)
        return Ok<float, JoysticErr>(value);
    else
        return Ok<float, JoysticErr>(0.0f);
}

Option<std::string> OSDriverInterface::getJoystickName(JoystickHandle handle) {
    auto cstr = glfwGetJoystickName(handle);
    if (cstr == nullptr)
        return None<std::string>();
    return Some(std::string(cstr));
}

Option<std::string> OSDriverInterface::getGamepadName(JoystickHandle handle) {
    auto cstr = glfwGetGamepadName(handle);
    if (cstr == nullptr)
        return None<std::string>();
    return Some(std::string(cstr));
}


///////////////////    OS Window    //////////////////////

OSDriverInterface::OSWindow::OSWindow() {
    std::fill(cachedKeys, cachedKeys + KeyMenu, false);
    std::fill(cachedMouseButtons, cachedMouseButtons + MouseButton8 + 1, false);
}

OSDriverInterface::OSWindow::~OSWindow() {
    glfwDestroyWindow((GLFWwindow*) this->OSDataPtr);
    auto* data = static_cast<std::pair<OSDriverInterface*, std::shared_ptr<OSWindow>>*>(
            glfwGetWindowUserPointer((GLFWwindow*) this->OSDataPtr)
    );
    delete data;
}

void OSDriverInterface::OSWindow::init() {
    setSwapInterval(this->swapInterval);
}

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
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::setTitle must be called only from main thread or via "
                "Engine::callDeferred/callDeferredAsync"
        )
        return;
    }
    glfwSetWindowTitle((GLFWwindow*) this->OSDataPtr, title.c_str());
}

void OSDriverInterface::OSWindow::setSwapInterval(int interval) {
    glfwSwapInterval(interval);
    this->swapInterval = interval;
}

void OSDriverInterface::OSWindow::setWindowSize(int w, int h) {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::setWindowSize must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return;
    }
    glfwSetWindowSize((GLFWwindow*) this->OSDataPtr, w, h);
}


void OSDriverInterface::OSWindow::setWindowMode(WindowMode mode, OSDisplay* display) {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::setWindowMode must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return;
    }
    // TODO: Add custom display for Fullscreen and WindowedFullscreen
    GLFWwindow* window = static_cast<GLFWwindow*>(this->OSDataPtr);

    if (mode == WindowMode::Window) {
        glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
        glfwSetWindowMonitor(window, nullptr, 100, 100, 800, 600, GLFW_DONT_CARE);
    } else if (mode == WindowMode::Fullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
        glfwSetWindowAttrib(window, GLFW_AUTO_ICONIFY, GLFW_TRUE);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else if (mode == WindowMode::WindowedFullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE); // Убираем рамки
        glfwSetWindowAttrib(window, GLFW_AUTO_ICONIFY, GLFW_FALSE); // Убираем рамки
    }
}

OSDriverInterface::OSWindow::WindowMode OSDriverInterface::OSWindow::getWindowMode() {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::getWindowMode must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return WindowMode::Error;
    }

    GLFWwindow* window = static_cast<GLFWwindow*>(this->OSDataPtr);
    GLFWmonitor* monitor = glfwGetWindowMonitor(window);

    if (monitor != nullptr) {
        return WindowMode::Fullscreen;
    }

    int decorated = glfwGetWindowAttrib(window, GLFW_DECORATED);
    if (!decorated) {
        int wx, wy;
        glfwGetWindowSize(window, &wx, &wy);

        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primary);

        if (wx == mode->width && wy == mode->height) {
            return WindowMode::WindowedFullscreen;
        }
    }

    return WindowMode::Window;
}


void OSDriverInterface::OSWindow::setMouseMode(MouseMode mode) {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::setMouseMode must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return;
    }

    glfwSetInputMode((GLFWwindow*) this->OSDataPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL + mode);
}
HateEngine::OSDriverInterface::MouseMode OSDriverInterface::OSWindow::getMouseMode() {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::getMouseMode must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return MouseMode::Error;
    }
    return (MouseMode) (glfwGetInputMode((GLFWwindow*) this->OSDataPtr, GLFW_CURSOR) -
                        GLFW_CURSOR_NORMAL);
}

int OSDriverInterface::OSWindow::getSwapInterval() {
    return this->swapInterval;
}

//// Input ////
bool OSDriverInterface::OSWindow::isKeyPressed(Key key) {
    return this->cachedKeys[key - 1];
}

bool OSDriverInterface::OSWindow::isMouseButtonPressed(MouseButton btn) {
    return this->cachedMouseButtons[btn];
}

glm::vec2 OSDriverInterface::OSWindow::getCursorPosition() {
    return this->cachedCursorPos;
}


////////////////////////////    Key mapping    ///////////////////////////////////
static_assert(0, "Input keys was updated");
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
            return KeyInvalid; // или другой "invalid" ключ
    }
}

static MouseButton getEngineMouseButtonFromGLFWMouseButton(int btn) {
    return static_cast<MouseButton>(btn);
}

int getGLFWGamepadAxisFromEngineJoystickAxis(GamepadAxis axis) {
    static_assert(
            GamepadAxisRightTrigger == 13, "Enum GamepadAxis was changed, need to update this function"
    );
    static_assert(GLFW_GAMEPAD_AXIS_LEFT_X == 0, "GLFW API was changed, need to update this function");
    static_assert(
            GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER == 5, "GLFW API was changed, need to update this function"
    );

    int int_axis = static_cast<int>(axis);
    if (axis <= GamepadAxisRightYDown) {
        return int_axis >> 1;
    } else {
        return int_axis - GamepadAxisLeftX;
    }
}


#endif // __HATE_ENGINE_USE_GLFW
