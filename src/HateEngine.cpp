#include <thread>
#include <algorithm>
#include <glad/gl.h>
#include <HateEngine/HateEngine.hpp>
#include <HateEngine/Render/OpenGL15.hpp>
#include <HateEngine/Error.hpp>

#include "globalStaticParams.hpp"

#ifdef __linux__
#elif __APPLE__
#include <sched.h>
#elif _WIN32
#include <Windows.h>
#endif


bool glad_is_initialized = false;
UUID_Generator global_uuid_generator;

using namespace HateEngine;

Engine::Engine(std::string window_lbl, int width, int height) : Input(this) {
    glfwInit();
    // Create window
    this->window = glfwCreateWindow(width, height, window_lbl.c_str(), NULL, NULL);
    if (this->window == NULL) {
        Error::throwError("Failed to create GLFW window", false);
        glfwTerminate();
    }

    glfwSetWindowUserPointer(this->window, this);
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval( 0 );
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(this->window, [] (GLFWwindow *win, int w, int h) {
        Engine *th = static_cast<Engine*>(glfwGetWindowUserPointer(win));
        //th->frameBufferSizeChange(win, w, h);
        // FIXME: If will be few render APIs, this code should be in RenderAPI class
        glViewport(0, 0, w, h);
        if (th->level->camera == nullptr) {
            // WARNING
            return;
        }

        //! FIXME: Camera should't has ViewAspect field
        th->level->camera->setViewAspect(float(w) / float(h));
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

    // Load Glad
    int version = gladLoadGL(glfwGetProcAddress);
    if (not version) {
        Error::throwError("Failed to initialize GLAD", false);
        glfwTerminate();
    }
    glad_is_initialized = true;


    // Calculating the delay between FixedProcessLoop iterations
    #ifdef __linux__
        // TODO: Add Linux
    #elif __APPLE__
        sched_param sch_params;
        sch_params.sched_priority = 99;
        pthread_setschedparam(pthread_self(), SCHED_RR, &sch_params);
    #elif _WIN32
        // TODO: Add Win
    #endif

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
    bool isOneThread = false;

    std::thread *fixedProcessThread = nullptr;
    std::thread *physicsEngineProcessThread = nullptr;
    if (not isOneThread) {
        if (this->fixedProcessLoop != nullptr)
            fixedProcessThread = new std::thread(&Engine::threadFixedProcessLoop, this);

        physicsEngineProcessThread = new std::thread(
                &Engine::threadPhysicsEngineIterateLoop, this
        );
    }

    OpenGL15 ogl;

    glfwSwapBuffers(this->window);
    double oldTime = glfwGetTime();
    double fixed_process_loop_delta = 0.0;
    double physics_engine_iterate_loop_delta = 0.0;
    double delta = 0.0;
    double fixed_process_loop_delay = 1.0 / this->fixedLoopRefreshRate;
    double physics_engine_iterate_loop_delay = 1.0 / this->physicsEngineIterateLoopRefreshRate;
    while (not glfwWindowShouldClose(this->window)) {
        delta = glfwGetTime() - oldTime;
        oldTime = glfwGetTime();
        glfwPollEvents();

        fixed_process_loop_delta += delta;
        physics_engine_iterate_loop_delta += delta;

        //meshesMutex.lock();
        if (this->processLoop != nullptr)
            this->processLoop(this, delta);

        if (isOneThread) {
            if (this->fixedProcessLoop != nullptr and fixed_process_loop_delta >= fixed_process_loop_delay) {
                this->fixedProcessLoop(this, fixed_process_loop_delta);
                fixed_process_loop_delta = 0.0;
            }

            if (physics_engine_iterate_loop_delta >= physics_engine_iterate_loop_delay) {
                physEngine.IteratePhysics((float)physics_engine_iterate_loop_delta);
                physics_engine_iterate_loop_delta = 0.0;
            }
        }
        //meshesMutex.unlock();

        ogl.Draw3D(
            this->level->camera,
            &this->level->meshes,
            &this->level->particles,
            &this->level->lights
        );
        //meshesMutex.unlock();

        glfwSwapBuffers(this->window);
    }

    if (fixedProcessThread != nullptr and fixedProcessThread->joinable())
        fixedProcessThread->join();
    if (physicsEngineProcessThread != nullptr and physicsEngineProcessThread->joinable())
        physicsEngineProcessThread->join();
}




void Engine::threadFixedProcessLoop() {
#ifdef __APPLE__
    sched_param sch_params;
    sch_params.sched_priority = 99;
    pthread_t this_thread = pthread_self();
    pthread_setschedparam(pthread_self(), SCHED_RR, &sch_params);
#endif

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
        //meshesMutex.unlock();
        func_delta = glfwGetTime() - oldTime;
    }
}


void Engine::threadPhysicsEngineIterateLoop() {
#ifdef __APPLE__
    sched_param sch_params;
    sch_params.sched_priority = 99;
    pthread_t this_thread = pthread_self();
    pthread_setschedparam(pthread_self(), SCHED_RR, &sch_params);
#endif

    double oldTime = glfwGetTime();
    double delta = (double)this->physicsEngineIterateDelayMCS / 1000000;
    double func_delta = 0.0;

    while (not glfwWindowShouldClose(this->window)) {
        int64_t a = this->fixedProcessDelayMCS - int64_t(func_delta * 1000000);
        std::this_thread::sleep_for(
                std::chrono::microseconds(a)
        );

        delta = glfwGetTime() - oldTime;
        physEngine.IteratePhysics((float)delta);
        oldTime = glfwGetTime();
        func_delta = glfwGetTime() - oldTime;
    }
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

PhysEngine* const Engine::getPhysEngine() {
    return &physEngine;
}


void Engine::setLevelRef(Level* lvl) {
    this->level = lvl;
}





/*===========> THIS CODE SECTION CANNOT BE MODIFIED <===========*/

/*
 R.I.P Alexey Navalny
 #РоссияБудетСвободной
 #RussiaWillBeFree
*/

/*=====> END OF CODE SECTION PROHIBITED FROM MODIFICATION <=====*/