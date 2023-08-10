#pragma once
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>

namespace Old3DEngine {
    class Engine {
    private:
        void (*processLoop)(double) = nullptr;

        void (*fixedProcessLoop)(double) = nullptr;

        GLFWwindow *window;
        uint16_t fixedLoopRefreshRate = 60;
        int64_t fixedProcessDelayMCS; // 1_000_000 / fixedLoopRefreshRate

        void threadFixedProcessLoop();

    public:
        Engine(std::string window_lbl, int width, int height);

        void Run();

        void setProcessLoop(void (*func)(double));

        void setFixedProcessLoop(void (*func)(double));
        //void setInputEvent(void (*func)());
    };
}