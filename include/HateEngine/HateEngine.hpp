#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>
#include <mutex>
#include <GLFW/glfw3.h>
#include "Objects/Camera.hpp"
//#include "Utilities/UUID_Generator.hpp"
#include "Objects/Mesh.hpp"
#include "Objects/Light/Light.hpp"
#include "Objects/Particles.hpp"
#include "Objects/Model.hpp"
#include "PhysEngine.hpp"
#include "glm/vec2.hpp"
#include "Resources/Level.hpp"

namespace HateEngine {
    class Engine {
    private:
        class InputClass {
            Engine *engine;
        public:
            InputClass(Engine*);
            bool isKeyPressed(int key);
            glm::vec2 getVector(int left, int right, int up, int down) ;
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
            //UUID_Generator::UUID id;
            bool is_ref;
        };


        struct InputEventInfo {
            InputEventType type;
            int key, scancode, action, mods = 0;
            glm::vec2 offset = {0, 0};
            glm::vec2 position = {0, 0};
        };
        
        

        GLFWwindow *window;

    private:
        void (*processLoop)(Engine*, double) = nullptr;
        void (*fixedProcessLoop)(Engine*, double) = nullptr;
        void (*inputEventFunc)(Engine*, InputEventInfo) = nullptr;

        RenderAPI renderApi = RenderAPI::OpenGL_1_5;
        uint16_t fixedLoopRefreshRate = 60;
        int64_t fixedProcessDelayMCS; // 1_000_000 / fixedLoopRefreshRate
        uint16_t physicsEngineIterateLoopRefreshRate = 120;
        int64_t physicsEngineIterateDelayMCS;
        
        bool isOneThread = true;

        glm::ivec2 resolution;
        float aspectRatio;
        Level* level;

        // Scene objects
        //std::unordered_map<UUID_Generator::UUID, SceneObject> objects;


        void threadFixedProcessLoop();
        //void frameBufferSizeChange(GLFWwindow* win, int w, int h);
        void threadPhysicsEngineIterateLoop();



    public:
        InputClass Input;

        Engine(std::string window_lbl, int width, int height);
        void Run();
        
        void setResolution(int width, int height);
        
        glm::ivec2 getResolution();
        float getAspectRatio();
        
        void setProcessLoop(void (*func)(Engine*, double));
        void setFixedProcessLoop(void (*func)(Engine*, double));
        void setInputEvent(void (*func)(Engine*, InputEventInfo));

        

        void setLevelRef(Level *lvl);
        Level* getLevel();
        //UUID_Generator::UUID addObjectClone(Object object);
        
        
        


        GLFWwindow* getWindow();
    };
}