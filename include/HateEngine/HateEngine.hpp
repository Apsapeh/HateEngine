#pragma once
#include <GLFW/glfw3.h>
#include <cstdint>
#include <mutex>
#include <string>
#include "Utilities/Signal.hpp"
#include "Level.hpp"
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
            Object* obj = nullptr;
            // UUID_Generator::UUID id;
            bool is_ref = false;
        };

        struct InputEventInfo {
            InputEventType type;
            union {
                Key key;
                MouseButton button;
            };
            int raw_key, scancode, mods = 0;
            bool isPressed;
            glm::vec2 offset = {0, 0};
            glm::vec2 position = {0, 0};
        };

        GLFWwindow* window = nullptr;

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

        glm::ivec2 resolution = {0, 0};
        glm::vec2 displayScale = {0.0f, 0.0f};
        float aspectRatio = 0;
        Level* level = nullptr;

        std::mutex levelMutex;

        /* Thread safety queue operations*/
        enum ThreadSafeRequestType {
            ChangeWindowTitle,
            ChangeMouseCaptureMode,
            ChangeFullScreenMode,
            ChangeLevelRef
        };

        struct ThreadSafeRequest {
            ThreadSafeRequestType type;
            void* data = nullptr;
        };

        std::vector<ThreadSafeRequest> threadSafeRequestsQueue;

        // Scene objects
        // std::unordered_map<UUID_Generator::UUID, SceneObject> objects;

        void threadFixedProcessLoop();
        // void frameBufferSizeChange(GLFWwindow* win, int w, int h);
        void threadPhysicsEngineIterateLoop();

        void __changeWindowTitle(ThreadSafeRequest req);
        void __changeMouseCaptureMode(ThreadSafeRequest req);
        void __changeFullScreenMode(ThreadSafeRequest req);
        void __changeLevelRef(ThreadSafeRequest req);

    public:
        InputClass Input;

        Signal<Engine*, Level*, Level*> onLevelChanged;

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

        /**
         * @brief Thread safe request to change the scene
         *
         * @note It's not change scene immediately, only before the next frame
         * @param lvl
         */
        void changeLevelRef(Level* lvl);
        /**
         * @brief Set the scene. NOT THREAD SAFE
         *
         * @warning It's set scene immediately, but not thread safe
         * @param lvl The level to set
         */
        void setLevelRef(Level* lvl);
        Level* getLevel();
        // UUID_Generator::UUID addObjectClone(Object object);

        GLFWwindow* getWindow();
    };
} // namespace HateEngine
