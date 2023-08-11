#include "Old3DEngine.h"
#include "Render/OpenGL15.h"
#include "Error.h"
#include <thread>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/ext.hpp>

#include "Objects/CubeMesh.h"

using namespace Old3DEngine;
Engine::Engine(std::string window_lbl, int width, int height) {
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
    glfwSetFramebufferSizeCallback(this->window, [] (GLFWwindow *win, int w, int h) {
        Engine *th = static_cast<Engine*>(glfwGetWindowUserPointer(win));
        th->frameBufferSizeChange(win, w, h);
    });

    // Load Glad
    int version = gladLoadGL(glfwGetProcAddress);
    if (not version) {
        Error::throwError("Failed to initialize GLAD", false);
        glfwTerminate();
    }

    // Calculating the delay between FixedProcessLoop iterations
    this->fixedProcessDelayMCS = 1000000 / fixedLoopRefreshRate;
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
    if (this->fixedProcessLoop != nullptr) {
        std::thread fixedProcessThread(&Engine::threadFixedProcessLoop, this);
        fixedProcessThread.detach();
    }

    if (cameraObject == nullptr) {
        Error::throwError("Camera is not installed", false);
        glfwTerminate();
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_DEPTH_TEST);
    //glShadeModel(GL_FLAT);

    //glEnableClientState(GL_IN)

    CubeMesh mesh1;
    CubeMesh mesh2;
    mesh1.setPosition({-1, 0, 0});
    mesh2.setPosition({1, 0, 0});
    mesh1.setRotation({10, 20, 30});
    std::cout << glm::normalize(mesh1.getRotation()).x << "\n";

    OpenGL15 ogl;
    auto a1 = ogl.addObjectRef(&mesh1);
    auto a2 = ogl.addObjectClone(mesh2);

    //gl.Draw();
    glfwSwapBuffers(this->window);
    double oldTime = glfwGetTime();
    while (not glfwWindowShouldClose(this->window)) {
        /*mesh1.setPosition(mesh1.getPosition().x + 0.001, mesh1.getPosition().y, mesh1.getPosition().z);
        mesh2.setPosition(mesh2.getPosition().x + 0.001, mesh2.getPosition().y, mesh2.getPosition().z);*/
        mesh1.setRotation(mesh1.getRotation() + glm::vec3(0, 0.001, 0));

        double delta = glfwGetTime() - oldTime;
        oldTime = glfwGetTime();

        if (this->renderApi == RenderAPI::OpenGL_1_5)
            this->cameraObject->renderOpenGL15();

        if (this->processLoop != nullptr)
            this->processLoop(delta);

        ogl.Draw();

        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }
}

void Engine::threadFixedProcessLoop() {
    double oldTime = glfwGetTime();
    double delta;
    double func_delta = 0.0;

    while (not glfwWindowShouldClose(this->window)) {
        std::this_thread::sleep_for(
                std::chrono::microseconds(this->fixedProcessDelayMCS - int64_t(func_delta * 1000000))
        );
        delta = glfwGetTime() - oldTime;
        oldTime = glfwGetTime();

        fixedProcessLoop(delta);
        func_delta = glfwGetTime() - oldTime;
    }
}


void Engine::setProcessLoop(void (*func)(double)) {
    this->processLoop = func;
}

void Engine::setFixedProcessLoop(void (*func)(double)) {
    this->fixedProcessLoop = func;
}

void Engine::setCameraRef(Camera *cam) {
    this->cameraObject = cam;
}
