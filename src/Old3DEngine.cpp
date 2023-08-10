#include "Old3DEngine.h"
#include "Render/OpenGL15.h"
#include "Error.h"
#include <thread>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/ext.hpp>

using namespace Old3DEngine;
Engine::Engine(std::string window_lbl, int width, int height) {
    glfwInit();

    // Create window
    this->window = glfwCreateWindow(width, height, window_lbl.c_str(), NULL, NULL);
    if (this->window == NULL) {
        Error::throwError("Failed to create GLFW window", false);
        glfwTerminate();
    }

    glfwMakeContextCurrent(this->window);
    glfwSwapInterval( 0 );
    glfwSetFramebufferSizeCallback(this->window, [] (GLFWwindow* win, int w, int h) {
        glViewport(0, 0, w, h);
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

void Engine::Run() {
    if (this->fixedProcessLoop != nullptr) {
        std::thread fixedProcessThread(&Engine::threadFixedProcessLoop, this);
        fixedProcessThread.detach();
    }

    float fovy = 60.0f;
    float aspect = 800.0 / float(600);
    float znear = 0.1f;
    float zfar = 100.0f;
    glm::mat4 Mp = glm::perspective(glm::radians(fovy), aspect, znear, zfar);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(Mp));

    glm::vec3 eye = glm::vec3(0, 0, 2);
    glm::vec3 lookat = glm::vec3(0, 0, 0);
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::mat4 M = glm::lookAt(eye, lookat, up);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(M));

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_DEPTH_TEST);
    //glShadeModel(GL_FLAT);

    //glEnableClientState(GL_IN)

    OpenGL15 ogl;
    //gl.Draw();
    glfwSwapBuffers(this->window);
    double oldTime = glfwGetTime();
    while (not glfwWindowShouldClose(this->window)) {
        double delta = glfwGetTime() - oldTime;
        oldTime = glfwGetTime();

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
