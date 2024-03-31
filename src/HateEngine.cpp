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

Engine::Engine(std::string window_lbl, int width, int height) : Input(this){
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
        glViewport(0, 0, w, h);
        if (th->level->camera == nullptr) {
            // WARNING
            return;
        }

        //! FIXME: Camera should't has ViewAspect field
        th->level->camera->setViewAspect(float(w) / float(h));
    });

    glfwSetCursorPosCallback(window, [] (GLFWwindow *win, double x, double y) {
        Engine *th = static_cast<Engine*>(glfwGetWindowUserPointer(win));
        if (th->inputEventFunc != nullptr) {
            InputEventInfo info;
            info.type = InputEventType::InputEventMouseMove;
            info.position = {x, y};
            th->inputEventFunc(th, info);
        }
    });

    glfwSetKeyCallback(window, [] (GLFWwindow *win, int key, int scancode, int action, int mods) {
        Engine *th = static_cast<Engine*>(glfwGetWindowUserPointer(win));
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
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        for (int o = 0; o < 10; ++o)
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        // Sleep correction
        int64_t d = (std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() - 10) / 10;
        this->fixedProcessDelayMCS = 1000000 / (fixedLoopRefreshRate) - d;
        this->physicsEngineIterateDelayMCS = 1000000 / physicsEngineIterateLoopRefreshRate - d;
    #elif __APPLE__
        sched_param sch_params;
        sch_params.sched_priority = 99;
        pthread_setschedparam(pthread_self(), SCHED_RR, &sch_params);

        int64_t fixDelay = 1000000 / fixedLoopRefreshRate;
        int64_t physDelay = 1000000 / physicsEngineIterateLoopRefreshRate;

        // FIXME: Добавить условие проверки на идентичность обновления физики и фикса
        //        Изменить назавания переменных
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        for (int o = 0; o < 10; ++o)
            std::this_thread::sleep_for(std::chrono::microseconds(fixDelay));
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        int64_t d_fp = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() / 10 - fixDelay;

        t1 = std::chrono::high_resolution_clock::now();
        for (int o = 0; o < 10; ++o)
            std::this_thread::sleep_for(std::chrono::microseconds(physDelay));
        t2 = std::chrono::high_resolution_clock::now();
        int64_t d_pp = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() / 10 - physDelay;

        this->fixedProcessDelayMCS = fixDelay - d_fp;
        this->physicsEngineIterateDelayMCS = physDelay - d_pp;
    #elif _WIN32
        // TODO: Fix Win
        this->fixedProcessDelayMCS = 1000000 / fixedLoopRefreshRate;
        this->physicsEngineIterateDelayMCS = 1000000 / physicsEngineIterateLoopRefreshRate;
    #endif
}


void Engine::Run() {
    std::thread *fixedProcessThread = nullptr;
    if (this->fixedProcessLoop != nullptr)
        fixedProcessThread = new std::thread(&Engine::threadFixedProcessLoop, this);

    std::thread physicsEngineProcessThread = std::thread(
            &Engine::threadPhysicsEngineIterateLoop, this
    );



    GLfloat mat_specular[]={1.0,1.0,1.0,1.0};
    GLfloat mat_shininess[]={50.0};
    GLfloat light_position[]={1.0,1.0,1.0,1.0};
    GLfloat white_light[]={1.0,1.0,1.0,1.0};
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_SMOOTH);
    //glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    //glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
    GLfloat light_ambient[]={1.0,1.0,1.0,1.0};
    /*glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);*/
    //glLightfv(GL_LIGHT0,GL_DIFFUSE,white_light);
    //glLightfv(GL_LIGHT0,GL_SPECULAR,white_light);
    //glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_DEPTH_TEST);

    OpenGL15 ogl;

    glfwSwapBuffers(this->window);
    double oldTime = glfwGetTime();
    while (not glfwWindowShouldClose(this->window)) {
        double delta = glfwGetTime() - oldTime;
        oldTime = glfwGetTime();
        glfwPollEvents();

        //meshesMutex.lock();
        if (this->processLoop != nullptr)
            this->processLoop(this, delta);
        //meshesMutex.unlock();



        //meshesMutex.lock();
        /*for (const auto& o : particles_obj) {
            Particles* p = (HateEngine::Particles*)o.second.obj;
            if (not p->pause)
                p->update(delta);
        }*/
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
    if (physicsEngineProcessThread.joinable())
        physicsEngineProcessThread.join();
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