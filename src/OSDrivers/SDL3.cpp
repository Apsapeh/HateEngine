#include "SDL3/SDL_joystick.h"
#include "SDL3/SDL_stdinc.h"
#ifdef __HATE_ENGINE_USE_SDL3
#include "HateEngine/InputEvent.hpp"
#include "HateEngine/InputKeys.hpp"
#include "SDL3/SDL_mouse.h"

#include <cstdlib>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "SDL3/SDL_gamepad.h"
#include "HateEngine/HateEngine.hpp"


#include <SDL3/SDL.h>
#include <HateEngine/OSDriverInterface.hpp>
#include <HateEngine/Log.hpp>
#include <memory>
#include "HateEngine/Types/Result.hpp"
#include <thread>

using namespace HateEngine;

static Key getEngineKeyFromSDL3Key(int key);
static SDL_Scancode getSDL3KeyFromEngineKey(Key key);
static MouseButton getEngineMouseButtonFromGLFWMouseButton(int btn);
static int getGLFWGamepadAxisFromEngineJoystickAxis(GamepadAxis axis);

////////////////////    OS Driver     //////////////////////////////
static bool sdl3_inited = false;

OSDriverInterface::OSDriverInterface() {
    if (not sdl3_inited) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) == false) {
            const auto msg = SDL_GetError();
            HATE_FATAL_F("SDL3 initialization failed: %s", msg);
        }
        sdl3_inited = true;
    }
}

OSDriverInterface::~OSDriverInterface() {
    SDL_Quit();
}

ProcAddr OSDriverInterface::getProcAddress(const char* proc) {
    return SDL_GL_GetProcAddress(proc);
}

Result<std::shared_ptr<OSDriverInterface::OSWindow>, bool> OSDriverInterface::createWindow(
        int width, int height, const std::string& title, OSDisplay* display
) {
    std::shared_ptr<OSWindow> window = std::make_shared<OSWindow>();

    window->OSDataPtr =
            SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window->OSDataPtr) {
        return Err<std::shared_ptr<OSWindow>, bool>(false);
    }

    window->OSContextPtr = SDL_GL_CreateContext((SDL_Window*) window->OSDataPtr);
    if (!window->OSContextPtr) {
        SDL_DestroyWindow((SDL_Window*) window->OSDataPtr);
        return Err<std::shared_ptr<OSWindow>, bool>(false);
    }

    auto id = SDL_GetWindowID((SDL_Window*) window->OSDataPtr);
    this->windows[id] = window;
    return Result<std::shared_ptr<OSWindow>, bool>::Ok(window);
}

void OSDriverInterface::makeWindowContextCurrent(OSDriverInterface::OSWindow& window) {
    SDL_GL_MakeCurrent((SDL_Window*) window.OSDataPtr, (SDL_GLContext) window.OSContextPtr);
}

