#pragma once
#include <string>
#include <cstdint>
#include <mutex>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Objects/Camera.hpp"
#include "Utilities/UUID_Generator.hpp"
#include "Objects/Mesh.hpp"
#include "Objects/Light/Light.hpp"
#include "glm/vec2.hpp"

namespace Old3DEngine {
    class Engine {
    private:
        class InputClass {
            Engine *engine;
        public:
            InputClass(Engine*);
            bool isKeyPressed(int key);
            glm::vec2 getVector(int left, int right, int up, int down);
            //bool isKeyPressed(int key);
        };
        friend class InputClass;

    public:
        enum RenderAPI {
            OpenGL_1_5
        };

        enum InputEventType {
            InputEventKey,
            InputEventMouseMove,
            InputEventMouseScroll
        };

        struct SceneObject {
            Object *obj;
            UUID_Generator::UUID id;
            bool is_ref;
        };


        struct InputEventInfo {
            InputEventType type;
            int key, scancode, action, mods = 0;
            glm::vec2 offset = {0, 0};
            glm::vec2 position = {0, 0};
        };

    public:
        void (*processLoop)(Engine*, double) = nullptr;
        void (*fixedProcessLoop)(Engine*, double) = nullptr;
        void (*inputEventFunc)(Engine*, InputEventInfo) = nullptr;

        GLFWwindow *window;
        RenderAPI renderApi = RenderAPI::OpenGL_1_5;
        uint16_t fixedLoopRefreshRate = 60;
        int64_t fixedProcessDelayMCS; // 1_000_000 / fixedLoopRefreshRate

        Camera* cameraObject = nullptr;
        UUID_Generator uuidGenerator;

        std::vector<SceneObject> objects;
        std::vector<SceneObject> meshes;
        std::vector<SceneObject> lights;

        std::mutex meshesMutex;

        void threadFixedProcessLoop();
        void frameBufferSizeChange(GLFWwindow* win, int w, int h);

    public:
        InputClass Input;

        Engine(std::string window_lbl, int width, int height);
        void Run();

        void setProcessLoop(void (*func)(Engine*, double));
        void setFixedProcessLoop(void (*func)(Engine*, double));
        void setInputEvent(void (*func)(Engine*, InputEventInfo));

        void setCameraRef(Camera *cam);
        UUID_Generator::UUID addObjectClone(Object object);
        UUID_Generator::UUID addObjectClone(Mesh object);
        UUID_Generator::UUID addObjectClone(Light object);
        UUID_Generator::UUID addObjectRef(Object* object);
        UUID_Generator::UUID addObjectRef(Mesh* object);
        UUID_Generator::UUID addObjectRef(Light* object);


        bool removeObject(UUID_Generator::UUID uuid);
    };
}