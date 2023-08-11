#pragma once
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include "Objects/Camera.h"

namespace Old3DEngine {
    class Engine {
    public:
        enum RenderAPI {
            OpenGL_1_5
        };

    private:
        void (*processLoop)(double) = nullptr;
        void (*fixedProcessLoop)(double) = nullptr;

        GLFWwindow *window;
        RenderAPI renderApi = RenderAPI::OpenGL_1_5;
        uint16_t fixedLoopRefreshRate = 60;
        int64_t fixedProcessDelayMCS; // 1_000_000 / fixedLoopRefreshRate

        Camera* cameraObject = nullptr;

        void threadFixedProcessLoop();
        void frameBufferSizeChange(GLFWwindow* win, int w, int h);

    public:
        Engine(std::string window_lbl, int width, int height);
        void Run();

        void setProcessLoop(void (*func)(double));
        void setFixedProcessLoop(void (*func)(double));

        void setCameraRef(Camera *cam);
        //void setInputEvent(void (*func)());
    };
}