void OSDriverInterface::poll() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        HATE_INFO_F("Event type: %d", event.type)
        if (event.type == SDL_EVENT_QUIT) {
            this->requireClose();
            return;
        } else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            auto it = this->windows.find(event.window.windowID);
            if (it != this->windows.end()) {
                it->second->requireClose();
                SDL_DestroyWindow((SDL_Window*) it->second->OSDataPtr);
            }
        } else if (event.type == SDL_EVENT_WINDOW_DESTROYED) {
            this->windows.erase(event.window.windowID);
        } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            auto it = this->windows.find(event.window.windowID);
            if (it != this->windows.end()) {
                this->onFramebufferSizeChanged.emit(it->second, event.window.data1, event.window.data2);
            }
        } else if ((event.type == SDL_EVENT_KEY_DOWN or event.type == SDL_EVENT_KEY_UP) and
                   not event.key.repeat) {
            InputEventInfo info;
            info.type = InputEventKey;
            info.isPressed = event.type == SDL_EVENT_KEY_DOWN;
            info.key = static_cast<Key>(event.key.scancode);

            auto it = this->windows.find(event.key.windowID);
            if (it != this->windows.end()) {
                // onKeyChanged(*it, info);
                this->onKeyChanged.emit(it->second, info);
            }
        } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
            InputEventInfo info;
            info.type = InputEventMouseMove;
            info.isPressed = false;
            info.offset.x = event.motion.xrel;
            info.offset.y = event.motion.yrel;
            info.position.x = event.motion.x;
            info.position.y = event.motion.y;

            auto it = this->windows.find(event.motion.windowID);
            if (it != this->windows.end()) {
                // onMouseMoved(*it, info);
                this->onCursorMoved.emit(it->second, info);
            }
        } else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
            InputEventInfo info;
            info.type = InputEventMouseScroll;
            info.isPressed = false;
            info.offset.x = event.wheel.x;
            info.offset.y = event.wheel.y;

            auto it = this->windows.find(event.wheel.windowID);
            if (it != this->windows.end()) {
                // onMouseWheel(*it, info);
                this->onMouseWheelScrolled.emit(it->second, info);
            }
        } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                   event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            InputEventInfo info;
            info.type = InputEventMouseButton;
            info.isPressed = event.button.down;
            info.position.x = event.button.x;
            info.position.y = event.button.y;

            auto it = this->windows.find(event.button.windowID);
            if (it != this->windows.end()) {
                // onMouseButton(*it, info);
                this->onMouseButtonChanged.emit(it->second, info);
            }
        }
    }
}

double OSDriverInterface::getTime() {
    return (double) SDL_GetTicksNS() / 1000000000.0;
}

bool OSDriverInterface::isShouldBeClosed() {
    for (auto& window: this->windows) {
        if (not window.second->isShouldBeClosed()) {
            return false;
        }
    }
    return true;
}

void OSDriverInterface::requireClose() {
    for (auto& window: this->windows) {
        window.second->requireClose();
    }
}

std::vector<JoystickHandle> OSDriverInterface::getAvailableJoysticks() {
    return {};
}
std::vector<JoystickHandle> OSDriverInterface::getAvailableGamepads() {
    int count;
    auto ids = SDL_GetGamepads(&count);
    std::vector<JoystickHandle> handles(ids, ids+count);
    SDL_free(ids);
    return handles;
}

bool OSDriverInterface::isJoystickAvailable(JoystickHandle handle) {
    return false;
}

bool OSDriverInterface::isJoystickGamepad(JoystickHandle handle) {
    return SDL_IsGamepad(handle);
}

Result<std::vector<bool>, OSDriverInterface::JoysticErr> OSDriverInterface::getJoystickButtonsRaw(
        JoystickHandle handle
) {
    return Ok<std::vector<bool>, JoysticErr>({});
}

Result<std::vector<float>, OSDriverInterface::JoysticErr> OSDriverInterface::getJoystickAxisRaw(
        JoystickHandle handle
) {
    return Ok<std::vector<float>, JoysticErr>({});
}

Result<bool, OSDriverInterface::JoysticErr> OSDriverInterface::isJoystickButtonPressed(
        JoystickHandle handle, int button
) {
    return Ok<bool, JoysticErr>(false);
}

Result<float, OSDriverInterface::JoysticErr> OSDriverInterface::getJoystickAxis(
        JoystickHandle handle, int axis
) {
    return Ok<float, JoysticErr>(0);
}

Result<bool, OSDriverInterface::JoysticErr> OSDriverInterface::isGamepadButtonPressed(
        JoystickHandle handle, GamepadButtons button
) {
    return Ok<bool, JoysticErr>(false);
}

Result<float, OSDriverInterface::JoysticErr> OSDriverInterface::getGamepadAxis(
        JoystickHandle handle, GamepadAxis axis
) {
    return Ok<float, JoysticErr>(0.0f);
}

