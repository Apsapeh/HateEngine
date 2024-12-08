// #include <bits/types/struct_sched_param.h>
// #include <pthread.h>
#include <glad/gl.h>

#include <HateEngine/HateEngine.hpp>
#include <HateEngine/Log.hpp>
#include <HateEngine/Render/OpenGL15.hpp>
#include <HateEngine/AudioServer.hpp>
#include <thread>

#include "GLFW/glfw3.h"
#include "HateEngine/AudioServer.hpp"
#include "HateEngine/Input.hpp"
#include "glm/ext/vector_float2.hpp"
#include "globalStaticParams.hpp"

#ifdef __linux__
#include <pthread.h>
#include <sched.h>
#define SET_THREAD_HIGH_PRIORITY                                                                   \
    sched_param sch_params;                                                                        \
    sch_params.sched_priority = 99;                                                                \
    pthread_setschedparam(pthread_self(), SCHED_RR, &sch_params);
#elif __APPLE__
#include <pthread.h>
#include <sched.h>
#define SET_THREAD_HIGH_PRIORITY                                                                   \
    sched_param sch_params;                                                                        \
    sch_params.sched_priority = 99;                                                                \
    pthread_setschedparam(pthread_self(), SCHED_RR, &sch_params);
#elif _WIN32
#include <windows.h>
#define SET_THREAD_HIGH_PRIORITY                                                                   \
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);                                    \
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
// #define SET_THREAD_HIGH_PRIORITY ;
#endif

bool glad_is_initialized = false;

using namespace HateEngine;

Engine::Engine(std::string window_lbl, int width, int height) : Input(this) {
    // glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    this->windowTitle = window_lbl;
    glfwInit();
    // Create window
    GLFWmonitor* monitor = NULL;
    glfwGetPrimaryMonitor();
    this->window = glfwCreateWindow(width, height, window_lbl.c_str(), monitor, NULL);
    if (this->window == NULL) {
        HATE_FATAL("Failed to create GLFW window");
        glfwTerminate();
    }

    // float xscale, yscale;

    this->setResolution(width, height);
    // this->setResolution(, )
    this->resolution = {(float) width, (float) height};


    glfwSetWindowUserPointer(this->window, this);
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(this->isVSync);

    // glfwWindowHint(GLFW_DECORATED, false);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* win, int w, int h) {
        Engine* th = static_cast<Engine*>(glfwGetWindowUserPointer(win));

        th->setResolution(w, h);
        // std::cout << "Framebuffer size: " << w << "x" << h << std::endl;
        HATE_DEBUG_F("Framebuffer size: %dx%d", w, h);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
        Engine* th = (Engine*) (glfwGetWindowUserPointer(win));
        if (th->inputEventFunc != nullptr) {
            InputEventInfo info;
            info.type = InputEventType::InputEventMouseMove;
            info.position = {x, y};
            th->inputEventFunc(th, info);
        }
    });

    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        Engine* th = (Engine*) (glfwGetWindowUserPointer(win));
        if (th->inputEventFunc != nullptr) {
            InputEventInfo info;
            info.type = InputEventType::InputEventKey;
            info.raw_key = key;
            info.key = static_cast<Key>(key);
            info.scancode = scancode;
            info.isPressed = action == GLFW_PRESS;
            info.mods = mods;
            th->inputEventFunc(th, info);
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods) {
        Engine* th = (Engine*) (glfwGetWindowUserPointer(win));
        if (th->inputEventFunc != nullptr) {
            InputEventInfo info;
            info.type = InputEventType::InputEventMouseButton;
            info.raw_key = button;
            info.button = static_cast<MouseButton>(button);
            info.scancode = button;
            info.isPressed = action == GLFW_PRESS;
            info.position = th->Input.getCursorPosition();
            info.mods = mods;
            th->inputEventFunc(th, info);
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow* win, double xoffset, double yoffset) {
        Engine* th = (Engine*) (glfwGetWindowUserPointer(win));
        if (th->inputEventFunc != nullptr) {
            InputEventInfo info;
            info.type = InputEventType::InputEventMouseScroll;
            info.position = {xoffset, yoffset};
            th->inputEventFunc(th, info);
        }
    });

    // Load Glad
    int version = gladLoadGL(glfwGetProcAddress);
    if (not version) {
        HATE_FATAL("Failed to initialize GLAD");
        glfwTerminate();
    }
    glad_is_initialized = true;

    // Calculating the delay between FixedProcessLoop iterations
    SET_THREAD_HIGH_PRIORITY

    int64_t fixDelay = 1000000 / fixedLoopRefreshRate;
    int64_t physDelay = 1000000 / physicsEngineIterateLoopRefreshRate;

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    for (int o = 0; o < 10; ++o)
        std::this_thread::sleep_for(std::chrono::microseconds(fixDelay));
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    int64_t d_fp =
            std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 10 - fixDelay;
    this->fixedProcessDelayMCS = fixDelay - d_fp;

    if (physicsEngineIterateLoopRefreshRate == fixedLoopRefreshRate) {
        this->physicsEngineIterateDelayMCS = this->fixedProcessDelayMCS;
        return;
    }

    t1 = std::chrono::high_resolution_clock::now();
    for (int o = 0; o < 10; ++o)
        std::this_thread::sleep_for(std::chrono::microseconds(physDelay));
    t2 = std::chrono::high_resolution_clock::now();
    int64_t d_pp =
            std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 10 - physDelay;
    this->physicsEngineIterateDelayMCS = physDelay - d_pp;
    AudioServer::Reinit();
}

