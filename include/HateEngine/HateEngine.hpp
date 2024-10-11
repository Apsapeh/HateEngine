#pragma once
#include <GLFW/glfw3.h>
#include <cstdint>
#include <mutex>
#include <string>
// #include "Utilities/UUID_Generator.hpp"
#include "Resources/Level.hpp"
#include "Input.hpp"

namespace HateEngine {
    class Engine {
    private:

        friend class InputClass;

    public:
        enum RenderAPI { OpenGL_1_5 };

        enum InputEventType {
            InputEventKey,
            InputEventMouseButton,
            InputEventMouseMove,
            InputEventMouseScroll
        };


        struct SceneObject {
            Object* obj;
            // UUID_Generator::UUID id;
            bool is_ref;
        };

        struct InputEventInfo {
            InputEventType type;
            int key, scancode, action, mods = 0;
            glm::vec2 offset = {0, 0};
            glm::vec2 position = {0, 0};
        };

        GLFWwindow* window;

    private:
        void (*processLoop)(Engine*, double) = nullptr;
        void (*fixedProcessLoop)(Engine*, double) = nullptr;
        void (*inputEventFunc)(Engine*, InputEventInfo) = nullptr;

        RenderAPI renderApi = RenderAPI::OpenGL_1_5;
        uint16_t fixedLoopRefreshRate = 60;
        int64_t fixedProcessDelayMCS; // 1_000_000 / fixedLoopRefreshRate
        uint16_t physicsEngineIterateLoopRefreshRate = 120;
        int64_t physicsEngineIterateDelayMCS;

        bool isRunned = false;

        bool isOneThread = false;
        bool isVSync = true;
        bool isMouseCaptured = false;
        bool isFullScreen = false;

        glm::ivec2 resolution = {0, 0};
        glm::vec2 displayScale = {0.0f, 0.0f};
        float aspectRatio;
        Level* level;

        std::string windowTitle = "";
        bool needChangeMouseCaptureMode = false;
        bool needChangeWindowTitle = false;
        bool needChangeFullScreenMode = false;

        // Scene objects
        // std::unordered_map<UUID_Generator::UUID, SceneObject> objects;

        void threadFixedProcessLoop();
        // void frameBufferSizeChange(GLFWwindow* win, int w, int h);
        void threadPhysicsEngineIterateLoop();

    public:
        InputClass Input;

        Engine(std::string window_lbl, int width, int height);
        void Run();
        void Exit();

        void changeWindowTitle(std::string title);

        void setResolution(int width, int height);
        void setOneThreadMode(bool mode);
        void setVSync(bool mode);
        void setMouseCapture(bool capture);
        void setFullScreen(bool fullScreen);

        glm::ivec2 getResolution();
        glm::vec2 getDisplayScale();
        float getAspectRatio();
        bool getOneThreadMode();
        bool getVSync();
        bool getMouseCapture();
        bool getFullScreen();

        void setProcessLoop(void (*func)(Engine*, double));
        void setFixedProcessLoop(void (*func)(Engine*, double));
        void setInputEvent(void (*func)(Engine*, InputEventInfo));

        void setLevelRef(Level* lvl);
        Level* getLevel();
        // UUID_Generator::UUID addObjectClone(Object object);

        GLFWwindow* getWindow();
    };
} // namespace HateEngine
