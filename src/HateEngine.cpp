//#include <bits/types/struct_sched_param.h>
//#include <pthread.h>
#include <thread>
#include <glad/gl.h>
#include <HateEngine/HateEngine.hpp>
#include <HateEngine/Render/OpenGL15.hpp>
#include <HateEngine/Error.hpp>

#include "GLFW/glfw3.h"
#include "glm/ext/vector_float2.hpp"
#include "globalStaticParams.hpp"

#ifdef __linux__
#include <pthread.h>
#define SET_THREAD_HIGH_PRIORITY\
    sched_param sch_params;\
    sch_params.sched_priority = 99;\
    pthread_setschedparam(pthread_self(), SCHED_RR, &sch_params);
#elif __APPLE__
#include <pthread.h>
#include <sched.h>
#define SET_THREAD_HIGH_PRIORITY\
    sched_param sch_params;\
    sch_params.sched_priority = 99;\
    pthread_setschedparam(pthread_self(), SCHED_RR, &sch_params);
#elif _WIN32
//#include <Windows.h>
// FIXME: I don't, it's correct or not
//#define SET_THREAD_HIGH_PRIORITY\
//    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);\
//    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
#define SET_THREAD_HIGH_PRIORITY ;
#endif



bool glad_is_initialized = false;

using namespace HateEngine;