Option<std::string> OSDriverInterface::getJoystickName(JoystickHandle handle) {
    return None<std::string>();
}

Option<std::string> OSDriverInterface::getGamepadName(JoystickHandle handle) {
    return None<std::string>();
}


///////////////////    OS Window    //////////////////////

OSDriverInterface::OSWindow::OSWindow() {
}

OSDriverInterface::OSWindow::~OSWindow() {
    SDL_GL_DestroyContext((SDL_GLContext) this->OSContextPtr);
    SDL_DestroyWindow((SDL_Window*) this->OSDataPtr);
}

void OSDriverInterface::OSWindow::init() {
    setSwapInterval(this->swapInterval);
}

void OSDriverInterface::OSWindow::swapBuffers() {
    SDL_GL_SwapWindow((SDL_Window*) this->OSDataPtr);
}

bool OSDriverInterface::OSWindow::isShouldBeClosed() {
    return this->shouldBeClosed;
}

glm::vec2 OSDriverInterface::OSWindow::getScale() {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::getScale must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return {1, 1};
    }

    float scale = SDL_GetWindowDisplayScale((SDL_Window*) OSDataPtr);
    return {scale, scale};
}

void OSDriverInterface::OSWindow::requireClose() {
    this->shouldBeClosed = true;
}

void OSDriverInterface::OSWindow::setTitle(const std::string& title) {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::setTitle must be called only from main thread or via "
                "Engine::callDeferred/callDeferredAsync"
        )
        return;
    }

    SDL_SetWindowTitle((SDL_Window*) this->OSDataPtr, title.c_str());
}

void OSDriverInterface::OSWindow::setSwapInterval(int interval) {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::setSwapInterval must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return;
    }

    // FIXME: change context
    // auto current_context = SDL_GL_GetCurrentContext();
    // SDL_GL_MakeCurrent((SDL_Window*) this->OSDataPtr, (SDL_GLContext) this->OSContextPtr);
    SDL_GL_SetSwapInterval(interval);

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

    SDL_SetWindowSize((SDL_Window*) this->OSDataPtr, w, h);
}


void OSDriverInterface::OSWindow::setWindowMode(WindowMode mode, OSDisplay* display) {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::setWindowMode must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return;
    }

    if (mode == WindowMode::Window) {
        SDL_SetWindowFullscreen((SDL_Window*) this->OSDataPtr, false);
    } else if (mode == WindowMode::WindowedFullscreen) {
        SDL_SetWindowFullscreen((SDL_Window*) this->OSDataPtr, true);
    } else if (mode == WindowMode::Fullscreen) {
        SDL_SetWindowFullscreen((SDL_Window*) this->OSDataPtr, true);
        auto display = SDL_GetDisplayForWindow((SDL_Window*) this->OSDataPtr);
        int count;
        SDL_DisplayMode** mode = SDL_GetFullscreenDisplayModes(display, &count);

        // TODO: Add mode selection
        if (count > 0) {
            SDL_SetWindowFullscreenMode((SDL_Window*) this->OSDataPtr, mode[0]);
        }
        // for (int i = 0; i < count; i++) {
        //     HATE_INFO_F("Mode %dx%d", mode[i]->w, mode[i]->h)
        //     //SDL_DisplayMode* currentMode = mode[i];
        //     //  TODO
        // }
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

    const auto flags = SDL_GetWindowFlags((SDL_Window*) this->OSDataPtr);
    if (flags & SDL_WINDOW_FULLSCREEN) {
        const auto* mode = SDL_GetWindowFullscreenMode((SDL_Window*) this->OSDataPtr);
        if (mode == nullptr)
            return WindowMode::WindowedFullscreen;
        else
            return WindowMode::Fullscreen;
    } else {
        return WindowMode::Window;
    }
}


