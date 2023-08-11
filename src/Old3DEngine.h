#pragma once
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include "Objects/Camera.h"
#include "Utilities/UUID_Generator.h"
#include "Objects/Mesh.h"

namespace Old3DEngine {
    class Engine {
    public:
        enum RenderAPI {
            OpenGL_1_5
        };

        struct SceneObject {
            Object *obj;
            UUID_Generator::UUID id;
            bool is_ref;
        };

    public:
        void (*fixedProcessLoopTEST)(Engine*, double) = nullptr;


    private:
        void (*processLoop)(Engine*, double) = nullptr;
        void (*fixedProcessLoop)(Engine*, double) = nullptr;

        GLFWwindow *window;
        RenderAPI renderApi = RenderAPI::OpenGL_1_5;
        uint16_t fixedLoopRefreshRate = 60;
        int64_t fixedProcessDelayMCS; // 1_000_000 / fixedLoopRefreshRate

        Camera* cameraObject = nullptr;
        UUID_Generator uuidGenerator;

        std::vector<SceneObject> objects;
        std::vector<SceneObject> meshes;

        void threadFixedProcessLoop();
        void frameBufferSizeChange(GLFWwindow* win, int w, int h);

    public:
        Engine(std::string window_lbl, int width, int height);
        void Run();

        void setProcessLoop(void (*func)(Engine*, double));
        void setFixedProcessLoop(void (*func)(Engine*, double));

        void setCameraRef(Camera *cam);

        UUID_Generator::UUID addObjectClone(Object object);
        UUID_Generator::UUID addObjectClone(Mesh object);
        UUID_Generator::UUID addObjectRef(Object* object);
        UUID_Generator::UUID addObjectRef(Mesh* object);

        bool removeObject(UUID_Generator::UUID uuid);


        //void setInputEvent(void (*func)());
    };
}