void Engine::Run() {
    SET_THREAD_HIGH_PRIORITY
    this->isRunned = true;
    std::thread* fixedProcessThread = nullptr;
    std::thread* physicsEngineProcessThread = nullptr;
    if (not this->isOneThread) {
        if (this->fixedProcessLoop != nullptr)
            fixedProcessThread = new std::thread(&Engine::threadFixedProcessLoop, this);

        physicsEngineProcessThread = new std::thread(&Engine::threadPhysicsEngineIterateLoop, this);
    }

    OpenGL15 ogl(this);

    glfwSwapBuffers(this->window);
    double oldTime = glfwGetTime();
    double fixed_process_loop_delta = 0;
    double physics_engine_iterate_loop_delta = 0;
    double delta = 0.0;
    double fixed_process_loop_delay = 1.0 / this->fixedLoopRefreshRate;
    double physics_engine_iterate_loop_delay = 1.0 / this->physicsEngineIterateLoopRefreshRate;
    while (not glfwWindowShouldClose(this->window)) {
        delta = glfwGetTime() - oldTime;
        oldTime = glfwGetTime();
        glfwPollEvents();

        /// Thread safety GLFW calls
        if (this->needChangeMouseCaptureMode) {
            glfwSetInputMode(
                    this->window, GLFW_CURSOR,
                    this->isMouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
            );
            this->needChangeMouseCaptureMode = false;
        }
        if (this->needChangeWindowTitle) {
            glfwSetWindowTitle(this->window, this->windowTitle.c_str());
            this->needChangeWindowTitle = false;
        }
        if (this->needChangeFullScreenMode) {
            if (this->isFullScreen) {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                glfwWindowHint(GLFW_RED_BITS, mode->redBits);
                glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
                glfwSetWindowMonitor(
                        this->window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate
                );
            } else
                glfwSetWindowMonitor(this->window, nullptr, 0, 0, 800, 600, GLFW_DONT_CARE);

            this->needChangeFullScreenMode = false;
        }
        ///

        // meshesMutex.lock();
        if (this->processLoop != nullptr)
            this->processLoop(this, delta);

        if (this->level != nullptr and this->level->processLoop != nullptr)
            this->level->processLoop(this, delta);

        if (this->isOneThread) {
            fixed_process_loop_delta += delta;
            physics_engine_iterate_loop_delta += delta;


            if (this->level != nullptr)
                this->level->Update(delta);

            if (this->fixedProcessLoop != nullptr and
                fixed_process_loop_delta >= fixed_process_loop_delay) {
                this->fixedProcessLoop(this, fixed_process_loop_delta);
                if (this->level != nullptr and this->level->fixedProcessLoop != nullptr)
                    this->level->fixedProcessLoop(this, delta);
                fixed_process_loop_delta = 0.0;
            }

            if (physics_engine_iterate_loop_delta >= physics_engine_iterate_loop_delay) {
                // uint32_t i = 0;
                /*std::cout << "Physics Engine Iteration: " <<
                physics_engine_iterate_loop_delta << std::endl;*/
                if (this->level != nullptr) {
                    while (physics_engine_iterate_loop_delta - 0.0001 > 0.0f) {
                        float d = physics_engine_iterate_loop_delay;
                        if (physics_engine_iterate_loop_delta < d)
                            d = physics_engine_iterate_loop_delta;
                        this->level->getPhysEngine()->IteratePhysics(d);
                        physics_engine_iterate_loop_delta -= d;
                        // std::cout << "Physics Engine Iteration [" << i++ << "]: "
                        // << d << std::endl;
                    }
                }
                physics_engine_iterate_loop_delta = 0.0;
            }
        }

        if (this->level != nullptr) {
            for (auto& obj: level->animationPlayers) {
                obj->Update(delta);
            }

            if (this->level->camera != nullptr) {
                AudioServer::setListener3DPosition(this->level->camera->getGlobalPosition());
                AudioServer::setListener3DDirection(this->level->camera->getGlobalDirection());
            }

            ogl.Render();
        }

        AudioServer::Update3D();


        glfwSwapBuffers(this->window);
    }

    if (fixedProcessThread != nullptr and fixedProcessThread->joinable())
        fixedProcessThread->join();
    if (physicsEngineProcessThread != nullptr and physicsEngineProcessThread->joinable())
        physicsEngineProcessThread->join();
    AudioServer::Deinit();
}