void OSDriverInterface::OSWindow::setMouseMode(MouseMode mode) {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::setMouseMode must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return;
    }

    SDL_Window* window = (SDL_Window*) this->OSDataPtr;
    if (mode == MouseMode::Normal) {
        SDL_ShowCursor();
        SDL_SetWindowMouseGrab(window, false);
        SDL_SetWindowRelativeMouseMode(window, false);
    } else if (mode == MouseMode::Hidden) {
        SDL_HideCursor();
        SDL_SetWindowMouseGrab(window, false);
        SDL_SetWindowRelativeMouseMode(window, false);
    } else if (mode == MouseMode::Disabled) {
        SDL_HideCursor();
        SDL_SetWindowRelativeMouseMode(window, true);
    } else if (mode == MouseMode::Captured) {
        SDL_ShowCursor();
        SDL_SetWindowMouseGrab(window, true);
        SDL_SetWindowRelativeMouseMode(window, false);
    }
}
HateEngine::OSDriverInterface::MouseMode OSDriverInterface::OSWindow::getMouseMode() {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::getMouseMode must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return MouseMode::Error;
    }

    SDL_Window* window = (SDL_Window*) this->OSDataPtr;
    auto is_visible = SDL_CursorVisible();
    auto is_grabbed = SDL_GetWindowMouseGrab(window);
    auto is_relative = SDL_GetWindowRelativeMouseMode(window);

    if (is_visible and not is_grabbed and not is_relative)
        return MouseMode::Normal;
    else if (not is_visible and not is_grabbed and not is_relative)
        return MouseMode::Hidden;
    else if (not is_visible and is_relative)
        return MouseMode::Disabled;
    else if (is_visible and is_grabbed and not is_relative)
        return MouseMode::Captured;
    else
        return MouseMode::Unknown;
}

int OSDriverInterface::OSWindow::getSwapInterval() {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::getSwapInterval must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return 0;
    }

    // FIXME: change context
    // auto current_context = SDL_GL_GetCurrentContext();
    // SDL_GL_MakeCurrent((SDL_Window*) this->OSDataPtr, (SDL_GLContext) this->OSContextPtr);
    int interval;
    SDL_GL_GetSwapInterval(&interval);

    // SDL_GL_MakeCurrent((SDL_Window*) this->OSDataPtr, current_context);
    return interval;
}

//// Input ////
bool OSDriverInterface::OSWindow::isPhysicalKeyPressed(Key key) {
    const auto state = SDL_GetKeyboardState(nullptr);
    return state[key];
}

bool OSDriverInterface::OSWindow::isMouseButtonPressed(MouseButton btn) {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::isMouseButtonPressed must be called only from main thread "
                "or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return false;
    }

    const auto state = SDL_GetMouseState(nullptr, nullptr);
    if (btn == MouseButtonLeft)
        return state & SDL_BUTTON_LMASK;
    else if (btn == MouseButtonRight)
        return state & SDL_BUTTON_RMASK;
    else if (btn == MouseButtonMiddle)
        return state & SDL_BUTTON_MMASK;
    else if (btn == MouseButton4)
        return state & SDL_BUTTON_X1MASK;
    else if (btn == MouseButton5)
        return state & SDL_BUTTON_X2MASK;
    return false;
}

glm::vec2 OSDriverInterface::OSWindow::getCursorPosition() {
    if (std::this_thread::get_id() != Engine::getMainThreadId()) {
        HATE_ERROR(
                "OSDriverInterface::OSWindow::getCursorPosition must be called only from main thread or "
                "via Engine::callDeferred/callDeferredAsync"
        )
        return glm::vec2(0, 0);
    }

    glm::vec2 pos;
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}


////////////////////////////    Key mapping    ///////////////////////////////////
static MouseButton getEngineMouseButtonFromGLFWMouseButton(int btn) {
    return MouseButton::MouseButtonLeft;
}

int getGLFWGamepadAxisFromEngineJoystickAxis(GamepadAxis axis) {
    return 0;
}


#endif // __HATE_ENGINE_USE_SDL3
