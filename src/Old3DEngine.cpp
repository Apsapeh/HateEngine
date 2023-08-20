#include "Old3DEngine.hpp"
#include "Render/OpenGL15.hpp"
#include "Error.hpp"
#include <thread>
#include <algorithm>
#include <iostream>

using namespace Old3DEngine;
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
        th->frameBufferSizeChange(win, w, h);
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

    // Load Glad
    int version = gladLoadGL(glfwGetProcAddress);
    if (not version) {
        Error::throwError("Failed to initialize GLAD", false);
        glfwTerminate();
    }

    // Calculating the delay between FixedProcessLoop iterations
    #ifdef __linux__
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        for (int o = 0; o < 10; ++o)
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        // Sleep correction
        int64_t d = (std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() - 10) / 10;
        this->fixedProcessDelayMCS = 1000000 / (fixedLoopRefreshRate) - d;
    #elif _WIN32
        this->fixedProcessDelayMCS = 1000000 / fixedLoopRefreshRate;
    #endif
}

void Engine::frameBufferSizeChange(GLFWwindow* win, int w, int h) {
    glViewport(0, 0, w, h);
    if (this->cameraObject == nullptr) {
        // WARNING
        return;
    }

    cameraObject->setViewAspect(float(w) / float(h));

    if (this->renderApi == RenderAPI::OpenGL_1_5)
        this->cameraObject->renderOpenGL15();
}


void Engine::Run() {
    std::thread *fixedProcessThread = nullptr;
    if (this->fixedProcessLoop != nullptr)
        fixedProcessThread = new std::thread(&Engine::threadFixedProcessLoop, this);


    if (cameraObject == nullptr) {
        Error::throwError("Camera is not installed", false);
        glfwTerminate();
    }

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
    glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_DEPTH_TEST);

    OpenGL15 ogl(&meshes, &lights);

    glfwSwapBuffers(this->window);
    double oldTime = glfwGetTime();
    while (not glfwWindowShouldClose(this->window)) {
        double delta = glfwGetTime() - oldTime;
        oldTime = glfwGetTime();
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        //glfwCal

        meshesMutex.lock();
        if (this->processLoop != nullptr)
            this->processLoop(this, delta);
        //meshesMutex.unlock();

        if (this->renderApi == RenderAPI::OpenGL_1_5)
            this->cameraObject->renderOpenGL15();

        //meshesMutex.lock();
        ogl.Draw();
        meshesMutex.unlock();

        glfwSwapBuffers(this->window);
    }

    if (fixedProcessThread != nullptr and fixedProcessThread->joinable())
        fixedProcessThread->join();
}

void Engine::threadFixedProcessLoop() {
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

        meshesMutex.lock();
        fixedProcessLoop(this, delta);
        meshesMutex.unlock();
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

void Engine::setCameraRef(Camera *cam) {
    this->cameraObject = cam;
}




UUID_Generator::UUID Engine::addObjectClone(Object object) {
    UUID_Generator::UUID id = uuidGenerator.gen();
    Object *new_mesh = new Object;
    *new_mesh = object;
    objects.push_back({new_mesh, id, false});
    return id;
}

UUID_Generator::UUID Engine::addObjectClone(Mesh object) {
    std::lock_guard<std::mutex> guard(meshesMutex);
    UUID_Generator::UUID id = uuidGenerator.gen();
    Mesh *new_mesh = new Mesh;
    *new_mesh = object;
    meshes.push_back({new_mesh, id, false});
    return id;
}

UUID_Generator::UUID Engine::addObjectClone(Light object) {
    std::lock_guard<std::mutex> guard(meshesMutex);
    UUID_Generator::UUID id = uuidGenerator.gen();
    Light *new_mesh = new Light(object.getType());
    *new_mesh = object;
    lights.push_back({new_mesh, id, false});
    return id;
}

UUID_Generator::UUID Engine::addObjectRef(Object *object) {
    UUID_Generator::UUID id = uuidGenerator.gen();
    objects.push_back({object, id, true});
    return id;
}

UUID_Generator::UUID Engine::addObjectRef(Mesh *object) {
    std::lock_guard<std::mutex> guard(meshesMutex);
    UUID_Generator::UUID id = uuidGenerator.gen();
    meshes.push_back({object, id, true});
    return id;
}

UUID_Generator::UUID Engine::addObjectRef(Light *object) {
    std::lock_guard<std::mutex> guard(meshesMutex);
    UUID_Generator::UUID id = uuidGenerator.gen();
    lights.push_back({object, id, true});
    return id;
}

bool Engine::removeObject(UUID_Generator::UUID uuid) {
    std::lock_guard<std::mutex> guard(meshesMutex);
    // FIXME: OPTIMIZE IT!!!
    std::vector<Engine::SceneObject>::iterator iter = std::find_if(
            objects.begin(), objects.end(), [&uuid] (SceneObject &obj) -> bool {return obj.id == uuid;}
    );
    if (iter != objects.end() and iter->id == uuid) {
        if (not iter->is_ref)
            delete iter->obj;
        objects.erase(iter);
        return true;
    }

    iter = std::find_if(
            meshes.begin(), meshes.end(), [&uuid] (SceneObject &obj) -> bool {return obj.id == uuid;}
    );
    if (iter != meshes.end() and iter->id == uuid) {
        if (not iter->is_ref)
            delete iter->obj;
        meshes.erase(iter);
        return true;
    }

    return false;
}