void Engine::Exit() {
    glfwSetWindowShouldClose(this->window, true);
}

void Engine::setResolution(int width, int height) {
    HATE_WARNING("Engine::setResolution not implemented");

    // FIXME: Implement setResolution
    float xscale, yscale;
    glfwGetWindowContentScale(this->window, &xscale, &yscale);
    this->displayScale = glm::vec2(xscale, yscale);
    this->resolution = glm::ivec2((float) width / xscale, (float) height / yscale);
    this->aspectRatio = (float) width / (float) height;
}

void Engine::setOneThreadMode(bool mode) {
    if (this->isRunned)
        HATE_ERROR("Engine::setOneThreadMode should be called before Engine::Run");
    this->isOneThread = mode;
}

void Engine::setVSync(bool vsync) {
    glfwSwapInterval(vsync);
    this->isVSync = vsync;
}

void Engine::setMouseCapture(bool capture) {
    /*if (capture)
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);*/
    this->isMouseCaptured = capture;
    this->needChangeMouseCaptureMode = true;
}

void Engine::setFullScreen(bool fullScreen) {
    this->isFullScreen = fullScreen;
    this->needChangeFullScreenMode = true;
}

glm::ivec2 Engine::getResolution() {
    return this->resolution;
}

glm::vec2 Engine::getDisplayScale() {
    return this->displayScale;
}

float Engine::getAspectRatio() {
    return this->aspectRatio;
}

bool Engine::getOneThreadMode() {
    return this->isOneThread;
}

bool Engine::getVSync() {
    return this->isVSync;
}

bool Engine::getMouseCapture() {
    return this->isMouseCaptured;
}

bool Engine::getFullScreen() {
    return this->isFullScreen;
}

void Engine::threadFixedProcessLoop() {
    SET_THREAD_HIGH_PRIORITY

    double oldTime = glfwGetTime();
    double delta = (float) this->fixedProcessDelayMCS / 1000000;
    double func_delta = 0.0;

    while (not glfwWindowShouldClose(this->window)) {
        int64_t a = this->fixedProcessDelayMCS - int64_t(func_delta * 1000000);
        std::this_thread::sleep_for(std::chrono::microseconds(a));
        // std::cout << d / 10<< "\n";
        delta = glfwGetTime() - oldTime;
        oldTime = glfwGetTime();
        if (this->level != nullptr)
            this->level->Update(delta);

        // meshesMutex.lock();
        fixedProcessLoop(this, delta);
        if (this->level != nullptr and this->level->fixedProcessLoop != nullptr)
            this->level->fixedProcessLoop(this, delta);
        // meshesMutex.unlock();
        func_delta = glfwGetTime() - oldTime;
    }
}

void Engine::threadPhysicsEngineIterateLoop() {
    SET_THREAD_HIGH_PRIORITY

    double oldTime = glfwGetTime();
    double delta = (double) this->physicsEngineIterateDelayMCS / 1000000;
    double func_delta = 0.0;

    while (not glfwWindowShouldClose(this->window)) {
        int64_t a = this->fixedProcessDelayMCS - int64_t(func_delta * 1000000);
        std::this_thread::sleep_for(std::chrono::microseconds(a));

        delta = glfwGetTime() - oldTime;
        if (this->level != nullptr)
            this->level->getPhysEngine()->IteratePhysics((float) delta);
        oldTime = glfwGetTime();
        func_delta = glfwGetTime() - oldTime;
    }
}

void Engine::changeWindowTitle(std::string title) {
    // glfwSetWindowTitle(this->window, title.c_str());
    this->windowTitle = title;
    glfwSetWindowTitle(this->window, this->windowTitle.c_str());
}

void Engine::setProcessLoop(void (*func)(Engine*, double)) {
    this->processLoop = func;
}
void Engine::setFixedProcessLoop(void (*func)(Engine*, double)) {
    this->fixedProcessLoop = func;
}
void Engine::setInputEvent(void (*func)(Engine*, InputEventInfo)) {
    this->inputEventFunc = func;
}

void Engine::setLevelRef(Level* lvl) {
    this->level = lvl;
}

Level* Engine::getLevel() {
    return this->level;
}

/*===========> THIS CODE SECTION CANNOT BE MODIFIED <===========*/

/*
 R.I.P Alexey Navalny
 #РоссияБудетСвободной
 #RussiaWillBeFree
*/

/*=====> END OF CODE SECTION PROHIBITED FROM MODIFICATION <=====*/
