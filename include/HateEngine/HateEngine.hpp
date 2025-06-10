#pragma once
#include <cstdint>
#include <mutex>
#include <string>
#include "HateEngine/InputEvent.hpp"
#include "HateEngine/Render/RenderInterface.hpp"
#include "HateEngine/Utilities/UUID.hpp"
#include "Utilities/Signal.hpp"
#include "Level.hpp"
#include "Input.hpp"
#include "OSDriverInterface.hpp"

namespace HateEngine {
    class Engine {
    private:
        friend class InputClass;

    public:
        enum RenderAPI { OpenGL_1_3 };


    private:
        void (*processLoop)(Engine*, double) = nullptr;
        void (*fixedProcessLoop)(Engine*, double) = nullptr;
        void (*inputEventFunc)(Engine*, const InputEventInfo&) = nullptr;

        RenderAPI renderApi = RenderAPI::OpenGL_1_3;
        uint16_t fixedLoopRefreshRate = 60;
        int64_t fixedProcessDelayMCS; // 1_000_000 / fixedLoopRefreshRate
        uint16_t physicsEngineIterateLoopRefreshRate = 120;
        int64_t physicsEngineIterateDelayMCS;
        uint16_t audioEngineIterateLoopRefreshRate = 30;

        RenderInterface* renderInterface = nullptr;
        long lastCPUTime = 0;

        bool isRunned = false;

        bool isOneThread = false;
        bool isVSync = true;

        double frameDelta = 0.0;

        glm::ivec2 resolution = {0, 0};
        glm::vec2 displayScale = {0.0f, 0.0f};
        float aspectRatio = 0;
        Level* level = nullptr;
        UUID level_callback_added_uuid = 0;
        UUID level_callback_removed_uuid = 0;


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

        void _inputEvent(const InputEventInfo& event);

        void __changeWindowTitle(ThreadSafeRequest req);
        void __changeMouseCaptureMode(ThreadSafeRequest req);
        void __changeFullScreenMode(ThreadSafeRequest req);
        void __changeLevelRef(ThreadSafeRequest req);

        void __updateResolution(int width, int height);

    public:
        OSDriverInterface OSDriver;
        OSDriverInterface::OSWindow mainWindow;
        InputClass Input;

        // New Level, Old Level
        Signal<Engine*, Level*, Level*> onLevelChanged;
        // New resolution, Old resolution
        Signal<Engine*, const glm::ivec2, const glm::ivec2> onResolutionChanged;
        // New scale, Old scale
        Signal<Engine*, const glm::vec2, const glm::vec2> onDisplayScaleChanged;
        // New aspect ratio, Old aspect ratio
        Signal<Engine*, const float, const float> onAspectRatioChanged;

        Engine(std::string window_lbl, int width, int height);
        void Run();
        void Exit();

        void changeWindowTitle(std::string title);

        void setResolution(int width, int height);
        void setOneThreadMode(bool mode);
        void setVSync(bool mode);
        void setMouseCapture(bool capture);
        void setFullScreen(bool fullScreen);

        RenderInterface* getRenderInterface();
        RenderAPI getRenderAPI();

        /**
         * @brief Return the GPU time in milliseconds
         * @warning Only for 32-bit systems. If GPU rendering takes more, then 2.147 seconds, the
         * result will be incorrect (long overflow)
         *
         * @return double
         */
        double getGPUTimeMS();

        /**
         * @brief Return the CPU time in milliseconds
         * @warning Only for 32-bit systems. If CPU takes more, then 2.147 seconds, the
         * result will be incorrect (long overflow)
         *
         * @return double
         */
        double getCPUTimeMS();

        /**
         * @brief Return the delta time in seconds between frames
         *
         * @return double
         */
        double getFrameDelta();

        glm::ivec2 getResolution();
        glm::vec2 getDisplayScale();
        float getAspectRatio();
        bool getOneThreadMode();
        bool getVSync();
        bool getMouseCapture();
        bool getFullScreen();

        void setProcessLoop(void (*func)(Engine*, double));
        void setFixedProcessLoop(void (*func)(Engine*, double));
        void setInputEvent(void (*func)(Engine*, const InputEventInfo&));

        /**
         * @brief Thread safe request to change the scene
         *
         * @note It's not change scene immediately, only before the next frame
         * @param lvl
         */
        void requestChangeLevel(Level* lvl);
        /**
         * @brief Set the scene. NOT THREAD SAFE
         *
         * @warning It's set scene immediately, but not thread safe
         * @param lvl The level to set
         */
        void setLevel(Level* lvl);
        Level* getLevel();
    };
} // namespace HateEngine