Engine::Engine(std::string window_lbl, int width, int height) : Input(this) {
    //glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    glfwInit();
    // Create window
    GLFWmonitor *monitor = NULL;glfwGetPrimaryMonitor();
    this->window = glfwCreateWindow(width, height, window_lbl.c_str(), monitor, NULL);
    if (this->window == NULL) {
        Error::throwError("Failed to create GLFW window", false);
        glfwTerminate();
    }

    float xscale, yscale;
    //glfwGetWindowContentScale(this->window, &xscale, &yscale);
    
    // FIXME: Баги с масштабированием
    this->setResolution(width, height);

    glfwSetWindowUserPointer(this->window, this);
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval( 0 );
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(this->window, [] (GLFWwindow *win, int w, int h) {
        Engine *th = static_cast<Engine*>(glfwGetWindowUserPointer(win));
        //th->frameBufferSizeChange(win, w, h);
        // XXX: If will be few render APIs, this code should be in RenderAPI class
        glViewport(0, 0, w, h);
        if (th->level->camera == nullptr) {
            // WARNING
            return;
        }

        th->setResolution(w, h);
        std::cout << "Framebuffer size: " << w << "x" << h << std::endl;
    });

    glfwSetCursorPosCallback(window, [] (GLFWwindow *win, double x, double y) {
        Engine *th = (Engine*)(glfwGetWindowUserPointer(win));
        if (th->inputEventFunc != nullptr) {
            InputEventInfo info;
            info.type = InputEventType::InputEventMouseMove;
            info.position = {x, y};
            th->inputEventFunc(th, info);
        }
    });

    glfwSetKeyCallback(window, [] (GLFWwindow *win, int key, int scancode, int action, int mods) {
        Engine *th = (Engine*)(glfwGetWindowUserPointer(win));
        if (th->inputEventFunc != nullptr) {
            InputEventInfo info;
            info.type = InputEventType::InputEventKey;
            info.key = key;
            info.scancode = scancode;
            info.action = action;
            info.mods = mods;
            th->inputEventFunc(th, info);
        }
    });
    
    glfwSetMouseButtonCallback(window, [] (GLFWwindow *win,  int button, int action, int mods) {
        Engine *th = (Engine*)(glfwGetWindowUserPointer(win));
        if (th->inputEventFunc != nullptr) {
            InputEventInfo info;
            info.type = InputEventType::InputEventMouseButton;
            info.key = button;
            info.scancode = button;
            info.action = action;
            info.mods = mods;
            th->inputEventFunc(th, info);
        }
    });
    
    glfwSetScrollCallback(window, [] (GLFWwindow *win, double xoffset, double yoffset) {
        Engine *th = (Engine*)(glfwGetWindowUserPointer(win));
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
        Error::throwError("Failed to initialize GLAD", false);
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
    int64_t d_fp = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() / 10 - fixDelay;
    this->fixedProcessDelayMCS = fixDelay - d_fp;

    if (physicsEngineIterateLoopRefreshRate == fixedLoopRefreshRate) {
        this->physicsEngineIterateDelayMCS = this->fixedProcessDelayMCS;
        return;
    }

    t1 = std::chrono::high_resolution_clock::now();
    for (int o = 0; o < 10; ++o)
        std::this_thread::sleep_for(std::chrono::microseconds(physDelay));
    t2 = std::chrono::high_resolution_clock::now();
    int64_t d_pp = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() / 10 - physDelay;
    this->physicsEngineIterateDelayMCS = physDelay - d_pp;
}


void Engine::Run() {
    std::thread *fixedProcessThread = nullptr;
    std::thread *physicsEngineProcessThread = nullptr;
    if (not this->isOneThread) {
        if (this->fixedProcessLoop != nullptr)
            fixedProcessThread = new std::thread(&Engine::threadFixedProcessLoop, this);

        physicsEngineProcessThread = new std::thread(
                &Engine::threadPhysicsEngineIterateLoop, this
        );
    }

    OpenGL15 ogl(this);
    

    glfwSwapBuffers(this->window);
    double oldTime = glfwGetTime();
    double fixed_process_loop_delta = -glfwGetTime();
    double physics_engine_iterate_loop_delta = -glfwGetTime();
    double delta = 0.0;
    double fixed_process_loop_delay = 1.0 / this->fixedLoopRefreshRate;
    double physics_engine_iterate_loop_delay = 1.0 / this->physicsEngineIterateLoopRefreshRate;
    while (not glfwWindowShouldClose(this->window)) {
        delta = glfwGetTime() - oldTime;
        oldTime = glfwGetTime();
        glfwPollEvents();

        //meshesMutex.lock();
        if (this->processLoop != nullptr)
            this->processLoop(this, delta);
            
        if (this->level->processLoop != nullptr)
            this->level->processLoop(this, delta);

        if (this->isOneThread) {
            fixed_process_loop_delta += delta;
            physics_engine_iterate_loop_delta += delta;

            if (this->fixedProcessLoop != nullptr and fixed_process_loop_delta >= fixed_process_loop_delay) {
                this->fixedProcessLoop(this, fixed_process_loop_delta);
                if (this->level->fixedProcessLoop != nullptr)
                    this->level->fixedProcessLoop(this, delta);
                fixed_process_loop_delta = 0.0;
            }

            if (physics_engine_iterate_loop_delta >= physics_engine_iterate_loop_delay) {
                //uint32_t i = 0;
                //std::cout << "Physics Engine Iteration: " << physics_engine_iterate_loop_delta << std::endl;
                while (physics_engine_iterate_loop_delta-0.0001 > 0.0f) {
                    float d = physics_engine_iterate_loop_delay;
                    if (physics_engine_iterate_loop_delta < d)
                        d = physics_engine_iterate_loop_delta;
                    this->level->getPhysEngine()->IteratePhysics(d); 
                    physics_engine_iterate_loop_delta -= d;
                    //std::cout << "Physics Engine Iteration [" << i++ << "]: " << d << std::endl;
                }
                physics_engine_iterate_loop_delta = 0.0;
            }
        }

        ogl.Render();

        glfwSwapBuffers(this->window);
    }

    if (fixedProcessThread != nullptr and fixedProcessThread->joinable())
        fixedProcessThread->join();
    if (physicsEngineProcessThread != nullptr and physicsEngineProcessThread->joinable())
        physicsEngineProcessThread->join();
}

void Engine::Exit() {
    glfwSetWindowShouldClose(this->window, true);
}



void Engine::setResolution(int width, int height) {
    Error::throwWarning("Engine::setResolution not implemented");
    
    // FIXME: Implement setResolution
    this->resolution = glm::ivec2(width, height);
    this->aspectRatio = (float)width / (float)height;
}

void Engine::setOneThreadMode(bool mode) {
    this->isOneThread = mode;
}

void Engine::setMouseCapture(bool capture) {
    if (capture)
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

glm::ivec2 Engine::getResolution() {
    return this->resolution;
}

float Engine::getAspectRatio() {
    return this->aspectRatio;
}




void Engine::threadFixedProcessLoop() {
    SET_THREAD_HIGH_PRIORITY

    double oldTime = glfwGetTime();
    double delta = (float)this->fixedProcessDelayMCS / 1000000;
    double func_delta = 0.0;

    while (not glfwWindowShouldClose(this->window)) {
        int64_t a = this->fixedProcessDelayMCS - int64_t(func_delta * 1000000);
        std::this_thread::sleep_for(
                std::chrono::microseconds(a)
        );
        //std::cout << d / 10<< "\n";
        delta = glfwGetTime() - oldTime;
        oldTime = glfwGetTime();
        //physicsWorld->update((float)delta);
        //std::cout << rbody->getTransform().getGlobalPosition().y << "\n";

        //meshesMutex.lock();
        fixedProcessLoop(this, delta);
        if (this->level->fixedProcessLoop != nullptr)
            this->level->fixedProcessLoop(this, delta);
        //meshesMutex.unlock();
        func_delta = glfwGetTime() - oldTime;
    }
}


void Engine::threadPhysicsEngineIterateLoop() {
    SET_THREAD_HIGH_PRIORITY

    double oldTime = glfwGetTime();
    double delta = (double)this->physicsEngineIterateDelayMCS / 1000000;
    double func_delta = 0.0;

    while (not glfwWindowShouldClose(this->window)) {
        int64_t a = this->fixedProcessDelayMCS - int64_t(func_delta * 1000000);
        std::this_thread::sleep_for(
                std::chrono::microseconds(a)
        );

        delta = glfwGetTime() - oldTime;
        level->getPhysEngine()->IteratePhysics((float)delta);
        oldTime = glfwGetTime();
        func_delta = glfwGetTime() - oldTime;
    }
}


void Engine::changeWindowTitle(std::string title) {
    glfwSetWindowTitle(this->window, title.c_str());
}

void Engine::setProcessLoop(void (*func)(Engine*, double)) {
    this->processLoop = func;
}
void Engine::setFixedProcessLoop(void (*func)(Engine*, double)) {
    this->fixedProcessLoop = func;
}
void Engine::setInputEvent(void (*func)(Engine *, InputEventInfo)) {
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